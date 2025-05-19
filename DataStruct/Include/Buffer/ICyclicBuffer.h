#ifndef _ICYCLIC_BUFFER_H
#define _ICYCLIC_BUFFER_H

#include "IBuffer.h"

namespace datastruct::buffer
{
    template<typename ObjectType, uint32_t BufferSize>
    class ICyclicBuffer : public IBuffer<ObjectType>
    {
    public:
        class CyclicIterator
        {
            // Friends operators
            friend bool operator==(const CyclicIterator& firstIterator, const CyclicIterator& secondIterator) { return firstIterator._element == secondIterator._element; };
            friend bool operator!=(const CyclicIterator& firstIterator, const CyclicIterator& secondIterator) { return firstIterator._element != secondIterator._element; };  

        public:
        using iterator_category = std::forward_iterator_tag;
        using value_type        = ObjectType;
        using difference_type   = std::ptrdiff_t;
        using pointer           = ObjectType*;
        using reference         = ObjectType&;
        
            // Ctor
            CyclicIterator(ObjectType* element, ICyclicBuffer& cyclicBuffer);
            CyclicIterator() = default; 

            // Operators
            inline ObjectType& operator*() { return *_element; }
            inline ObjectType* operator->() { return _element; }
            CyclicIterator& operator++();
            CyclicIterator operator++(int);
            CyclicIterator operator+(uint32_t size);
            CyclicIterator& operator=(const CyclicIterator& other);
            operator ObjectType*() const { return this->_element; }
            
        protected:
            ObjectType* _element;
            ICyclicBuffer& _cyclicBuffer;

            inline auto& _GetBuffer() { return _cyclicBuffer._buffer; }
        };
        
        friend class CyclicIterator;

        // Ctor
        ICyclicBuffer();

        // Public methods
        void WriteDiscard(ObjectType* address, uint32_t length);
        virtual void WriteEmpty(uint32_t size);
        void AlignmentBuffer();
        inline auto IsFull() { return _isFull; }

        inline CyclicIterator begin() { return CyclicIterator(_readPos, *this); }
        inline CyclicIterator end() { return CyclicIterator(_writePos, *this); }

    private:
        // private methods
        virtual void _Write(ObjectType* address, uint32_t length) = 0;
        virtual void _Peek(ObjectType* address, uint32_t length) = 0;
        virtual uint32_t _NumberOfWritableElements() = 0;
        virtual void _WriteEmpty(uint32_t size) = 0;
        virtual void _Skip(uint32_t size) = 0;
    
    protected:
        // Members
        std::array<ObjectType, BufferSize> _buffer;
        bool _isFull;

        // Inherited Members
        using IBuffer<ObjectType>::_writePos;
        using IBuffer<ObjectType>::_readPos;

        // Protected Method
        virtual uint32_t _NumberOfUnReadElements() override;
        virtual void _Clear() override;
    };
}

#include "ICyclicBuffer.hpp"
#endif