#include <Rendering\Data\Resource\ResourceView\GPUResourceView.hpp>

GPUResourceView::GPUResourceView() : offsetInHeap_{ SIZE_MAX }, describedResource_{ SIZE_MAX }
{ }

GPUResourceView::GPUResourceView(GPUResourceView const& rhs) = default;

GPUResourceView::GPUResourceView(GPUResourceView&& rhs) = default;

GPUResourceView& GPUResourceView::operator=(GPUResourceView const& rhs) = default;

GPUResourceView& GPUResourceView::operator=(GPUResourceView&& rhs) = default;