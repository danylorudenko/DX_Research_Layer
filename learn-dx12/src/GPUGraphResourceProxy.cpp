#include <Data\GPUGraphResourceProxy.hpp>

GPUGraphResourceProxy::GPUGraphResourceProxy() = default;

GPUGraphResourceProxy::GPUGraphResourceProxy(GPUGraphResourceProxy&&) = default;

GPUGraphResourceProxy& GPUGraphResourceProxy::operator=(GPUGraphResourceProxy&&) = default;

void GPUGraphResourceProxy::Insert(char const* semantics, GPUResource* resource)
{
    resourceMap_.insert(std::make_pair(semantics, resource));
}

GPUResource* GPUGraphResourceProxy::Request(char const* semantics) const
{
    auto mapIterator = resourceMap_.find(semantics);
    return mapIterator != resourceMap_.end() ? mapIterator->second : nullptr;
}