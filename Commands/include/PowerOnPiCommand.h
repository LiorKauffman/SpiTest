#ifndef _POWERON_PI_COMMAND_H
#define _POWERON_PI_COMMAND_H

#include "InterfaceCommand.h"
#include <memory>
#include <algorithm>
// #include "../../jenframework/framework/os/include/os_errors.hpp"

// class Controller;
class PowerOnPiCommand : public InterfaceCommand
{
public:
    void execute(const uint8_t* data, uint32_t length) override;
    PowerOnPiCommand() = default;
    // Controller *controller;

private:
};

#endif //_POWERON_PI_COMMAND_H