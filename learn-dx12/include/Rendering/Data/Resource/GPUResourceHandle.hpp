#pragma once

#include <pch.hpp>

class GPUResource;
class GPUStaticResourceAllocator;

class GPUResourceHandle
{
public:
    GPUResourceHandle();
    GPUResourceHandle(std::size_t ID, GPUStaticResourceAllocator& allocator);

    GPUResourceHandle(GPUResourceHandle const& rhs);
    GPUResourceHandle(GPUResourceHandle&& rhs);

    GPUResourceHandle& operator=(GPUResourceHandle const& rhs);
    GPUResourceHandle& operator=(GPUResourceHandle&& rhs);

    std::size_t ID() const;
    
    GPUResource& Resource() const;

private:
    std::size_t ID_ = 0;
    GPUStaticResourceAllocator* allocator_ = nullptr;
};

//////////////////////////////////////////////////////////////////////////////////

class GPUResourceView;
class GPUResourceViewTable;
class GPUResourceViewAllocator;

class GPUResourceViewHandle
{
public:
    GPUResourceViewHandle();
    GPUResourceViewHandle(std::size_t ID, GPUResourceViewTable& viewTable, GPUResourceViewAllocator& allocator);

    GPUResourceViewHandle(GPUResourceViewHandle const& rhs);
    GPUResourceViewHandle(GPUResourceViewHandle&& rhs);

    GPUResourceViewHandle& operator=(GPUResourceViewHandle const& rhs);
    GPUResourceViewHandle& operator=(GPUResourceViewHandle&& rhs);

    std::size_t ID() const;
    GPUResourceView& View(std::size_t frameIndex) const;

private:
    std::size_t ID_ = 0;
    GPUResourceViewTable* viewTable_;
    GPUResourceViewAllocator* viewAllocator_;
};

///////////////////////////////////////////////////////////////////////////////////

class GPUResourceViewHeap;

class GPUResourceViewDirectHandle
{
public:
    GPUResourceViewDirectHandle();
    GPUResourceViewDirectHandle(std::size_t ID);
    GPUResourceViewDirectHandle(GPUResourceViewDirectHandle const& rhs);
    GPUResourceViewDirectHandle(GPUResourceViewDirectHandle&& rhs);

    GPUResourceViewDirectHandle& operator=(GPUResourceViewDirectHandle const& rhs);
    GPUResourceViewDirectHandle& operator=(GPUResourceViewDirectHandle&& rhs);

    std::size_t ID() const;

private:
    std::size_t ID_ = 0;
};

