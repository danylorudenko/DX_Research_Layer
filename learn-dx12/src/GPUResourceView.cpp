#include <Rendering\Data\Resource\View\GPUResourceView.hpp>

GPUResourceView::GPUResourceView() :
    descriptorHeap_{ nullptr }, descriptorOffset_{ 0 }, descriptorSize_{ 0 }, targetState_{ D3D12_RESOURCE_STATE_INVALID }, describedResource_{ nullptr }
{ }

GPUResourceView::GPUResourceView(Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& heap, std::int32_t descriptorOffset, std::uint32_t descriptorSize, D3D12_RESOURCE_STATES targetState, GPUResource* describedResource) :
    descriptorHeap_{ heap }, descriptorOffset_{ descriptorOffset }, descriptorSize_{ descriptorSize }, targetState_{ targetState }, describedResource_{ describedResource }
{ }

GPUResourceView::GPUResourceView(GPUResourceView const&) = default;

GPUResourceView::GPUResourceView(GPUResourceView&&) = default;

GPUResourceView& GPUResourceView::operator=(GPUResourceView const& rhs) = default;

GPUResourceView& GPUResourceView::operator=(GPUResourceView&& rhs) = default;

D3D12_GPU_DESCRIPTOR_HANDLE GPUResourceView::GPUHandle() const
{
    CD3DX12_GPU_DESCRIPTOR_HANDLE handle(descriptorHeap_->GetGPUDescriptorHandleForHeapStart(), descriptorOffset_, descriptorSize_);
    return handle;
}

D3D12_CPU_DESCRIPTOR_HANDLE GPUResourceView::CPUHandle() const
{
    CD3DX12_CPU_DESCRIPTOR_HANDLE handle(descriptorHeap_->GetCPUDescriptorHandleForHeapStart(), descriptorOffset_, descriptorSize_);
    return handle;
}

GPUResource const* GPUResourceView::Resource() const
{
    return describedResource_;
}