#include <Rendering\Data\GPUResource.hpp>

GPUResource::GPUResource() : GPUResource(nullptr) 
{ }

GPUResource::GPUResource(GPUResourceDesc const* resourceDesc)
{
    GPUResourceDesc const& descLocal = resourceDesc ? *resourceDesc : GPUResourceDesc{};

    resourceHandle_ = descLocal.resourceHandle;
    virtualState_ = descLocal.state;
    size_ = descLocal.size;
    capacity_ = descLocal.capcatity;
}

GPUResource::GPUResource(GPUResource&& rhs)
{
    resourceHandle_ = rhs.resourceHandle_;
    virtualState_ = rhs.virtualState_;
    size_ = rhs.size_;
    capacity_ = rhs.capacity_;
}

GPUResource& GPUResource::operator=(GPUResource&& rhs)
{
    resourceHandle_ = rhs.resourceHandle_;
    virtualState_ = rhs.virtualState_;
    size_ = rhs.size_;
    capacity_ = rhs.capacity_;

    return *this;
}

ID3D12Resource* GPUResource::Get()
{
    return resourceHandle_.Get();
}

Microsoft::WRL::ComPtr<ID3D12Resource>& GPUResource::GetHandle()
{
    return resourceHandle_;
}

D3D12_RESOURCE_STATES GPUResource::GetVirtualState() const
{
    return virtualState_;
}

D3D12_RESOURCE_STATES GPUResource::SetVirtualState(D3D12_RESOURCE_STATES state)
{
    auto oldState = virtualState_;
    virtualState_ = state;
    return oldState;
}