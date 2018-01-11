#include <Rendering\Data\Resource\ResourceView\GPUResourceView.hpp>

GPUResourceView::GPUResourceView() = default;

GPUResourceView::GPUResourceView(GPUResourceViewDirectID viewID, GPUResourceViewHeap const& parentHeap) :
    viewID_{ viewID }, parentHeap_{ &parentHeap }
{ }

GPUResourceView::GPUResourceView(GPUResourceView const& rhs) = default;

GPUResourceView::GPUResourceView(GPUResourceView&& rhs) = default;

GPUResourceView& GPUResourceView::operator=(GPUResourceView const& rhs) = default;

GPUResourceView& GPUResourceView::operator=(GPUResourceView&& rhs) = default;

D3D12_CPU_DESCRIPTOR_HANDLE GPUResourceView::CPUHandle() const
{
    return D3D12_CPU_DESCRIPTOR_HANDLE{};
}