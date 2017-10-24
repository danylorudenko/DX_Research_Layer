#pragma once

#include <pch.hpp>

#include <Data\GPUResource.hpp>

class GPUGraphResourceProxy
{
public:
    GPUGraphResourceProxy();
    GPUGraphResourceProxy(GPUGraphResourceProxy const&) = delete;
    GPUGraphResourceProxy(GPUGraphResourceProxy&& rhs);

    GPUGraphResourceProxy& operator=(GPUGraphResourceProxy const&) = delete;
    GPUGraphResourceProxy& operator=(GPUGraphResourceProxy&& rhs) = delete;

    void Insert(char const* semantics, GPUResource* resource);
    GPUResource* Request(char const* semantics);

    void Clear() { resourceMap_.clear(); }

private:
    std::map<std::string const, GPUResource*> resourceMap_;
};