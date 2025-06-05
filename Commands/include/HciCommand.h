#ifndef _HCI_COMMAND_H
#define _HCI_COMMAND_H

#include "InterfaceCommand.h"
#include <memory>
#include <algorithm>

class HciCommand : public InterfaceCommand
{
public:
    void execute(const uint8_t* data, uint32_t length) override;
    HciCommand() = default;

private:
};

#endif //_HCI_COMMAND_H