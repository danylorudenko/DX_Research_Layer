#pragma once

#include <Foundation\Types.hpp>
#include <Memory\Pointer.hpp>

namespace DXRL

{

////////////////////////////////////////
template<typename T, Size SIZE>
class Array
{
public:
    Array(Array<T, SIZE> const&) = delete;
    Array(Array<T, SIZE>&&) = delete;

    Array<T, SIZE>& operator=(Array<T, SIZE> const&) = delete;
    Array<T, SIZE>& operator=(Array<T, SIZE>&&) = delete;

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
    DynamicArray(TAllocator* allocator, Size reservedSize = 10)
        : storage_{ nullptr }
        , size_{ 0 }
        , elementsStorageSize_{ reservedSize }
    {
        if (elementsStorageSize_ > 0) {
            storage_ = allocator->AllocArray(elementsStorageSize_, sizeof(T), alignof(T));
        }
    }

    DynamicArray(DynamicArray<T, TAllocator> const&) = delete;
    DynamicArray(DynamicArray<T, TAllocator>&&) = delete;

    DynamicArray<T, TAllocator>& operator=(DynamicArray<T, TAllocator> const&) = delete;
    DynamicArray<T, TAllocator>& operator=(DynamicArray<T, TAllocator>&&) = delete;

    T& operator[](Size arg)
    {
        assert(arg < size_ && "Out-of-bounds access to DynamicArray");
        return storage_[arg];
    }

    T const& operator[](Size arg) const
    {
        assert(arg < size_ && "Out-of-bounds access to DynamicArray");
        return storage_[arg];
    }

    template<typename... TArgs>
    void PushBack(TArgs&&... args)
    {
        if (size_ == storageSize_) {
            ExpandStorage(size_ * 2);
        }

        T* newElementAddress = (storage_ + size_++);
        new (newElementAddress) T{ args... };
    }

    T PopBack()
    {
        return T{ std::move(operator[](size_-- - 1)) };
    }

    Size GetSize() const
    {
        return size_;
    }

    ~DynamicArray()
    {
        for (Size i = 0; i < size_; ++i) {
            (storage_ + i)->~T();
        }
        size_ = 0;

        allocator->FreeArray(reinterpret_cast<Memory::VoidPtr>(storage_));
        storageSize_ = 0;
    }


private:
    void ExpandStorage(Size newSize)
    {
        assert((newSize > elementsStorageSize) && "newSize is smaller than current elementStorageSize_");

        T* newStorage = reinterpret_cast<T*>(allocator_->AllocArray(newSize, sizeof(T), alignof(T)));
        std::memcpy(newStorage, storage_, size_ * sizeof(T));
        
        if (elementsStorageSize_ > 0) {
            allocator_->FreeArray(storage_);
        }
        
        storage_ = newStorage;
        storageSize_ = newSize;
    }


protected:
    TAllocator* allocator_;
    T* storage_;
    Size size_;
    Size storageSize_; // in elements, not bytes
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