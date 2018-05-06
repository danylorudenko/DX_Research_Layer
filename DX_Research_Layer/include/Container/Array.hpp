#pragma once

#include <Foundation\Types.hpp>

namespace DXRL

{

////////////////////////////////////////
template<typename T, Size SIZE>
class Array
{
public:

    Array(Array<T, SIZE> const& rhs)
    {
        operator=(rhs);
    }

    Array(Array<T, SIZE>&& rhs)
    {
        operator=(std::move(rhs));
    }

    Array& operator=(Array<T, SIZE> const& rhs)
    {
        for (Size i = 0; i < SIZE; ++i) {
            rhs[i] = array_[i];
        }

        return *this;
    }

    Array& operator=(Array<T, SIZE>&& rhs)
    {
        for (Size i = 0; i < SIZE; ++i) {
            rhs[i] = std::move(array_[i]);
        }

        return *this;
    }

    DXRL::Size ArraySize() const { return SIZE; }

    T const& operator[](Size i) const { return array_[i]; }
    T& operator[](Size i) { return array_[i]; }

protected:
    T array_[SIZE];
};



////////////////////////////////////////
template<typename T, typename TAllocator>
class DynamicArray
{
public:
    DynamicArray(TAllocator* allocator, Size reservedSize = 0)
        : data_{ nullptr }
        , size_{ 0 }
        , elementsStorageSize_{ reservedSize }
    {
        if (elementsStorageSize_ > 0) {
            data_ = allocator->AllocArray(elementsStorageSize_, sizeof(T), alignof(T));
        }
    }

    DynamicArray(DynamicArray<T, TAllocator> const&) = delete;
    DynamicArray(DynamicArray<T, TAllocator>&&) = delete;

    DynamicArray<T, TAllocator>& operator=(DynamicArray<T, TAllocator> const&) = delete;
    DynamicArray<T, TAllocator>& operator=(DynamicArray<T, TAllocator>&&) = delete;

    template<typename TArg>
    void PushBack(TArg&& arg)
    {
        if((size_ + 1) > elementsStorageSize_)
    }

    Size GetSize() const
    {
        return size_;
    }


private:
    void ExpandStorage(Size newSize)
    {

    }


protected:
    TAllocator* allocator_;
    T* data_;
    Size size_;
    Size elementsStorageSize_;
};


////////////////////////////////////////
template<typename T, Size INPLACE_SIZE, typename TAllocator>
class InplaceArray : private Array<T, INPLACE_SIZE>
{
public:
    InplaceArray(TAllocator* allocator, Size size)
        : allocator_{ allocator }
        , inplaceOverflow_{ false }
        , size_{ size }
    {
        if (size_ > INPLACE_SIZE) {
            inplaceOverflow_ = true;
            overflowData_ = allocator_->AllocArray<T>(size_);
        }
    }

    InplaceArray(InplaceArray<T, INPLACE_SIZE, TAllocator> const&) = delete;
    InplaceArray(InplaceArray<T, INPLACE_SIZE, TAllocator>&&) = delete;

    InplaceArray<T, INPLACE_SIZE, TAllocator>& operator=(InplaceArray<T, INPLACE_SIZE, TAllocator> const& rhs) = delete;
    InplaceArray<T, INPLACE_SIZE, TAllocator>& operator=(InplaceArray<T, INPLACE_SIZE, TAllocator>&& rhs) = delete;

    void Reset()
    {
        if (inplaceOverflow_) {
            allocator_->FreeArray(overflowData_);
        }
        else {
            for (Size i = 0; i < size_; ++i) {
                (array_ + i)->~T();
            }
        }

        size_ = 0;
    }

    Size GetSize() const
    {
        return size_;
    }

    Size constexpr GetInplaceSize() { return INPLACE_SIZE; }

protected:
    TAllocator* allocator_;
    Size size_;
    T* overflowData_;
    bool inplaceOverflow_;

};


} // namespace DXRL