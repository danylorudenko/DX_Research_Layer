#include <Rendering\Data\Resource\ResourceView\GPUResourceView.hpp>

GPUResourceView::GPUResourceView() = default;

GPUResourceView::GPUResourceView(std::size_t offsetInHeap, GPUResourceViewHeap const& parentHeap) :
    offsetInHeap_{ offsetInHeap }, parentHeap_{ &parentHeap }
{ }

GPUResourceView::GPUResourceView(GPUResourceView const& rhs) = default;

GPUResourceView::GPUResourceView(GPUResourceView&& rhs) = default;

GPUResourceView& GPUResourceView::operator=(GPUResourceView const& rhs) = default;

GPUResourceView& GPUResourceView::operator=(GPUResourceView&& rhs) = default;

D3D12_CPU_DESCRIPTOR_HANDLE GPUResourceView::CPUHandle() const
{
    return CD3DX12_CPU_DESCRIPTOR_HANDLE{ parentHeap_->CPUHeapStart(), static_cast<INT>(offsetInHeap_), static_cast<UINT>(parentHeap_->DescriptorSize()) };
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

GPUShaderVisibleResourceView::GPUShaderVisibleResourceView() = default;

GPUShaderVisibleResourceView::GPUShaderVisibleResourceView(std::size_t offsetInHeap, GPUResourceViewHeap const& parentHeap, GPUResourceDirectID resourceID, D3D12_RESOURCE_STATES targetState) :
    GPUResourceView{ offsetInHeap_, parentHeap }, resourceID_{ resourceID }, targetState_{ targetState }
{ }

GPUShaderVisibleResourceView::GPUShaderVisibleResourceView(GPUShaderVisibleResourceView const& rhs) = default;

GPUShaderVisibleResourceView::GPUShaderVisibleResourceView(GPUShaderVisibleResourceView&& rhs) = default;

GPUShaderVisibleResourceView& GPUShaderVisibleResourceView::operator=(GPUShaderVisibleResourceView const& rhs) = default;

GPUShaderVisibleResourceView& GPUShaderVisibleResourceView::operator=(GPUShaderVisibleResourceView&& rhs) = default;

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

GPUConstantBufferView::GPUConstantBufferView(std::size_t offsetInHeap, GPUResourceViewHeap const& parentHeap, GPUResourceDirectID resourceID, D3D12_RESOURCE_STATES targetState) :
    GPUShaderVisibleResourceView{ offsetInHeap, parentHeap, resourceID, targetState }
{ }

GPUConstantBufferView::GPUConstantBufferView(GPUConstantBufferView const& rhs) = default;

GPUConstantBufferView::GPUConstantBufferView(GPUConstantBufferView&& rhs) = default;

GPUConstantBufferView& GPUConstantBufferView::operator=(GPUConstantBufferView const& rhs) = default;

GPUConstantBufferView& GPUConstantBufferView::operator=(GPUConstantBufferView&& rhs) = default;

/////////////////////////////////////////////////////////////////////////////////

GPUShaderResourceView::GPUShaderResourceView() = default;

GPUShaderResourceView::GPUShaderResourceView(std::size_t offsetInHeap, GPUResourceViewHeap const& parentHeap, GPUResourceDirectID resourceID, D3D12_RESOURCE_STATES targetState) :
    GPUShaderVisibleResourceView{ offsetInHeap, parentHeap, resourceID, targetState }
{ }

GPUShaderResourceView::GPUShaderResourceView(GPUShaderResourceView const& rhs) = default;

GPUShaderResourceView::GPUShaderResourceView(GPUShaderResourceView&& rhs) = default;

GPUShaderResourceView& GPUShaderResourceView::operator=(GPUShaderResourceView const& rhs) = default;

GPUShaderResourceView& GPUShaderResourceView::operator=(GPUShaderResourceView&& rhs) = default;

/////////////////////////////////////////////////////////////////////////////////

GPUUnorderedAccessView::GPUUnorderedAccessView() = default;

GPUUnorderedAccessView::GPUUnorderedAccessView(std::size_t offsetInHeap, GPUResourceViewHeap const& parentHeap, GPUResourceDirectID resourceID, D3D12_RESOURCE_STATES targetState) :
    GPUShaderVisibleResourceView{ offsetInHeap, parentHeap, resourceID, targetState }
{ }

GPUUnorderedAccessView::GPUUnorderedAccessView(GPUUnorderedAccessView const& rhs) = default;

GPUUnorderedAccessView::GPUUnorderedAccessView(GPUUnorderedAccessView&& rhs) = default;

GPUUnorderedAccessView& GPUUnorderedAccessView::operator=(GPUUnorderedAccessView const& rhs) = default;

GPUUnorderedAccessView& GPUUnorderedAccessView::operator=(GPUUnorderedAccessView&& rhs) = default;

