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

    std::size_t ID_ = 0;
};
