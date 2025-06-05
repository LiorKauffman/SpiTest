#ifndef _POWEROFF_PI_COMMAND_H
#define _POWEROFF_PI_COMMAND_H

#include "InterfaceCommand.h"
#include <memory>
#include <algorithm>
// #include "../../jenframework/framework/os/include/os_errors.hpp"

// class Controller;
class PowerOffPiCommand : public InterfaceCommand
{
public:
    void execute(const uint8_t* data, uint32_t length) override;
    PowerOffPiCommand() = default;
    // Controller *controller;

private:
};

#endif //_POWEROFF_PI_COMMAND_H