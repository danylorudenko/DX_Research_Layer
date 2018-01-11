#pragma once

#include <pch.hpp>

class GPUResourceDirectID
{
public:
    GPUResourceDirectID();
    GPUResourceDirectID(std::size_t ID);

    GPUResourceDirectID(GPUResourceDirectID const& rhs);
    GPUResourceDirectID(GPUResourceDirectID&& rhs);

    GPUResourceDirectID& operator=(GPUResourceDirectID const& rhs);
    GPUResourceDirectID& operator=(GPUResourceDirectID&& rhs);

    std::size_t ID() const;

private:
    std::size_t ID_ = 0;
};

//////////////////////////////////////////////////////////////////////////////////

class GPUResourceID
{
public:
    GPUResourceID();
    GPUResourceID(std::size_t ID);

    GPUResourceID(GPUResourceID const& rhs);
    GPUResourceID(GPUResourceID&& rhs);

    GPUResourceID& operator=(GPUResourceID const& rhs);
    GPUResourceID& operator=(GPUResourceID&& rhs);

    std::size_t ID() const;

private:
    std::size_t ID_ = 0;
};

///////////////////////////////////////////////////////////////////////////////////

class GPUResourceViewHeap;

class GPUResourceViewDirectID
{
public:
    GPUResourceViewDirectID();
    GPUResourceViewDirectID(std::size_t ID, GPUResourceViewHeap const& parentHeap);
    GPUResourceViewDirectID(GPUResourceViewDirectID const& rhs);
    GPUResourceViewDirectID(GPUResourceViewDirectID&& rhs);

    GPUResourceViewDirectID& operator=(GPUResourceViewDirectID const& rhs);
    GPUResourceViewDirectID& operator=(GPUResourceViewDirectID&& rhs);

    std::size_t ID() const;

private:
    std::size_t ID_ = 0;
    GPUResourceViewHeap const* parentHeap_;
};

