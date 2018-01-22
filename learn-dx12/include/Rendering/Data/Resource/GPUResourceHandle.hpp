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
    std::size_t ID_ = (std::numeric_limits<std::size_t>::max)();
    GPUStaticResourceAllocator* allocator_ = nullptr;
};

//////////////////////////////////////////////////////////////////////////////////

class GPUResourceView;
class GPUResourceViewContextTable;
class GPUResourceViewAllocator;

class GPUResourceViewHandle
{
public:
    GPUResourceViewHandle();
    GPUResourceViewHandle(std::size_t ID, GPUResourceViewContextTable& viewTable, GPUResourceViewAllocator& allocator);

    GPUResourceViewHandle(GPUResourceViewHandle const& rhs);
    GPUResourceViewHandle(GPUResourceViewHandle&& rhs);

    GPUResourceViewHandle& operator=(GPUResourceViewHandle const& rhs);
    GPUResourceViewHandle& operator=(GPUResourceViewHandle&& rhs);

    std::size_t ID() const;
    bool IsValid() const;
    GPUResourceView& View(std::size_t frameIndex) const;

private:
    std::size_t ID_ = (std::numeric_limits<std::size_t>::max)();
    GPUResourceViewContextTable* viewContextTable_;
    GPUResourceViewAllocator* viewAllocator_;
};

///////////////////////////////////////////////////////////////////////////////////

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
    bool IsValid() const;

private:
    std::size_t ID_ = (std::numeric_limits<std::size_t>::max)();
};

