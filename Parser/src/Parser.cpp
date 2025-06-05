#include "Parser/include/Parser.h"
#include <algorithm>
#include <stdio.h>

void Parser::SetCommandsType(const std::vector<uint8_t> &list)
{
    for (const auto &elementInList : list)
    {
        auto it = std::find(_commandsType.begin(), _commandsType.end(), elementInList);

        if (it == _commandsType.end())
        {
            _commandsType.push_back(elementInList);
        }
        else
        {
            printf("Command Type: 0x%x is already exist! \n", elementInList);
        }
    }
}

void Parser::SetCommandsType(std::initializer_list<uint8_t> list)
{
    SetCommandsType(std::vector<uint8_t>(list));

    for (const auto& t : _handlers)
    {
        printf("cmd-type: 0x%x\n", t.first);
    }


}

bool Parser::RegisterHandler(uint8_t cmd, std::unique_ptr<InterfaceCommand> handler)
{
    auto returnValue = true;

    if (std::find(_commandsType.begin(), _commandsType.end(), cmd) == _commandsType.end())
    {
        returnValue = false;
    }
    else
    {
        _handlers[cmd].push_back(std::move(handler));
    }

    return returnValue;
}

void Parser::ParseCmd(const uint8_t *data, size_t length)
{
    uint8_t cmdType = data[0];
    
    printf("cmd type = 0x%x\n", cmdType);
    auto it = _handlers.find(cmdType);
    if (it != _handlers.end())
    {
        printf("Handler found!\n");

        for (auto &handle : it->second)
        {
            handle->execute(data + 1, length - 1);
        }
    }
    else
    {
        printf("Handler not found\n");
    }
}