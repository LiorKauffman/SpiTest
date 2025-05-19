#include "ICyclicBuffer.h"

using namespace datastruct::buffer;

template<typename ObjectType, uint32_t BufferSize>
ICyclicBuffer<ObjectType, BufferSize>::CyclicIterator::CyclicIterator(ObjectType* pElement, ICyclicBuffer& cyclicBuffer) :
    _element(pElement),
    _cyclicBuffer(cyclicBuffer)
{
}

template<typename ObjectType, uint32_t BufferSize>
typename ICyclicBuffer<ObjectType, BufferSize>::CyclicIterator& ICyclicBuffer<ObjectType, BufferSize>::CyclicIterator::operator++()
{
    uint32_t lastBufferIndex = BufferSize - 1;
    uint32_t currentElementIndex = (_element - _cyclicBuffer._buffer.data());
    _element =  lastBufferIndex == currentElementIndex ? 
                 _cyclicBuffer._buffer.data() : _element + 1;

    return *this;
}

template<typename ObjectType, uint32_t BufferSize>
typename ICyclicBuffer<ObjectType, BufferSize>::CyclicIterator ICyclicBuffer<ObjectType, BufferSize>::CyclicIterator::operator++(int)
{
    CyclicIterator temp = *this;
    ++(*this);
    return temp;
} 

template<typename ObjectType, uint32_t BufferSize>
typename ICyclicBuffer<ObjectType, BufferSize>::CyclicIterator ICyclicBuffer<ObjectType, BufferSize>::CyclicIterator::operator+(uint32_t size)
{
    auto it = *this;
    for(auto index = 0; index < size; index++, it++);

    return it;
}

template<typename ObjectType, uint32_t BufferSize>
typename ICyclicBuffer<ObjectType, BufferSize>::CyclicIterator& ICyclicBuffer<ObjectType, BufferSize>::CyclicIterator::operator=(const ICyclicBuffer<ObjectType, BufferSize>::CyclicIterator& other)
{
    _element = other._element;
    _cyclicBuffer = other._cyclicBuffer;

    return *this;
}

template<typename ObjectType, uint32_t BufferSize>
ICyclicBuffer<ObjectType, BufferSize>::ICyclicBuffer() : 
    IBuffer<ObjectType>(nullptr),
    _buffer(),
    _isFull(false)
{
    _Clear();
}

template<typename ObjectType, uint32_t BufferSize>
void ICyclicBuffer<ObjectType, BufferSize>::WriteDiscard(ObjectType* address, uint32_t length)
{
    if (length > _NumberOfWritableElements())
    {
        _Skip(length - _NumberOfWritableElements());
    }

    Write(address, length);
}

template<typename ObjectType, uint32_t BufferSize>
void ICyclicBuffer<ObjectType, BufferSize>::WriteEmpty(uint32_t size)
{
    if (_NumberOfWritableElements() >= size)
    {
        _WriteEmpty(size);
    }
    else
    {
        printf("Write Overflow\n");
    }
}

template<typename ObjectType, uint32_t BufferSize>
void ICyclicBuffer<ObjectType, BufferSize>::_Clear()
{
    _writePos = _buffer.data();
    _readPos = _buffer.data();
    _isFull = false;
}

template<typename ObjectType, uint32_t BufferSize>
void ICyclicBuffer<ObjectType, BufferSize>::AlignmentBuffer()
{
    std::array<ObjectType, BufferSize> tempArray;

    auto numberOfUnReadElements = _NumberOfUnReadElements();

    IBuffer<ObjectType>::Read(tempArray.data(), numberOfUnReadElements);
    IBuffer<ObjectType>::Clear();
    IBuffer<ObjectType>::Write(tempArray.data(), numberOfUnReadElements);
}

template<typename ObjectType, uint32_t BufferSize>
uint32_t ICyclicBuffer<ObjectType, BufferSize>::_NumberOfUnReadElements()
{
    return _writePos > _readPos ? _writePos - _readPos :
           _readPos > _writePos || (_readPos == _writePos && _isFull) ?
           BufferSize - (_readPos - _writePos) :  0;
}