#include <Rendering\Data\Resource\View\GPUResourceNativeView.hpp>

GPUResourceNativeView::GPUResourceNativeView() :
    GPUResourceView{}, descriptorHeap_{ nullptr }, descriptorOffset_{ 0 }, descriptorSize_{ 0 }
{ }

GPUResourceNativeView::GPUResourceNativeView(Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& heap, std::int32_t descriptorOffset, std::uint32_t descriptorSize, D3D12_RESOURCE_STATES targetState, GPUResource* describedResource) :
    GPUResourceView{ targetState, describedResource }, descriptorHeap_{ heap }, descriptorOffset_{ descriptorOffset }, descriptorSize_{ descriptorSize }
{ }

GPUResourceNativeView::GPUResourceNativeView(GPUResourceNativeView const& rhs) = default;

GPUResourceNativeView::GPUResourceNativeView(GPUResourceNativeView&& rhs) = default;

GPUResourceNativeView& GPUResourceNativeView::operator=(GPUResourceNativeView const& rhs) = default;

GPUResourceNativeView& GPUResourceNativeView::operator=(GPUResourceNativeView&& rhs) = default;

D3D12_GPU_DESCRIPTOR_HANDLE GPUResourceNativeView::GPUHandle() const
{
    CD3DX12_GPU_DESCRIPTOR_HANDLE handle(descriptorHeap_->GetGPUDescriptorHandleForHeapStart(), descriptorOffset_, descriptorSize_);
    return handle;
}

D3D12_CPU_DESCRIPTOR_HANDLE GPUResourceNativeView::CPUHandle() const
{
    CD3DX12_CPU_DESCRIPTOR_HANDLE handle(descriptorHeap_->GetCPUDescriptorHandleForHeapStart(), descriptorOffset_, descriptorSize_);
    return handle;
}

bool GPUResourceNativeView::IsNativeView() const
{
    return true;
}