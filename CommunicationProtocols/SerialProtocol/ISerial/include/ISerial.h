#ifndef _ISERIAL_H
#define _ISERIAL_H

#include <string>
#include <vector>
#include <functional>

class ISerial {
public:
    virtual ~ISerial() = default;

    virtual void SetProperties(const std::string& deviceName,
                                uint32_t baudRateOrSpeed,
                                uint8_t mode,
                                uint8_t bits,
                                std::function<void(const std::vector<uint8_t>&)>) = 0;
    
    virtual bool Transmit(const std::vector<uint8_t>& tx) = 0;
    virtual bool Receive(std::vector<uint8_t>& rx, size_t length) = 0;
    // virtual bool Transfer(const std::vector<uint8_t>& tx, std::vector<uint8_t>& rx) = 0;

    virtual const std::string& GetDeviceName() const = 0;
    virtual uint32_t GetCurrentSpeed() const = 0;
    virtual bool IsRunning() const = 0;

protected:
    std::function<void(const std::vector<uint8_t>&)> _rxCompletedHandler;
};

#endif //_ISERIAL_H