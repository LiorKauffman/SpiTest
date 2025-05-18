#include "CommunicationProtocols/SerialProtocol/ISerial/include/ISerial.h"

void ISerial::SetProperties(const std::string& deviceName,
    uint32_t baudRateOrSpeed,
    uint8_t mode,
    uint8_t bits,
    std::function<void(const std::vector<uint8_t>&)> rxCompletedHandler)
{
    _rxCompletedHandler = rxCompletedHandler;
    _deviceName = deviceName;
    _currentSpeed = baudRateOrSpeed;
}

bool ISerial::Transmit(const std::vector<uint8_t>& tx)
{
    std::lock_guard<std::mutex> lock(_mutex);
    
    printf("Length: %d\n", tx.size());
    printf("Send: ");

    for (const auto& byte : tx)
    {
        printf("0x%x ", byte);
    }

    printf("\n");

    return write(_serialInstance, tx.data(), tx.size()) == static_cast<ssize_t>(tx.size());
}

bool ISerial::Receive(std::vector<uint8_t>& rx, size_t length)
{
    std::lock_guard<std::mutex> lock(_mutex);
    auto retValue = true;

    rx.resize(length);

    auto bytesRead = read(_serialInstance, rx.data(), length);

    if (bytesRead < 0)
    {
        retValue = false;
    }
    else
    {
        rx.resize(bytesRead);
    }

    return retValue;
}