#include "IBuffer.h"

using namespace datastruct::buffer;

template<typename ObjectType>
IBuffer<ObjectType>::IBuffer(ObjectType* address) :
    _writePos(address),
    _readPos(address)
{
}

template<typename ObjectType>
void IBuffer<ObjectType>::Read(ObjectType* address, uint32_t length)
{
    Peek(address, length);
    Skip(length);
}

template<typename ObjectType>
void IBuffer<ObjectType>::Write(ObjectType* address, uint32_t length)
{
    if (_NumberOfWritableElements() >= length && address != nullptr)
    {
        _Write(address, length);
    }
    else
    {
        printf("Write Overflow\n");
    }
}

template<typename ObjectType>
void IBuffer<ObjectType>::Clear()
{
    _Clear();
}

template<typename ObjectType>
void IBuffer<ObjectType>::Skip(uint32_t size)
{
    if (_NumberOfUnReadElements() >= size)
    {
        _Skip(size);
    }
    else
    {
        printf("Skip Overflow\n");
    }
}

template<typename ObjectType>
void IBuffer<ObjectType>::Peek(ObjectType* address, uint32_t length)
{
    if (_NumberOfUnReadElements() >= length && address != nullptr)
     {
        _Peek(address, length);
     }
     else
     {
        printf("Peek Overflow\n");
     }
}