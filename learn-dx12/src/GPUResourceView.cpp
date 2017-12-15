#include <Rendering\Data\Resource\View\GPUResourceView.hpp>

GPUResourceView::GPUResourceView() :
    targetState_{ D3D12_RESOURCE_STATE_INVALID }, describedResource_{ nullptr }
{ }

GPUResourceView::GPUResourceView(D3D12_RESOURCE_STATES targetState, GPUResource* describedResource) :
    targetState_{ targetState }, describedResource_{ describedResource }
{ }

GPUResourceView::GPUResourceView(GPUResourceView const&) = default;

GPUResourceView::GPUResourceView(GPUResourceView&&) = default;

GPUResourceView& GPUResourceView::operator=(GPUResourceView const& rhs) = default;

GPUResourceView& GPUResourceView::operator=(GPUResourceView&& rhs) = default;

D3D12_RESOURCE_STATES GPUResourceView::ViewState() const
{
    return targetState_;
}

GPUResource const* GPUResourceView::Resource() const
{
    return describedResource_;
}

bool GPUResourceView::IsNativeView() const
{
    return false;
}