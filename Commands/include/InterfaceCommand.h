#ifndef _INTERFACE_COMMAND_H
#define _INTERFACE_COMMAND_H

#include <stdint.h>
#include <stdio.h>

class InterfaceCommand
{
public:
    virtual ~InterfaceCommand() = default;
    /**
     * Execute this command handler with raw payload data.
     * @param data pointer to payload bytes
     * @param length number of payload bytes
     */
    virtual void execute(const uint8_t *data, uint32_t length) = 0;
};

#endif //_INTERFACE_COMMAND_H