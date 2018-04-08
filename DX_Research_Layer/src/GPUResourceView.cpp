#include <pch.hpp>

#include <Rendering\Resource\ResourceView\GPUResourceView.hpp>

namespace DXRL
{

GPUResourceView::GPUResourceView() = default;

GPUResourceView::GPUResourceView(std::size_t offsetInHeap, GPUResourceViewHeap const& parentHeap) :
    offsetInHeap_{ offsetInHeap }, parentHeap_{ &parentHeap }
{ }

GPUResourceView::GPUResourceView(GPUResourceView&& rhs) = default;

GPUResourceView& GPUResourceView::operator=(GPUResourceView&& rhs) = default;

D3D12_CPU_DESCRIPTOR_HANDLE GPUResourceView::CPUHandle() const
{
    return CD3DX12_CPU_DESCRIPTOR_HANDLE{ parentHeap_->CPUHeapStart(), static_cast<INT>(offsetInHeap_), static_cast<UINT>(parentHeap_->DescriptorSize()) };
}

bool GPUResourceView::IsValid() const
{
    return offsetInHeap_ != (std::numeric_limits<std::size_t>::max)();
}

GPUResourceView::~GPUResourceView() = default;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

GPUShaderVisibleResourceView::GPUShaderVisibleResourceView() = default;

GPUShaderVisibleResourceView::GPUShaderVisibleResourceView(std::size_t offsetInHeap, GPUResourceViewHeap const& parentHeap, GPUResourceHandle const& resourceID, D3D12_RESOURCE_STATES targetState) :
    GPUResourceView{ offsetInHeap, parentHeap }, resourceID_{ resourceID }, targetState_{ targetState }
{ }

GPUShaderVisibleResourceView::GPUShaderVisibleResourceView(GPUShaderVisibleResourceView&& rhs) = default;

GPUShaderVisibleResourceView& GPUShaderVisibleResourceView::operator=(GPUShaderVisibleResourceView&& rhs) = default;

GPUResource& GPUShaderVisibleResourceView::Resource() const
{
    return resourceID_.Resource();
}

D3D12_RESOURCE_STATES GPUShaderVisibleResourceView::TargetState() const
{
    return targetState_;
}

D3D12_GPU_DESCRIPTOR_HANDLE GPUShaderVisibleResourceView::GPUHandle() const
{
    return CD3DX12_GPU_DESCRIPTOR_HANDLE{ parentHeap_->GPUHeapStart(), static_cast<INT>(offsetInHeap_), static_cast<UINT>(parentHeap_->DescriptorSize()) };
}

/////////////////////////////////////////////////////////////////////////////////

GPUConstantBufferView::GPUConstantBufferView() = default;

GPUConstantBufferView::GPUConstantBufferView(std::size_t offsetInHeap, GPUResourceViewHeap const& parentHeap, GPUResourceHandle const& resourceID, D3D12_RESOURCE_STATES targetState) :
    GPUShaderVisibleResourceView{ offsetInHeap, parentHeap, resourceID, targetState }
{ }

GPUConstantBufferView::GPUConstantBufferView(GPUConstantBufferView&& rhs) = default;

GPUConstantBufferView& GPUConstantBufferView::operator=(GPUConstantBufferView&& rhs) = default;

/////////////////////////////////////////////////////////////////////////////////

GPUShaderResourceView::GPUShaderResourceView() = default;

GPUShaderResourceView::GPUShaderResourceView(std::size_t offsetInHeap, GPUResourceViewHeap const& parentHeap, GPUResourceHandle const& resourceID, D3D12_RESOURCE_STATES targetState) :
    GPUShaderVisibleResourceView{ offsetInHeap, parentHeap, resourceID, targetState }
{ }

GPUShaderResourceView::GPUShaderResourceView(GPUShaderResourceView&& rhs) = default;

GPUShaderResourceView& GPUShaderResourceView::operator=(GPUShaderResourceView&& rhs) = default;

/////////////////////////////////////////////////////////////////////////////////

GPUUnorderedAccessView::GPUUnorderedAccessView() = default;

GPUUnorderedAccessView::GPUUnorderedAccessView(std::size_t offsetInHeap, GPUResourceViewHeap const& parentHeap, GPUResourceHandle const& resourceID, D3D12_RESOURCE_STATES targetState) :
    GPUShaderVisibleResourceView{ offsetInHeap, parentHeap, resourceID, targetState }
{ }

GPUUnorderedAccessView::GPUUnorderedAccessView(GPUUnorderedAccessView&& rhs) = default;

GPUUnorderedAccessView& GPUUnorderedAccessView::operator=(GPUUnorderedAccessView&& rhs) = default;

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////


GPUDepthStencilView::GPUDepthStencilView() = default;

GPUDepthStencilView::GPUDepthStencilView(GPUDepthStencilView&& rhs) = default;

GPUDepthStencilView::GPUDepthStencilView(std::size_t offsetInHeap, GPUResourceViewHeap const& parentHeap, GPUResourceHandle const& resourceID, D3D12_RESOURCE_STATES targetState) :
    GPUResourceView{ offsetInHeap, parentHeap }, resourceID_{ resourceID }, targetState_{ targetState }
{ }

GPUDepthStencilView& GPUDepthStencilView::operator=(GPUDepthStencilView&& rhs) = default;

GPUResource& GPUDepthStencilView::Resource() const
{
    return resourceID_.Resource();
}

D3D12_RESOURCE_STATES GPUDepthStencilView::TargetState() const
{
    return targetState_;
}

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

GPURenderTargetView::GPURenderTargetView() = default;

GPURenderTargetView::GPURenderTargetView(std::size_t offsetInHeap, GPUResourceViewHeap const& parentHeap) :
    GPUResourceView{ offsetInHeap, parentHeap }
{ }

GPURenderTargetView::GPURenderTargetView(GPURenderTargetView&& rhs) = default;

GPURenderTargetView& GPURenderTargetView::operator=(GPURenderTargetView&& rhs) = default;

D3D12_RESOURCE_STATES GPURenderTargetView::TargetState() const
{
    return D3D12_RESOURCE_STATE_RENDER_TARGET;
}

/////////////////////////////////////////////////////////////////////////////////

GPUSwapChainRenderTargetView::GPUSwapChainRenderTargetView() = default;

GPUSwapChainRenderTargetView::GPUSwapChainRenderTargetView(std::size_t offsetInHeap, GPUResourceViewHeap const& parentHeap, GPUResource& renderBuffer) :
    GPURenderTargetView{ offsetInHeap, parentHeap }, renderBuffer_{ &renderBuffer }
{ }

GPUSwapChainRenderTargetView::GPUSwapChainRenderTargetView(GPUSwapChainRenderTargetView&& rhs) = default;

GPUSwapChainRenderTargetView& GPUSwapChainRenderTargetView::operator=(GPUSwapChainRenderTargetView&& rhs) = default;

GPUResource& GPUSwapChainRenderTargetView::Resource() const
{
    return *renderBuffer_;
}

/////////////////////////////////////////////////////////////////////////////////

GPUGenericRenderTargetView::GPUGenericRenderTargetView() = default;

GPUGenericRenderTargetView::GPUGenericRenderTargetView(std::size_t offsetInHeap, GPUResourceViewHeap const& parentHeap, GPUResourceHandle const& resourceID) :
    GPURenderTargetView{ offsetInHeap, parentHeap }, resourceID_{ resourceID }
{ }

GPUGenericRenderTargetView::GPUGenericRenderTargetView(GPUGenericRenderTargetView&& rhs) = default;

GPUGenericRenderTargetView& GPUGenericRenderTargetView::operator=(GPUGenericRenderTargetView&& rhs) = default;

GPUResource& GPUGenericRenderTargetView::Resource() const
{
    return resourceID_.Resource();
}

}