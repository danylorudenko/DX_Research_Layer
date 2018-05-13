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

    T* Data() const { return array_; }

private:
    T array_[SIZE];
};


////////////////////////////////////////
template<typename T, Size STORAGE_SIZE>
class StaticArrayStorage
{
public:
    StaticArrayStorage()
        : size_{ 0 }
    { }

    StaticArrayStorage(StaticArrayStorage<T, STORAGE_SIZE> const&) = delete;
    StaticArrayStorage(StaticArrayStorage<T, STORAGE_SIZE>&&) = delete;

    StaticArrayStorage<T, STORAGE_SIZE>& operator=(StaticArrayStorage<T, STORAGE_SIZE> const&) = delete;
    StaticArrayStorage<T, STORAGE_SIZE>& operator=(StaticArrayStorage<T, STORAGE_SIZE>&&) = delete;

    Size GetSize() const { return size_; }
    Size constexpr GetStorageSize() const { return STORAGE_SIZE; }
    bool IsFull() const { return size_ == STORAGE_SIZE; }


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

    T* Data() const
    {
        return reinterpret_cast<T*>(&array_);
    }

    template<typename... TArgs>
    void EmplaceBack(TArgs&&... args)
    {
        assert(size_ + 1 <= STORAGE_SIZE);
        new (TypePtr(size_++)) T{ std::forward<TArgs>(args)... };
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

    void _MoveData(T* dest)
    {
        Size const size = GetSize();
        for (Size i = 0; i < size; ++i) {
            std::memcpy((dest + i), TypePtr(i), sizeof(T));
        }
        size_ = 0;
    }

    void _WrapData(T* src, Size count)
    {
        assert(GetSize() == 0);
        assert(STORAGE_SIZE >= count);

        for (Size i = 0; i < count; ++i) {
            std::memcpy(TypePtr(i), src + i, sizeof(T));
        }
        size_ = count;
    }

    void _ResizePure(Size size)
    {
        size_ = size;
    }

private:
    T* TypePtr(Size i) { return reinterpret_cast<T*>(&array_) + i; }
    T const* TypePtr(Size i) const { return reinterpret_cast<T*>(&array_) + i; }

private:
    std::aligned_storage_t<sizeof(T) * STORAGE_SIZE, alignof(T)> array_;
    Size size_;

};


////////////////////////////////////////
template<typename T, typename TAllocator>
class DynamicArray
{
public:
    DynamicArray(TAllocator* allocator, Size reservedSize = 10)
        : allocator_{ allocator }
        , storage_{ nullptr }
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

    T* Data() const
    {
        return storage_;
    }

    template<typename... TArgs>
    void EmplaceBack(TArgs&&... args)
    {
        if (size_ == storageSize_) {
            ExpandStorage(storageSize_ != 0 ? size_ * 2 : 10);
        }

        T* newElementAddress = (storage_ + size_++);
        new (newElementAddress) T{ std::forward<TArgs>(args)... };
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

    TAllocator& Allocator()
    {
        return allocator_;
    }

    void ExpandStorage(Size newSize)
    {
        assert((newSize > storageSize_) && "newSize is smaller than current elementStorageSize_");

        T* newStorage = reinterpret_cast<T*>(allocator_->AllocArray(newSize, sizeof(T), alignof(T)));
        std::memcpy(newStorage, storage_, size_ * sizeof(T));
        
        if (storageSize_ > 0) {
            allocator_->FreeArray(reinterpret_cast<Memory::VoidPtr>(storage_));
        }
        
        storage_ = newStorage;
        storageSize_ = newSize;
    }

    void _MoveData(T* dest)
    {
        Size const size = GetSize();
        std::memcpy(dest, storage_, size * sizeof(T));
        size_ = 0;
    }

    void _WrapData(T* src, Size count)
    {
        assert(GetSize() == 0);
        if (GetStorageSize() < count) {
            ExpandStorage(count + 2);
        }

        std::memcpy(storage_, src, count * sizeof(T));
        size_ = count;
    }

    void _ResizePure(Size size)
    {
        size_ = size;
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
    : private StaticArrayStorage<T, INPLACE_SIZE>
    , private DynamicArray<T, TAllocator>
{
public:
    InplaceDynamicArray(TAllocator* allocator)
        : StaticArrayStorage<T, INPLACE_SIZE>{}
        , DynamicArray<T, TAllocator>{ allocator, 0 }
        , isDynamic_{ false }
    {
    }

    InplaceDynamicArray(InplaceDynamicArray<T, INPLACE_SIZE, TAllocator> const&) = delete;
    InplaceDynamicArray(InplaceDynamicArray<T, INPLACE_SIZE, TAllocator>&&) = delete;

    InplaceDynamicArray<T, INPLACE_SIZE, TAllocator>& operator=(InplaceDynamicArray<T, INPLACE_SIZE, TAllocator> const& rhs) = delete;
    InplaceDynamicArray<T, INPLACE_SIZE, TAllocator>& operator=(InplaceDynamicArray<T, INPLACE_SIZE, TAllocator>&& rhs) = delete;

    template<typename... TArgs>
    void EmplaceBack(TArgs&&... args)
    {
        if (isDynamic_) {
            DynamicArray<T, TAllocator>::EmplaceBack(std::forward<TArgs>(args)...);
        }
        else {
            if (StaticArrayStorage<T, INPLACE_SIZE>::IsFull()) {
                _TransferToDynamic();
                DynamicArray<T, TAllocator>::EmplaceBack(std::forward<TArgs>(args)...);
                return;
            }

            StaticArrayStorage<T, INPLACE_SIZE>::EmplaceBack(std::forward<TArgs>(args)...);
        }
    }

    void PopBack()
    {
        if (isDynamic_) {
            DynamicArray<T, TAllocator>::PopBack();
        }
        else {
            StaticArrayStorage<T, INPLACE_SIZE>::PopBack();
        }
    }


    void Clear()
    {
        if (isDynamic_) {
            DynamicArray<T, TAllocator>::Clear();
            isDynamic_ = false;
        }
        else {
            StaticArrayStorage<T, INPLACE_SIZE>::Clear();
        }
    }

    void Reset()
    {
        DynamicArray<T, TAllocator>::Reset();
        StaticArrayStorage<T, INPLACE_SIZE>::Clear();
    }

    Size GetSize() const
    {
        if (isDynamic_) {
            return DynamicArray<T, TAllocator>::GetSize();
        }
        else {
            StaticArrayStorage<T, INPLACE_SIZE>::GetSize();
        }
    }


private:
    void _TransferToInplace()
    {
        Size const size = DynamicArray<T, TAllocator>::GetSize();
        DynamicArray<T, TAllocator>::_MoveData(StaticArrayStorage<T, INPLACE_SIZE>::Data());
        StaticArrayStorage<T, INPLACE_SIZE>::_ResizePure(size);

        isDynamic_ = false;
    }

    void _TransferToDynamic()
    {
        Size const size = StaticArrayStorage<T, INPLACE_SIZE>::GetSize();
        Size const requiredDynamicSize = size * 2;
        if (DynamicArray<T, TAllocator>::GetStorageSize() < requiredDynamicSize) {
            DynamicArray<T, TAllocator>::ExpandStorage(requiredDynamicSize);
        }

        DynamicArray<T, TAllocator>::_ResizePure(size);
        StaticArrayStorage<T, INPLACE_SIZE>::_MoveData(DynamicArray<T, TAllocator>::Data());

        isDynamic_ = true;
    }


private:
    bool isDynamic_;

};


} // namespace DXRL