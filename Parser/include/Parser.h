#ifndef _PARSER_H
#define _PARSER_H

// #include "../../HelloCommand/include/HelloCommand.h"
#include "../../Singleton/include/Singleton.h"
#include "../../Commands/include/InterfaceCommand.h"
#include <stdint.h>
#include <vector>
#include <functional>
#include <unordered_map>
#include <memory>

class Parser : public Singleton<Parser>
{
    friend class Singleton<Parser>;

public:
    using Handler = std::function<void(const uint8_t *, size_t)>;

    void SetCommandsType(std::initializer_list<uint8_t> list);
    void SetCommandsType(const std::vector<uint8_t> &list);
    bool RegisterHandler(uint8_t cmd, std::unique_ptr<InterfaceCommand> handler);
    void ParseCmd(const uint8_t *data, size_t length);
    inline auto UnregisterHandler(uint8_t cmd) { return _handlers.erase(cmd) > 0; }
    inline auto ClearHandlers() { _handlers.clear(); }
    inline const auto &GetStartBarker() { return _START_BARKER; }
    inline const auto &GetEndBarker() { return _END_BARKER; }

    enum class CmdType : uint8_t
    {
        CMD_HELLO = 0X01,
        CMD_SENSOR_DATA,
        CMD_CONTROL,
        CMD_ACK,
        CMD_HCI,
        CMD_ERROR,
        CMD_POWEROFF,
        CMD_POWERON
    };

private:
    static constexpr auto _START_BARKER = {0XAA, 0XAA};
    static constexpr auto _END_BARKER = {0XFF, 0XFF};

    Parser() = default;
    ~Parser() = default;

    inline auto _ContainsCommand(uint8_t cmdCode) const
    {
        return std::find(_commandsType.begin(), _commandsType.end(), cmdCode) != _commandsType.end();
    }

    std::vector<uint8_t> _commandsType;
    // std::unordered_map<uint8_t, std::vector<Handler>> _handlers;
    std::unordered_map<uint8_t, std::vector<std::unique_ptr<InterfaceCommand>>> _handlers;
};

#endif // _PARSER_H