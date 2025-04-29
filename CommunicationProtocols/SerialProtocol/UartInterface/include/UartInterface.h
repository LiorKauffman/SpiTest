#ifndef _UART_INTERFACE_H
#define _UART_INTERFACE_H

#include "Utils/Singleton/include/Singleton.h"
#include "CommunicationProtocols/SerialProtocol/ISerial/include/ISerial.h"
// #include <functional>
// #include <atomic>
// #include <mutex>
// #include <thread>
// #include <string>


class UartInterface : public Singleton<UartInterface>, public ISerial
{
    friend class Singleton<UartInterface>;

public:
    // static constexpr auto UART_0 = "/dev/serial0";
    static constexpr auto UART_0 = "/dev/ttyAMA0";
    void SetProperties(const std::string& deviceName,
        uint32_t baudRateOrSpeed,
        uint8_t mode,
        uint8_t bits,
        std::function<void(const std::vector<uint8_t>&)>) override;
    // bool Transmit(const std::vector<uint8_t>& tx) override;
    // bool Receive(std::vector<uint8_t>& rx, size_t length) override;
    
    private:
    UartInterface() = default;
    ~UartInterface() = default;
    
    void _ReceiveTask() override;

};

#endif //_UART_INTERFACE_H