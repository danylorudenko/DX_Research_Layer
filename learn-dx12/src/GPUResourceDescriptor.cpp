#include <Data\GPUResourceDescriptor.hpp>

GPUResourceDescriptor::GPUResourceDescriptor() = default;

GPUResourceDescriptor::GPUResourceDescriptor(Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& heap, 
                                             int descriptorSize, int offsetInHeap, 
                                             D3D12_RESOURCE_STATES state, char const* semantics, GPUResource* resource) :
    descriptorHeap_(heap),
    descriptorSize_(descriptorSize),
    descriptorOffset_(offsetInHeap),
    semantics_(semantics),
    state_(state),
    describedResource_(resource)
{

}

GPUResourceDescriptor::GPUResourceDescriptor(GPUResourceDescriptor const&) = default;

GPUResourceDescriptor::GPUResourceDescriptor(GPUResourceDescriptor&&) = default;

GPUResourceDescriptor& GPUResourceDescriptor::operator=(GPUResourceDescriptor const&) = default;

GPUResourceDescriptor& GPUResourceDescriptor::operator=(GPUResourceDescriptor&&) = default;

D3D12_GPU_DESCRIPTOR_HANDLE GPUResourceDescriptor::GPUViewHandle() const
{
    CD3DX12_GPU_DESCRIPTOR_HANDLE handle(descriptorHeap_->GetGPUDescriptorHandleForHeapStart(), descriptorOffset_, descriptorSize_);
    return handle;
}

D3D12_CPU_DESCRIPTOR_HANDLE GPUResourceDescriptor::CPUViewHandle() const
{
    CD3DX12_CPU_DESCRIPTOR_HANDLE handle(descriptorHeap_->GetCPUDescriptorHandleForHeapStart(), descriptorOffset_, descriptorSize_);
    return handle;
}

std::string const& GPUResourceDescriptor::Semantics() const
{
    return semantics_;
}
