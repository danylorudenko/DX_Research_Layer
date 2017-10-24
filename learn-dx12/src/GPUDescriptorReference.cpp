#include <Data\GPUDescriptorReference.hpp>

GPUDescriptorReference::GPUDescriptorReference() = default;

GPUDescriptorReference::GPUDescriptorReference(Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& heap, int descriptorSize, int offsetInHeap) :
    descriptorHeap_(heap),
    descriptorSize_(descriptorSize),
    descriptorOffset_(offsetInHeap)
{

}

GPUDescriptorReference::GPUDescriptorReference(GPUDescriptorReference const&) = default;

GPUDescriptorReference::GPUDescriptorReference(GPUDescriptorReference&&) = default;

GPUDescriptorReference& GPUDescriptorReference::operator=(GPUDescriptorReference const&) = default;

GPUDescriptorReference& GPUDescriptorReference::operator=(GPUDescriptorReference&&) = default;

D3D12_GPU_DESCRIPTOR_HANDLE GPUDescriptorReference::GPUHandle() const
{
    CD3DX12_GPU_DESCRIPTOR_HANDLE handle(descriptorHeap_->GetGPUDescriptorHandleForHeapStart(), descriptorOffset_, descriptorSize_);
    return handle;
}

D3D12_CPU_DESCRIPTOR_HANDLE GPUDescriptorReference::CPUHandle() const
{
    CD3DX12_CPU_DESCRIPTOR_HANDLE handle(descriptorHeap_->GetCPUDescriptorHandleForHeapStart(), descriptorOffset_, descriptorSize_);
    return handle;
}