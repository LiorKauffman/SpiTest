#ifndef _CYCLIC_BIT_BUFFER_H
#define _CYCLIC_BIT_BUFFER_H

#include "ICyclicBuffer.h"
#include <cmath>

namespace datastruct::buffer
{
    template<typename ObjectType, uint32_t BufferSize>
    class CyclicBitBuffer : public ICyclicBuffer<ObjectType, (uint32_t)std::ceil(BufferSize / ((double)sizeof(ObjectType) * 8))>
    {
    public:

        static constexpr uint32_t _NUMBER_OF_FULL_OBJECT_TYPE_FULL_ELEMENTS = (uint32_t)std::ceil(BufferSize / ((double)sizeof(ObjectType) * 8));

        class CyclicBitIterator : public ICyclicBuffer<ObjectType, _NUMBER_OF_FULL_OBJECT_TYPE_FULL_ELEMENTS>::CyclicIterator
        {
        public:
            // Ctor
            CyclicBitIterator(ObjectType* element, CyclicBitBuffer& cyclicBitBuffer, uint32_t& posOffset, uint32_t endOfBufferOffset);

            // Operators
            CyclicBitIterator& operator++();
            CyclicBitIterator operator++(int);
            CyclicBitIterator operator+(uint32_t size);
            CyclicBitIterator& operator=(const CyclicBitIterator& other);

            operator ObjectType*() const { return this->_element; }

            inline auto& GetOffset() { return _posOffset; }
            inline auto SetOffset(uint32_t offset) { _posOffset = offset; }
            inline auto LoadParam() { _posOffset = _posOffsetToSave; _element = _objectToSave; }
            inline auto SaveParam() {_posOffsetToSave = _posOffset; _objectToSave = _element;}

        private:
            // Members
            using ICyclicBuffer<ObjectType, _NUMBER_OF_FULL_OBJECT_TYPE_FULL_ELEMENTS>::CyclicIterator::_element;
            using ICyclicBuffer<ObjectType, _NUMBER_OF_FULL_OBJECT_TYPE_FULL_ELEMENTS>::CyclicIterator::_cyclicBuffer;


            uint32_t& _posOffset;
            uint32_t _endOfBufferOffset;

            ObjectType* _objectToSave;
            uint32_t _posOffsetToSave;
        };

        friend class CyclicBitIterator;

        // Ctor
        CyclicBitBuffer() : _readPosOffset(0), _writePosOffset(0), _endOfBufferOffset(BufferSize % _OBJECT_BITS_SIZE) {};

        // Public Methods
        inline uint32_t NumberOfUnReadFullTypeSize() { return ICyclicBuffer<ObjectType, _NUMBER_OF_FULL_OBJECT_TYPE_FULL_ELEMENTS>::_NumberOfUnReadElements(); }
        inline auto GetView() { return *this; }
        void WriteType(ObjectType* address, uint32_t typeSize);
        void ReadType(ObjectType* address, uint32_t typeSize);

        inline CyclicBitIterator begin() { return CyclicBitIterator(_readPos, *this, _readPosOffset, _endOfBufferOffset); }
        inline CyclicBitIterator end() { return CyclicBitIterator(_writePos, *this, _writePosOffset, _endOfBufferOffset); }

    private:
        //Const Member
        static constexpr auto _OBJECT_BITS_SIZE = sizeof(ObjectType) * 8;
        
        // Inherited Members
        using ICyclicBuffer<ObjectType, _NUMBER_OF_FULL_OBJECT_TYPE_FULL_ELEMENTS>::_isFull;
        using ICyclicBuffer<ObjectType, _NUMBER_OF_FULL_OBJECT_TYPE_FULL_ELEMENTS>::_writePos;
        using ICyclicBuffer<ObjectType, _NUMBER_OF_FULL_OBJECT_TYPE_FULL_ELEMENTS>::_readPos;
        using ICyclicBuffer<ObjectType, _NUMBER_OF_FULL_OBJECT_TYPE_FULL_ELEMENTS>::_buffer;

        uint32_t _readPosOffset;
        uint32_t _writePosOffset;
        uint32_t _endOfBufferOffset;

        // Private Methods
        void _Write(ObjectType* address, uint32_t length) override;
        void _Peek(ObjectType* address, uint32_t length) override;
        void _Skip(uint32_t size) override;
        void _WriteEmpty(uint32_t size) override;
        void _Clear() override;
        inline auto _NumberOfWritableElementsInTypeSize() { return (BufferSize - ICyclicBuffer<ObjectType, _NUMBER_OF_FULL_OBJECT_TYPE_FULL_ELEMENTS>::_NumberOfUnReadElements()) / _OBJECT_BITS_SIZE; }
        uint32_t _NumberOfUnReadElements() override;
        inline uint32_t _NumberOfWritableElements() override { return BufferSize - _NumberOfUnReadElements(); }
    };
}

#include "CyclicBitBuffer.hpp"
#endif