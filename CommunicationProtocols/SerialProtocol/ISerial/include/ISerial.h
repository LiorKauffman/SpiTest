#ifndef _ISERIAL_H
#define _ISERIAL_H

#include <string>
#include <vector>
#include <functional>
#include <functional>
#include <atomic>
#include <mutex>
#include <thread>
#include <string>
#include "unistd.h"
#include "DataStruct/Include/Buffer/CyclicBuffer.h"

class ISerial {
public:
    virtual ~ISerial() = default;

    virtual void SetProperties(const std::string& deviceName,
                                uint32_t baudRateOrSpeed,
                                uint8_t mode,
                                uint8_t bits,
                                std::function<void(const std::vector<uint8_t>&)> rxCompletedHandler);
    
    virtual bool Transmit(const std::vector<uint8_t>& tx);
    virtual bool Receive(std::vector<uint8_t>& rx, size_t length);
    // virtual bool Transfer(const std::vector<uint8_t>& tx, std::vector<uint8_t>& rx) = 0;

    inline const std::string& GetDeviceName() { return _deviceName; }
    inline uint32_t GetCurrentSpeed() { return _currentSpeed; }
    inline bool IsRunning() { return _isRunning; }
    inline auto SetRxCompletedHandler(std::function<void(const std::vector<uint8_t>&)> rxCompletedHandler)
    {
        _rxCompletedHandler = rxCompletedHandler;
    }

protected:
    static constexpr auto _BUFFER_SIZE = 1024;
    static constexpr auto _START_BARKER = {0XAA, 0XAA};
    static constexpr auto _END_BARKER = {0XFF, 0XFF};

    std::function<void(const std::vector<uint8_t>&)> _rxCompletedHandler;
    std::string _deviceName;
    uint32_t _currentSpeed;
    std::mutex _mutex;
    std::atomic<bool> _isRunning;
    std::thread _receiveThread;
    int _serialInstance;
    bool _isStartBakerFound;
    bool _isEndBarkerFound;
    std::vector<uint8_t> _cmdToParse;
    datastruct::buffer::CyclicBuffer<std::vector<uint8_t>, _BUFFER_SIZE> _rxCyclicBuffer;

private:
    virtual void _ReceiveTask() = 0;
};

#endif //_ISERIAL_H