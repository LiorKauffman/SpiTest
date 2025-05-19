#include "CyclicBuffer.h"

using namespace datastruct::buffer;

template<typename ObjectType, uint32_t BufferSize>
void CyclicBuffer<ObjectType, BufferSize>::_Write(ObjectType* address, uint32_t length)
{
    _writePos = std::copy(address, 
                          address + length, 
                          CyclicBuffer<ObjectType, BufferSize>::end());
    _isFull = _writePos == _readPos;
}

template<typename ObjectType, uint32_t BufferSize>
void CyclicBuffer<ObjectType, BufferSize>::_Skip(uint32_t size)
{
    _readPos =  ICyclicBuffer<ObjectType, BufferSize>::begin() + size;
    _isFull = false;
}


template<typename ObjectType, uint32_t BufferSize>
void CyclicBuffer<ObjectType, BufferSize>::_WriteEmpty(uint32_t size)
{
    _Write(&(*ICyclicBuffer<ObjectType, BufferSize>::end()), size);
}

template<typename ObjectType, uint32_t BufferSize>
void CyclicBuffer<ObjectType, BufferSize>::_Peek(ObjectType* address, uint32_t length)
{
    auto it = ICyclicBuffer<ObjectType, BufferSize>::begin();
    auto x = it + (length - 1);
    std::copy(it, x, address);
    address[length - 1] = *(it + (length - 1));
}