#include "CyclicBitBuffer.h"

using namespace datastruct::buffer;

template<typename ObjectType, uint32_t BufferSize>
CyclicBitBuffer<ObjectType, BufferSize>::CyclicBitIterator::CyclicBitIterator(ObjectType* pElement, CyclicBitBuffer& cyclicBitBuffer, uint32_t& posOffset, uint32_t endOfBufferOffset) :
    ICyclicBuffer<ObjectType, _NUMBER_OF_FULL_OBJECT_TYPE_FULL_ELEMENTS>::CyclicIterator(pElement, cyclicBitBuffer),
    _posOffset(posOffset),
    _endOfBufferOffset(endOfBufferOffset)
{
}

template<typename ObjectType, uint32_t BufferSize>
CyclicBitBuffer<ObjectType, BufferSize>::CyclicBitIterator& CyclicBitBuffer<ObjectType, BufferSize>::CyclicBitIterator::operator++()
{
    auto bufferAddress = &ICyclicBuffer<ObjectType, _NUMBER_OF_FULL_OBJECT_TYPE_FULL_ELEMENTS>::CyclicIterator::_GetBuffer()[ICyclicBuffer<ObjectType, _NUMBER_OF_FULL_OBJECT_TYPE_FULL_ELEMENTS>::CyclicIterator::_GetBuffer().size()- 1];
    if (_posOffset == _endOfBufferOffset - 1 && bufferAddress == _element)
    {
        _posOffset = 0;
        _element = ICyclicBuffer<ObjectType, _NUMBER_OF_FULL_OBJECT_TYPE_FULL_ELEMENTS>::CyclicIterator::_GetBuffer().data();
    }
    else if (_posOffset == _OBJECT_BITS_SIZE - 1)
    {
        _posOffset = 0;
        (dynamic_cast<ICyclicBuffer<ObjectType, _NUMBER_OF_FULL_OBJECT_TYPE_FULL_ELEMENTS>::CyclicIterator*>(this))->operator++();
    }
    else
    {
        _posOffset++;
    }

    return *this;
}

template<typename ObjectType, uint32_t CyclicBitBufferSize>
CyclicBitBuffer<ObjectType, CyclicBitBufferSize>::CyclicBitIterator CyclicBitBuffer<ObjectType, CyclicBitBufferSize>::CyclicBitIterator::operator++(int)
{
    CyclicBitIterator temp = *this;
    ++(*this);

    return temp;
}

template<typename ObjectType, uint32_t CyclicBitBufferSize>
CyclicBitBuffer<ObjectType, CyclicBitBufferSize>::CyclicBitIterator CyclicBitBuffer<ObjectType, CyclicBitBufferSize>::CyclicBitIterator::operator+(uint32_t size)
{    
    auto it = *this;
    for(uint32_t index = 0; index < size; index++, it++);

    return it;
}

template<typename ObjectType, uint32_t CyclicBitBufferSize>
CyclicBitBuffer<ObjectType, CyclicBitBufferSize>::CyclicBitIterator& CyclicBitBuffer<ObjectType, CyclicBitBufferSize>::CyclicBitIterator::operator=(const CyclicBitIterator& other)
{
     (dynamic_cast<ICyclicBuffer<ObjectType, _NUMBER_OF_FULL_OBJECT_TYPE_FULL_ELEMENTS>::CyclicBitIterator*>(this))->operator=(other);
    _posOffset = other._posOffset;

    return *this;
}

template<typename ObjectType, uint32_t BufferSize>
void CyclicBitBuffer<ObjectType, BufferSize>::_Write(ObjectType* address, uint32_t length)
{
    auto it = end();
    uint32_t index = 0;

    while (length)
    {
        const uint32_t numberOfBitsToShift = length >= _OBJECT_BITS_SIZE ? _OBJECT_BITS_SIZE : length;

        for (uint32_t i = 0; i < numberOfBitsToShift; ++i)
        {
            uint32_t bitMask = 1;
            bitMask = bitMask << i;
            auto temp = ( address[index] & bitMask) >> i;
            *it |= temp << it.GetOffset();
            ++it;
        } 

        length -= length >= _OBJECT_BITS_SIZE ? _OBJECT_BITS_SIZE : length;
        ++index;
    }

    _writePos = &(*it);
    _isFull = _writePos == _readPos && _writePosOffset == _readPosOffset;
}

template<typename ObjectType, uint32_t CyclicBitBufferSize>
void CyclicBitBuffer<ObjectType, CyclicBitBufferSize>::WriteType(ObjectType* address, uint32_t typeSize)
{
    if (typeSize <= _NumberOfWritableElementsInTypeSize())
    {
        _Write(address, typeSize * _OBJECT_BITS_SIZE);
    }
    else
    {
        printf("Write In Type Size Overflow\n");
    }
}

template<typename ObjectType, uint32_t CyclicBitBufferSize>
void CyclicBitBuffer<ObjectType, CyclicBitBufferSize>::_WriteEmpty(uint32_t size)
{
    _Write(&(*end()), size);
}

template<typename ObjectType, uint32_t CyclicBitBufferSize>
void CyclicBitBuffer<ObjectType, CyclicBitBufferSize>::ReadType(ObjectType* address, uint32_t typeSize)
{
    IBuffer<ObjectType>::Peek(address, typeSize * _OBJECT_BITS_SIZE);
    IBuffer<ObjectType>::Skip(typeSize * _OBJECT_BITS_SIZE);

}

template<typename ObjectType, uint32_t CyclicBitBufferSize>
void CyclicBitBuffer<ObjectType, CyclicBitBufferSize>::_Peek(ObjectType* address, uint32_t length)
{
    auto it = begin();
    it.SaveParam();
    uint32_t index = 0;

    while (length)
    {
        const uint32_t numberOfBitsToShift = length >= _OBJECT_BITS_SIZE ? _OBJECT_BITS_SIZE : length;

        for (uint32_t i = 0; i < numberOfBitsToShift; ++i)
        {
            auto bitMask = 1;
            bitMask = bitMask << it.GetOffset();
            address[index] |= ((*it & bitMask) >> it.GetOffset()) << i;
            ++it;
        } 

        length -= length >= _OBJECT_BITS_SIZE ? _OBJECT_BITS_SIZE : length;
        ++index;
    }

    it.LoadParam();
}

template<typename ObjectType, uint32_t CyclicBitBufferSize>
void CyclicBitBuffer<ObjectType, CyclicBitBufferSize>::_Skip(uint32_t size)
{
    auto temp = begin() + size;
    _readPos = &(*temp);
    _isFull = false;
}

template<typename ObjectType, uint32_t CyclicBitBufferSize>
uint32_t CyclicBitBuffer<ObjectType, CyclicBitBufferSize>::_NumberOfUnReadElements()
{
    uint32_t numberOfUnReadElements;

    if (_isFull) 
    {
        numberOfUnReadElements = CyclicBitBufferSize;
    }
    else if (_readPos ==_writePos)
    {
        int32_t posOffsetDif = _writePosOffset - _readPosOffset;
        
        if (posOffsetDif < 0) 
        {
            numberOfUnReadElements = CyclicBitBufferSize + posOffsetDif;
        }
        else
        {
            numberOfUnReadElements = posOffsetDif;
        }
    }
    else 
    {
        auto diff = ICyclicBuffer<ObjectType, _NUMBER_OF_FULL_OBJECT_TYPE_FULL_ELEMENTS>::_NumberOfUnReadElements() * _OBJECT_BITS_SIZE;
        diff += _writePosOffset;
        diff -= _readPosOffset;
        
        if (_writePos < _readPos)
        {
            diff -=  _OBJECT_BITS_SIZE - _endOfBufferOffset;
        }

        numberOfUnReadElements = diff;
    }

    return numberOfUnReadElements;
}

template<typename ObjectType, uint32_t CyclicBitBufferSize>
void CyclicBitBuffer<ObjectType, CyclicBitBufferSize>::_Clear()
{
    _readPosOffset = 0;
    _writePosOffset = 0;

    ICyclicBuffer<ObjectType, _NUMBER_OF_FULL_OBJECT_TYPE_FULL_ELEMENTS>::_Clear();
}


