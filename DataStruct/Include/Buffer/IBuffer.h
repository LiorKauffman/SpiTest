#ifndef _IBUFFER_H
#define _IBUFFER_H

#include "stdint.h"
#include <iostream>

namespace datastruct::buffer
{
    template<typename ObjectType>
    class IBuffer
    {
    public:
        // Ctor
        IBuffer(ObjectType* address);
        
        // Public Methods
        void Read(ObjectType* address, uint32_t length);
        void Write(ObjectType* address, uint32_t length);
        void Clear();
        void Skip(uint32_t size);
        void Peek (ObjectType* address, uint32_t length);
        inline uint32_t NumberOfUnReadElements() { return _NumberOfUnReadElements(); }

    protected:
        // Members
        ObjectType* _writePos;
        ObjectType* _readPos;

    private:
        // Pure Virtual Method
        virtual void _Write(ObjectType* address, uint32_t length) = 0;
        virtual void _Clear() = 0;
        virtual void _Skip(uint32_t size) = 0;
        virtual void _Peek(ObjectType* address, uint32_t length) = 0;
        virtual uint32_t _NumberOfWritableElements() = 0;
        virtual uint32_t _NumberOfUnReadElements() = 0;
    };
}

#include "IBuffer.hpp"
#endif