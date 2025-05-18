#include "CommunicationProtocols/SerialProtocol/UartInterface/include/UartInterface.h"
#include "fcntl.h"
#include "termios.h"
#include "iostream"
#include "cstring"

void UartInterface::SetProperties(const std::string& deviceName,
    uint32_t baudRateOrSpeed,
    uint8_t mode,
    uint8_t bits,
    std::function<void(const std::vector<uint8_t>&)> rxCompletedHandler)
{
    ISerial::SetProperties(deviceName, baudRateOrSpeed, mode, bits, rxCompletedHandler);

    printf(" Device Name: %s\n", _deviceName.c_str());
    
    _serialInstance = open(deviceName.c_str(), O_RDWR | O_NOCTTY);

    if (_serialInstance == -1)
    {
        perror("Uart open");
        // throw std::runtime_error("failed to open uart device");
    }

    struct termios options;
    if (tcgetattr(_serialInstance, &options) != 0)
    {
        perror("tcgetattr!");
        // throw std::runtime_error("Failed to get uart attributes!");
    }

    speed_t baudRate;
    switch (baudRateOrSpeed) 
    {
        case 9600: baudRate = B9600; break;
        case 19200: baudRate = B19200; break;
        case 38400: baudRate = B38400; break;
        case 57600: baudRate = B57600; break;
        case 115200: baudRate = B115200; break;
        case 230400: baudRate = B230400; break;
        default:
            throw std::invalid_argument("Unsupported baud rate");
    }

    cfsetispeed(&options, baudRate);
    cfsetospeed(&options, baudRate);

    options.c_cflag |= (CLOCAL | CREAD);
    options.c_cflag &= ~PARENB;
    options.c_cflag &= ~CSTOPB;
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;

    options.c_lflag = 0;
    options.c_oflag = 0;
    options. c_iflag = 0;

    if (tcsetattr(_serialInstance, TCSANOW, &options) != 0)
    {
        perror("tcsetattr!");
        // throw std::runtime_error("Failed to set Uart attributes!");
    }

    if (_isRunning)
    {
        return;
    }

    _isRunning = true;
    _receiveThread = std::thread(&UartInterface::_ReceiveTask, this);

}

void UartInterface::_ReceiveTask()
{
    while (_isRunning)
    {
        std::vector<uint8_t> buffer(_BUFFER_SIZE);

        auto bytesRead = read(_serialInstance, buffer.data(), buffer.size());

        if (bytesRead > 0)
        {
            buffer.resize(bytesRead);

            if (_rxCompletedHandler)
            {
                _rxCompletedHandler(buffer);
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}
