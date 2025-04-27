#include "CommunicationProtocols/SerialProtocol/SpiInterface/include/SpiInterface.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include <stdexcept>

// void SpiInterface::SetProperties(const std::string& deviceName,
//                                 uint32_t spiSpeed,
//                                 uint8_t mode,
//                                 uint8_t bits,
//                                 std::function<void(const std::vector<uint8_t>&)> rxCompletedHandler)
// {
//     _rxCompletedHandler = rxCompletedHandler;
//     _deviceName = deviceName;
//     _currentSpeed = spiSpeed;

//     printf("Device name: %s\n", _deviceName.c_str());

//     _spiInstance = open(_deviceName.c_str(), O_RDWR);

//     if (_spiInstance < 0)
//     {
//         perror("SPI Open");
//         throw std::runtime_error("failed to open spi device");
//     }

    
    
//     auto currentMode = mode;
//     auto currentBits = bits;

//     printf("Before!\n");
//     ioctl(_spiInstance, SPI_IOC_WR_MODE, &currentMode);
//     ioctl(_spiInstance, SPI_IOC_RD_BITS_PER_WORD, &currentBits);
//     ioctl(_spiInstance, SPI_IOC_RD_MAX_SPEED_HZ, &_currentSpeed);

//     printf("Hety!\n");

//     if (_isRunning) return;
    
//     _isRunning = true;
//     _receiveThread = std::thread(&SpiInterface::_ReceiveTask, this);

//     printf("Here!\n");
// }

void SpiInterface::SetProperties(const std::string& deviceName,
    uint32_t spiSpeed,
    uint8_t mode,
    uint8_t bits,
    std::function<void(const std::vector<uint8_t>&)> rxCompletedHandler)
{
_rxCompletedHandler = rxCompletedHandler;
_deviceName = deviceName;
_currentSpeed = spiSpeed;

printf("Device name: %s\n", _deviceName.c_str());

_spiInstance = open(_deviceName.c_str(), O_RDWR);
printf("_spiInstance = %d\n", _spiInstance);
if (_spiInstance < 0)
{
perror("SPI Open");
// throw std::runtime_error("Failed to open SPI device");
}

// Set SPI mode (MODE0, MODE1, etc.)
if (ioctl(_spiInstance, SPI_IOC_WR_MODE, &mode) == -1)
{
perror("SPI Set Mode");
// throw std::runtime_error("Failed to set SPI mode");
}

// printf("Line = %d\n", __LINE__);

// Set bits per word
if (ioctl(_spiInstance, SPI_IOC_WR_BITS_PER_WORD, &bits) == -1)
{
perror("SPI Set Bits");
// throw std::runtime_error("Failed to set bits per word");
}

// Set max speed
if (ioctl(_spiInstance, SPI_IOC_WR_MAX_SPEED_HZ, &_currentSpeed) == -1)
{
perror("SPI Set Speed");
// throw std::runtime_error("Failed to set SPI speed");
}


printf("SPI configured: mode=%u, bits=%u, speed=%u Hz\n", mode, bits, _currentSpeed);

if (_isRunning) return;

// printf("Line = %d\n", __LINE__);
_isRunning = true;
_receiveThread = std::thread(&SpiInterface::_ReceiveTask, this);
}

bool SpiInterface::Transmit(const std::vector<uint8_t>& txData)
{
    std::lock_guard<std::mutex> lock(_mutex);

    return write(_spiInstance, txData.data(), txData.size()) == static_cast<ssize_t>(txData.size());
}

bool SpiInterface::Receive(std::vector<uint8_t>& rx, size_t length)
{
    std::lock_guard<std::mutex> lock(_mutex);

    rx.resize(length);

    return read(_spiInstance, rx.data(), length) == static_cast<ssize_t>(length);
}

bool SpiInterface::Transfer(const std::vector<uint8_t>& tx, std::vector<uint8_t>& rx)
{
    std::lock_guard<std::mutex> lock(_mutex);

    rx.resize(tx.size());

    struct spi_ioc_transfer transaction = {};
    transaction.tx_buf = reinterpret_cast<unsigned long>(tx.data());
    transaction.rx_buf = reinterpret_cast<unsigned long>(rx.data());
    transaction.len = static_cast<uint32_t>(tx.size());
    transaction.speed_hz = _currentSpeed;
    transaction.bits_per_word = 8;
    transaction.delay_usecs = 0;
    transaction.cs_change = 0;

    int ret = ioctl(_spiInstance, SPI_IOC_MESSAGE(1), &transaction);
    return ret >= 1;
}

// bool SpiInterface::Transfer(const std::vector<uint8_t>& tx, std::vector<uint8_t>& rx)
// {
//     std::lock_guard<std::mutex> lock(_mutex);

//     struct spi_ioc_transfer transaction = {
//         // .tx_buf = reinterpret_cast<unsigned long>(const_cast<uint8_t*>(tx.data()))
//         .tx_buf = (unsigned long)tx.data(),
//         .rx_buf = (unsigned long)rx.data(),
//         .len = static_cast<uint32_t>(tx.size()),
//         .speed_hz = _currentSpeed,
//         .bits_per_word = 8
//     };

//     return ioctl(_spiInstance, SPI_IOC_MESSAGE(1), &transaction) >= 1;
// }

int SpiInterface::Transfer(const uint8_t* tx, uint8_t* rx, uint32_t length)
{
    std::lock_guard<std::mutex> lock(_mutex);

    struct spi_ioc_transfer transaction = {};
    transaction.tx_buf = reinterpret_cast<unsigned long>(tx);
    transaction.rx_buf = reinterpret_cast<unsigned long>(rx);
    transaction.len = length;
    transaction.speed_hz = _currentSpeed;
    transaction.bits_per_word = 8;
    transaction.delay_usecs = 0;
    transaction.cs_change = 1;

    int ret = ioctl(_spiInstance, SPI_IOC_MESSAGE(1), &transaction);
    if (ret < 0) {
        perror("SPI Transfer failed");
    } else if (ret != static_cast<int>(length)) {
        fprintf(stderr, "Warning: only %d/%u bytes transferred\n", ret, length);
    }

    return ret;
}

void SpiInterface::_ReceiveTask()
{
    while (_isRunning)
    {
        // Step 1: Receive length byte (passively listen)
        std::vector<uint8_t> txLen(1, 0x00);  // Dummy transmit
        std::vector<uint8_t> rxLen;

        // if (!Transfer(txLen, rxLen))
        // {
        //     std::this_thread::sleep_for(std::chrono::milliseconds(10));
        //     continue;
        // }

        // if (rxLen.empty())
        // {
        //     std::this_thread::sleep_for(std::chrono::milliseconds(10));
        //     continue;
        // }

        uint8_t expectedLength = 250;

        if (expectedLength == 0 || expectedLength > 255)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            continue;
        }

        // Step 2: Receive data of length `expectedLength`
        std::vector<uint8_t> txDummy(expectedLength, 0x00);  // Dummy transmit for passive reception
        std::vector<uint8_t> rxPayload;

        if (Transfer(txDummy, rxPayload))
        {
            if (_rxCompletedHandler && rxPayload.at(1) == 0X55)
            {
                _rxCompletedHandler(rxPayload);
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}
// void SpiInterface::_ReceiveTask()
// {
//     while (_isRunning) {
//                 // Step 1: read length prefix (1 byte)
//                 std::vector<uint8_t> lengthTx(1, 0x00);
//                 std::vector<uint8_t> lengthRx;
//                 lengthRx.reserve(1);
//                 printf("Line = %d\n", __LINE__);
                
//                 if (!Transfer(lengthTx, lengthRx)) {
//                     std::this_thread::sleep_for(std::chrono::milliseconds(10));
//                     printf("there!!!!\n");
//                     continue;
//                 }
//                 else
//                 {


//                     if (lengthRx.empty())
//                     {
//                         // printf("[ERROR]\n");
//                         std::this_thread::sleep_for(std::chrono::milliseconds(10));
//                         printf("Line = %d\n", __LINE__);
//                         continue;
//                     }
//                 }


//                 printf("Line = %d\n", __LINE__);

        
//                 size_t dynamicLength = static_cast<size_t>(lengthRx[0]);
//                 if (dynamicLength == 0 || dynamicLength > 255) {
//                     std::this_thread::sleep_for(std::chrono::milliseconds(10));
//                     continue;
//                 }

//                 printf("Line = %d\n", __LINE__);

        
//                 // Step 2: read payload
//                 std::vector<uint8_t> txDummy(dynamicLength, 0x00);
//                 std::vector<uint8_t> rx;
//                 printf("Line = %d\n", __LINE__);
//                 rx.reserve(dynamicLength);
//                 auto res = Transfer(txDummy, rx);

//                 printf("res = %d\n", res);
        
//                 if (res) {
//                     if (_rxCompletedHandler) {
//                         _rxCompletedHandler(rx);
//                     }
//                 }

//                 rx.clear();
//                 printf("Line = %d\n", __LINE__);
//                 std::this_thread::sleep_for(std::chrono::milliseconds(10));
//             }
// }



// SPIInterface.cpp
// #include "SPIInterface.h"
// #include <fcntl.h>
// #include <unistd.h>
// #include <sys/ioctl.h>
// #include <iostream>
// #include <stdexcept>

// void SPIInterface::SetProperties(const std::string& deviceName, SPISpeed speed, uint8_t mode, uint8_t bits, PacketHandler handler) {
//     std::lock_guard<std::mutex> lock(_spiMutex);
//     _deviceName = deviceName;
//     _currentSpeed = speed;
//     _mode = mode;
//     _bitsPerWord = bits;
//     _handler = handler;

//     _spiInstance = open(_deviceName.c_str(), O_RDWR);
//     if (_spiInstance < 0) {
//         perror("SPI open");
//         throw std::runtime_error("Failed to open SPI device");
//     }

//     ioctl(_spiInstance, SPI_IOC_WR_MODE, &_mode);
//     ioctl(_spiInstance, SPI_IOC_WR_BITS_PER_WORD, &_bitsPerWord);
//     uint32_t speedHz = static_cast<uint32_t>(_currentSpeed);
//     ioctl(_spiInstance, SPI_IOC_WR_MAX_SPEED_HZ, &speedHz);
// }

// bool SPIInterface::Transmit(const std::vector<uint8_t>& txData) {
//     std::lock_guard<std::mutex> lock(_spiMutex);
//     return write(_spiInstance, txData.data(), txData.size()) == static_cast<ssize_t>(txData.size());
// }

// bool SPIInterface::Receive(std::vector<uint8_t>& rx, size_t length) {
//     std::lock_guard<std::mutex> lock(_spiMutex);
//     rx.resize(length);
//     return read(_spiInstance, rx.data(), length) == static_cast<ssize_t>(length);
// }

// bool SPIInterface::Transfer(const std::vector<uint8_t>& tx, std::vector<uint8_t>& rx) {
//     std::lock_guard<std::mutex> lock(_spiMutex);
//     rx.resize(tx.size());

//     struct spi_ioc_transfer tr = {
//         .tx_buf = (unsigned long)tx.data(),
//         .rx_buf = (unsigned long)rx.data(),
//         .len = static_cast<uint32_t>(tx.size()),
//         .speed_hz = static_cast<uint32_t>(_currentSpeed),
//         .bits_per_word = _bitsPerWord,
//     };

//     return ioctl(_spiInstance, SPI_IOC_MESSAGE(1), &tr) >= 1;
// }

// void SPIInterface::StartReceiving(PacketHandler handler) {
//     if (_isRunning) return;
//     _handler = handler;
//     _isRunning = true;
//     _receiveThread = std::thread(&SPIInterface::ReceiveTask, this);
// }

// void SPIInterface::StopReceiving() {
//     if (!_isRunning) return;
//     _isRunning = false;
//     if (_receiveThread.joinable()) {
//         _receiveThread.join();
//     }
// }

// void SPIInterface::ReceiveTask() {
//     while (_isRunning) {
//         // Step 1: read length prefix (1 byte)
//         std::vector<uint8_t> lengthTx(1, 0x00);
//         std::vector<uint8_t> lengthRx;

//         if (!Transfer(lengthTx, lengthRx)) {
//             std::this_thread::sleep_for(std::chrono::milliseconds(10));
//             continue;
//         }

//         size_t dynamicLength = static_cast<size_t>(lengthRx[0]);
//         if (dynamicLength == 0 || dynamicLength > 255) {
//             std::this_thread::sleep_for(std::chrono::milliseconds(10));
//             continue;
//         }

//         // Step 2: read payload
//         std::vector<uint8_t> txDummy(dynamicLength, 0x00);
//         std::vector<uint8_t> rx;

//         if (Transfer(txDummy, rx)) {
//             if (_handler) {
//                 _handler(rx);
//             }
//         }

//         std::this_thread::sleep_for(std::chrono::milliseconds(10));
//     }
// }

// SPIInterface::~SPIInterface() {
//     StopReceiving();
//     if (_spiInstance >= 0) {
//         close(_spiInstance);
//     }
// }