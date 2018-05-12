#pragma once

#include <Foundation\Types.hpp>
#include <Memory\Pointer.hpp>

#include <type_traits>

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

    Size constexpr GetSize() const { return SIZE; }

    T const& operator[](Size i) const { return array_[i]; }
    T& operator[](Size i) { return array_[i]; }

private:
    T array_[SIZE];
};


////////////////////////////////////////
template<typename T, Size STORAGE_SIZE>
class StaticArrayStorage
{
    StaticArrayStorage(StaticArrayStorage<T, STORAGE_SIZE> const&) = delete;
    StaticArrayStorage(StaticArrayStorage<T, STORAGE_SIZE>&&) = delete;

    StaticArrayStorage<T, SIZE>& operator=(StaticArrayStorage<T, STORAGE_SIZE> const&) = delete;
    StaticArrayStorage<T, SIZE>& operator=(StaticArrayStorage<T, STORAGE_SIZE>&&) = delete;

    Size GetSize() const { return size_; }
    Size constexpr GetStorageSize() const { return STORAGE_SIZE; }


    T const& operator[](Size i) const 
    { 
        assert(i < size_);
        return *TypePtr(i); 
    }

    T& operator[](Size i)
    {
        assert(i < size_);
        return *TypePtr(i);
    }

    template<typename... TArgs>
    void EmplaceBack(TArgs&&... args)
    {
        assert(size_ + 1 < STORAGE_SIZE);
        new (TypePtr(size_++)) T{ args... };
    }

    void PopBack()
    {
        assert(size_ > 0);
        (TypePtr(--size_))->~T();
    }

    void Clear()
    {
        for (Size i = 0; i < size_; ++i) {
            TypePtr(i)->~T();
        }
        size_ = 0;
    }

private:
    T* TypePtr(Size i) { return reinterpret_cast<T*>(array_ + i); }
    T const* TypePtr(Size i) const { return reinterpret_cast<T*>(array_ + i); }

private:
    std::aligned_storage<sizeof(T), alignof(T)> array_[STORAGE_SIZE];
    Size size_;

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

    T& operator[](Size i)
    {
        assert(i < size_ && "Out-of-bounds access to DynamicArray");
        return storage_[i];
    }

    T const& operator[](Size i) const
    {
        assert(i < size_ && "Out-of-bounds access to DynamicArray");
        return storage_[i];
    }

    template<typename... TArgs>
    void EmplaceBack(TArgs&&... args)
    {
        if (size_ == storageSize_) {
            ExpandStorage(storageSize_ != 0 ? size_ * 2 : 10);
        }

        T* newElementAddress = (storage_ + size_++);
        new (newElementAddress) T{ args... };
    }

    void PopBack()
    {
        (storage_ + --size_)->~T();
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
        storageSize_ = 0;
    }

    Size GetSize() const
    {
        return size_;
    }

    Size GetStorageSize() const
    {
        return storageSize_;
    }

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

    TAllocator& Allocator()
    {
        return allocator_;
    }
    
    ~DynamicArray()
    {
        Reset();
    }

private:
    TAllocator* allocator_;
    T* storage_;
    Size size_;
    Size storageSize_; // in elements, not bytes
};


////////////////////////////////////////
template<typename T, Size INPLACE_SIZE, typename TAllocator>
class InplaceDynamicArray 
    : private Array<T, INPLACE_SIZE>
    , private DynamicArray<T, TAllocator>
{
public:
    InplaceDynamicArray(TAllocator* allocator, Size size = 0)
        : Array<T, INPLACE_SIZE>{}
        , DynamicArray<T, TAllocator>{ allocator, 0 }
        , inplaceOverflow_{ false }
    {
        if (size > INPLACE_ARRAY) {
            inplaceOverflow_ = true;
            DynamicArray<T, TAllocator>::ExpandStorage(10);
        }
    }

    InplaceDynamicArray(InplaceDynamicArray<T, INPLACE_SIZE, TAllocator> const&) = delete;
    InplaceDynamicArray(InplaceDynamicArray<T, INPLACE_SIZE, TAllocator>&&) = delete;

    InplaceDynamicArray<T, INPLACE_SIZE, TAllocator>& operator=(InplaceDynamicArray<T, INPLACE_SIZE, TAllocator> const& rhs) = delete;
    InplaceDynamicArray<T, INPLACE_SIZE, TAllocator>& operator=(InplaceDynamicArray<T, INPLACE_SIZE, TAllocator>&& rhs) = delete;

    void Clear()
    {
        size_ = 0;
        if (inplaceOverflow_) {
            DynamicArray<T, TAllocator>::Clear();
        }
    }

    void Reset()
    {
        
    }

    Size GetSize() const
    {
        return size_;
    }

protected:
    bool inplaceOverflow_;

};


} // namespace DXRL