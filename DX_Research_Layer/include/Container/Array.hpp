#pragma once

#include <Foundation\Types.hpp>
#include <Memory\Pointer.hpp>

namespace DXRL

{


////////////////////////////////////////
template<typename T, Size_t SIZE>
class Array
{
public:
    Array() = default;

    Array(Array const& rhs) 
    {
        operator=(rhs);
    }

    Array(Array&& rhs)
    {
        operator=(std::move(rhs));
    }

    Array& operator=(Array const& rhs)
    {
        for (S32_fast_t i = 0; i < SIZE; ++i) {
            array_[i] = rhs.array_[i];
        }

        return *this;
    }

    Array& operator=(Array&& rhs)
    {
        for (S32_fast_t i = 0; i < SIZE; ++i) {
            array_[i] = std::move<T>(rhs.array_[i]);
        }

        return *this;
    }

    inline Size_t constexpr Size() const { return SIZE; }

    inline T const& operator[](Size_t i) const { return array_[i]; }
    inline T& operator[](Size_t i) { return array_[i]; }

    inline T* Data() { return array_; }
    inline T const* Data() const { return array_; }

private:
    T array_[SIZE];
};


////////////////////////////////////////
template<typename T, Size_t STORAGE_SIZE>
class StaticArrayStorage
{
public:
    StaticArrayStorage()
        : size_{ 0 }
    { }

    StaticArrayStorage(StaticArrayStorage const& rhs)
    {
        operator=(rhs);
    }
    StaticArrayStorage(StaticArrayStorage&& rhs)
    {
        operator=(std::move(rhs));
    }

    StaticArrayStorage& operator=(StaticArrayStorage const& rhs)
    {
        Clear();
        
        auto const size = static_cast<S32_fast_t>(Size());
        for (S32_fast_t i = 0; i < size; ++i) {
            TypeRef(i) = rhs.TypeRef(i);
        }

        return *this;
    }

    StaticArrayStorage& operator=(StaticArrayStorage&& rhs)
    {
        Clear();

        auto const size = static_cast<S32_fast_t>(Size());
        for (S32_fast_t i = 0; i < size; ++i) {
            TypeRef(i) = std::move<T>(rhs.TypeRef(i));
        }

        rhs.Clear();
        return *this;
    }

    inline Size_t Size() const { return size_; }
    inline Size_t constexpr StorageSize() const { return STORAGE_SIZE; }
    inline bool IsFull() const { return size_ == STORAGE_SIZE; }


    inline T const& operator[](Size_t i) const 
    { 
        assert(i < size_);
        return TypeRef(i); 
    }

    inline T& operator[](Size_t i)
    {
        assert(i < size_);
        return TypeRef(i);
    }

    inline T* Data()
    {
        return reinterpret_cast<T*>(&array_);
    }

    inline T const* Data() const
    {
        return reinterpret_cast<T const*>(&array_);
    }

    template<typename... TArgs>
    void EmplaceBack(TArgs&&... args)
    {
        assert(size_ + 1 <= STORAGE_SIZE);
        new (TypePtr(size_++)) T{ std::forward<TArgs>(args)... };
    }

    void PushBackRange(T* range, Size_t size)
    {
        for (Size_t i = 0; i < size; ++i) {
            EmplaceBack(range[i]);
        }
    }
    
    void PopBack()
    {
        assert(size_ > 0);
        (TypePtr(--size_))->~T();
    }

    void Clear()
    {
        for (Size_t i = 0; i < size_; ++i) {
            TypePtr(i)->~T();
        }
        size_ = 0;
    }

    void _MoveData(T* dest)
    {
        Size_t const totalSize = Size() * sizeof(T);

        if (totalSize > 0)
            std::memcpy(dest, Data(), totalSize);
        size_ = 0;
    }

    void _WrapData(T const* src, Size_t count)
    {
        assert(Size() == 0);
        assert(STORAGE_SIZE >= count);

        Size_t const totalSize = count * sizeof(T);
        if (totalSize > 0)
            std::memcpy(Data(), src, totalSize);
        size_ = count;
    }

    inline void _ResizePure(Size_t size)
    {
        assert(size <= STORAGE_SIZE);
        size_ = size;
    }

private:
    inline T* TypePtr(Size_t i) { return reinterpret_cast<T*>(&array_) + i; }
    inline T const* TypePtr(Size_t i) const { return reinterpret_cast<T*>(&array_) + i; }
    
    inline T& TypeRef(Size_t i) { return *TypePtr(i); }
    inline T const& TypeRef(Size_t i) const { return *TypePtr(i); }

private:
    std::aligned_storage_t<sizeof(T) * STORAGE_SIZE, alignof(T)> array_;
    Size_t size_;

};


////////////////////////////////////////
template<typename T, typename TAllocator>
class DynamicArray
{
public:
    DynamicArray(TAllocator* allocator, Size_t reservedSize = 10)
        : allocator_{ allocator }
        , storage_{ nullptr }
        , size_{ 0 }
        , storageSize_{ reservedSize }
    {
        if (storageSize_ > 0) {
            storage_ = reinterpret_cast<T*>(allocator->AllocArray(storageSize_, sizeof(T), alignof(T)));
        }
    }

    DynamicArray(DynamicArray const& rhs)
    {
        operator=(rhs);
    }
    DynamicArray(DynamicArray&& rhs)
    {
        operator=(std::move(rhs));
    }

    DynamicArray& operator=(DynamicArray const& rhs)
    {
        Clear();
        auto const size = static_cast<S32_fast_t>(rhs.Size());
        if (storageSize_ < size) {
            ExpandStorage(size * 2);
        }
        for (S32_fast_t i = 0; i < size; ++i) {
            operator[](i) = rhs[i];
        }

        return *this;
    }

    DynamicArray& operator=(DynamicArray&& rhs)
    {
        Clear();
        auto const size = static_cast<S32_fast_t>(rhs.Size());
        if (storageSize_ < size) {
            ExpandStorage(size * 2);
        }
        for (S32_fast_t i = 0; i < size; ++i) {
            operator[](i) = std::move<T>(rhs[i]);
        }

        rhs.Clear();
        return *this;
    }

    inline T& operator[](Size_t i)
    {
        assert(i < size_ && "Out-of-bounds access to DynamicArray");
        return storage_[i];
    }

    inline T const& operator[](Size_t i) const
    {
        assert(i < size_ && "Out-of-bounds access to DynamicArray");
        return storage_[i];
    }

    inline T* Data()
    {
        return storage_;
    }

    inline T const* Data() const
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
        for (Size_t i = 0; i < size_; ++i) {
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

    inline Size_t Size() const
    {
        return size_;
    }

    inline Size_t StorageSize() const
    {
        return storageSize_;
    }

    inline TAllocator& Allocator()
    {
        return allocator_;
    }

    void ExpandStorage(Size_t newSize)
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
        Size_t const totalSize = Size() * sizeof(T);
        std::memcpy(dest, storage_, totalSize);
        size_ = 0;
    }

    void _WrapData(T const* src, Size_t count)
    {
        assert(Size() == 0);
        if (StorageSize() < count) {
            ExpandStorage(count + 2);
        }

        Size_t const totalSize = count * sizeof(T);
        std::memcpy(storage_, src, totalSize);
        size_ = count;
    }

    inline void _ResizePure(Size_t size)
    {
        assert(size <= storageSize_);
        size_ = size;
    }
    
    ~DynamicArray()
    {
        Reset();
    }

private:
    TAllocator* allocator_;
    T* storage_;
    Size_t size_;
    Size_t storageSize_; // in elements, not bytes
};


////////////////////////////////////////
template<typename T, Size_t INPLACE_SIZE, typename TAllocator>
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

    InplaceDynamicArray(InplaceDynamicArray const& rhs)
    {
        operator=(rhs);
    }
    InplaceDynamicArray(InplaceDynamicArray&& rhs)
    {
        operator=(std::move(rhs));
    }

    InplaceDynamicArray& operator=(InplaceDynamicArray const& rhs)
    {
        if (isDynamic_) {
            DynamicArray::operator=(rhs);
        }
        else {
            StaticArrayStorage::operator=(rhs);
        }

        return *this;
    }

    InplaceDynamicArray& operator=(InplaceDynamicArray&& rhs)
    {
        if (isDynamic_) {
            DynamicArray::operator=(std::move(rhs));
        }
        else {
            StaticArrayStorage::operator=(std::move(rhs));
        }

        return *this;
    }

    template<typename... TArgs>
    void EmplaceBack(TArgs&&... args)
    {
        if (isDynamic_) {
            DynamicArray::EmplaceBack(std::forward<TArgs>(args)...);
        }
        else {
            if (StaticArrayStorage::IsFull()) {
                _TransferToDynamic();
                DynamicArray::EmplaceBack(std::forward<TArgs>(args)...);
                return;
            }

            StaticArrayStorage::EmplaceBack(std::forward<TArgs>(args)...);
        }
    }

    void PopBack()
    {
        if (isDynamic_) {
            DynamicArray::PopBack();
        }
        else {
            StaticArrayStorage::PopBack();
        }
    }


    void Clear()
    {
        if (isDynamic_) {
            DynamicArray::Clear();
            isDynamic_ = false;
        }
        else {
            StaticArrayStorage::Clear();
        }
    }

    void Reset()
    {
        DynamicArray::Reset();
        StaticArrayStorage::Clear();
    }

    Size_t Size() const
    {
        if (isDynamic_) {
            return DynamicArray::Size();
        }
        else {
            return StaticArrayStorage::Size();
        }
    }

    T const* Data() const
    {
        if (isDynamic_) {
            return DynamicArray::Data();
        }
        else {
            return StaticArrayStorage::Data();
        }
    }

    T* Data()
    {
        if (isDynamic_) {
            return DynamicArray::Data();
        }
        else {
            return StaticArrayStorage::Data();
        }
    }

    T const& operator[](Size_t i) const
    {
        if (isDynamic_) {
            return DynamicArray::operator[](i);
        }
        else {
            return StaticArrayStorage::operator[](i);
        }
    }

    T& operator[](Size_t i)
    {
        if (isDynamic_) {
            return DynamicArray::operator[](i);
        }
        else {
            return StaticArrayStorage::operator[](i);
        }
    }

    inline TAllocator& Allocator()
    {
        return DynamicArray::Allocator();
    }


private:
    void _TransferToInplace()
    {
        Size_t const size = DynamicArray::Size();
        DynamicArray::_MoveData(StaticArrayStorage::Data());
        StaticArrayStorage::_ResizePure(size);

        isDynamic_ = false;
    }

    void _TransferToDynamic()
    {
        Size_t const size = StaticArrayStorage::Size();
        Size_t const requiredDynamicSize = size * 2;
        if (DynamicArray::StorageSize() < requiredDynamicSize) {
            DynamicArray::ExpandStorage(requiredDynamicSize);
        }

        DynamicArray::_ResizePure(size);
        StaticArrayStorage::_MoveData(DynamicArray::Data());

        isDynamic_ = true;
    }


private:
    bool isDynamic_;

};


} // namespace DXRL