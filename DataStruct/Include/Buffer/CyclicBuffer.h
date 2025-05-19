#ifndef _CYCLIC_BUFFER_H
#define _CYCLIC_BUFFER_H

#include "ICyclicBuffer.h"
#include "array"
#include <iterator>
#include <cstddef>

namespace datastruct::buffer
{
    template<typename ObjectType, uint32_t BufferSize>
    class CyclicBuffer : public ICyclicBuffer<ObjectType, BufferSize>
    {
    public:
        // Ctor
        CyclicBuffer() = default;

        // Public Method
        inline auto GetView() { return *this; }

    private:
        // Inherited Members
        using ICyclicBuffer<ObjectType, BufferSize>::_isFull;
        using ICyclicBuffer<ObjectType, BufferSize>::_writePos;
        using ICyclicBuffer<ObjectType, BufferSize>::_readPos;

        // Private Methods
        void _Write(ObjectType* address, uint32_t length) override;
        void _Peek(ObjectType* address, uint32_t length) override;
        void _WriteEmpty(uint32_t size) override;
        void _Skip(uint32_t size) override;
        inline uint32_t _NumberOfWritableElements() override { return BufferSize - ICyclicBuffer<ObjectType, BufferSize>::_NumberOfUnReadElements(); }
    };
}

#include "CyclicBuffer.hpp"
#endif