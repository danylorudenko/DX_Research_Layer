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

    DXRL::Size GetSize() const { return SIZE; }

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
        , storageSize_{ reservedSize }
    {
        if (storageSize_ > 0) {
            storage_ = reinterpret_cast<T*>(allocator->AllocArray(storageSize_, sizeof(T), alignof(T)));
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

    void Clear()
    {
        for (Size i = 0; i < size_; ++i) {
            (storage_ + i)->~T();
        }

        size_ = 0;
    }

    void Reset()
    {
        Clear();
        allocator_->FreeArray(reinterpret_cast<Memory::VoidPtr>(storage_));
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

        allocator_->FreeArray(reinterpret_cast<Memory::VoidPtr>(storage_));
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
class InplaceArray 
    : private Array<T, INPLACE_SIZE>
    , private DynamicArray<T, TAllocator>
{
public:
    InplaceArray(TAllocator* allocator, Size size)
        : Array<T, INPLACE_SIZE>{}
        , DynamicArray<T, TAllocator>{ allocator, 0 }
        , inplaceOverflow_{ false }
    {
        //if (size_ > INPLACE_SIZE) {
        //    inplaceOverflow_ = true;
        //    overflowData_ = allocator_->AllocArray<T>(size_);
        //}
    }

    InplaceArray(InplaceArray<T, INPLACE_SIZE, TAllocator> const&) = delete;
    InplaceArray(InplaceArray<T, INPLACE_SIZE, TAllocator>&&) = delete;

    InplaceArray<T, INPLACE_SIZE, TAllocator>& operator=(InplaceArray<T, INPLACE_SIZE, TAllocator> const& rhs) = delete;
    InplaceArray<T, INPLACE_SIZE, TAllocator>& operator=(InplaceArray<T, INPLACE_SIZE, TAllocator>&& rhs) = delete;

    void Clear()
    {
        
    }

    void Reset()
    {
        
    }

    Size GetSize() const
    {
        return 0;
    }

protected:
    bool inplaceOverflow_;

};


} // namespace DXRL