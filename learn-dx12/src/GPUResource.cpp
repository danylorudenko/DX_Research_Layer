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

Microsoft::WRL::ComPtr<ID3D12Resource>& GPUResource::GetHandle(IResourceHandleGetter const&)
{
    return resourceHandle_;
}

D3D12_RESOURCE_STATES& GPUResource::GetVirtualState(IResourceStateGetter const&)
{
    return virtualState_;
}

D3D12_GPU_VIRTUAL_ADDRESS GPUResource::GetGPUVirtualAddress(IResourceVirtualGPUAddressGetter const&)
{
    return resourceHandle_->GetGPUVirtualAddress();
}

std::size_t& GPUResource::GetSize(IResourceSizeGetter const&)
{
    return size_;
}

std::size_t& GPUResource::GetCapacity(IResourceCapacityGetter const&)
{
    return capacity_;
}