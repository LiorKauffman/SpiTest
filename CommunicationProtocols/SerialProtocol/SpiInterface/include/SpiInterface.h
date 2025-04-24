#ifndef _SPI_INTERFACE_H
#define _SPI_INTERFACE_H

#include "Utils/Singleton/include/Singleton.h"
#include "CommunicationProtocols/SerialProtocol/ISerial/include/ISerial.h"
#include <functional>
#include <atomic>
#include <mutex>
#include <thread>
#include <linux/spi/spidev.h>
#include <string>

class SpiInterface : public Singleton<SpiInterface>, public ISerial
{
    friend class Singleton<SpiInterface>;

public:
static constexpr auto SPI_0 = "/dev/spidev0.0";

    enum class SPISpeed : uint32_t 
    {
        SPEED_125_KHZ  = 125000,
        SPEED_250_KHZ  = 250000,
        SPEED_500_KHZ  = 500000,
        SPEED_1_MHZ    = 1000000,
        SPEED_2_MHZ    = 2000000,
        SPEED_4_MHZ    = 4000000,
        SPEED_8_MHZ    = 8000000,
        SPEED_16_MHZ   = 16000000
    };

    void SetProperties(const std::string& deviceName,
        uint32_t baudRateOrSpeed,
        uint8_t mode,
        uint8_t bits,
        std::function<void(const std::vector<uint8_t>&)>) override;
    bool Transmit(const std::vector<uint8_t>& tx) override;
    bool Receive(std::vector<uint8_t>& rx, size_t length) override;
    bool Transfer(const std::vector<uint8_t>& tx, std::vector<uint8_t>& rx);

    inline const std::string& GetDeviceName() const override { return _deviceName; }
    inline uint32_t GetCurrentSpeed() const override { return _currentSpeed; }
    inline bool IsRunning() const override { return _isRunning; }
    inline auto SetRxCompletedHandler(std::function<void(const std::vector<uint8_t>&)> rxCompletedHandler)
    {
        _rxCompletedHandler = rxCompletedHandler;
    }

private:
    SpiInterface() = default;
    ~SpiInterface() = default;

    std::string _deviceName;
    uint32_t _currentSpeed;
    int _spiInstance;
    // bool _isRunning;
    std::mutex _mutex;
    std::atomic<bool> _isRunning;
    std::thread _receiveThread;
    
    

    void _ReceiveTask();
};

#endif // _SPI_INTERFACE_H