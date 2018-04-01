#include <Rendering\Resource\ResourceView\GPUResourceViewAllocator.hpp>
#include <Rendering\GPUFoundation.hpp>

namespace DXRL
{

GPUResourceViewAllocator::GPUResourceViewAllocator() = default;

GPUResourceViewAllocator::GPUResourceViewAllocator(GPUDelegate& foundation) :
    foundation_{ &foundation }
{
    rtvHeap_ = GPUResourceViewHeap{ *foundation_, D3D12_DESCRIPTOR_HEAP_TYPE_RTV, RTV_HEAP_CAPACITY };
    dsvHeap_ = GPUResourceViewHeap{ *foundation_, D3D12_DESCRIPTOR_HEAP_TYPE_DSV, DSV_HEAP_CAPACITY };
    cbvSrvUavHeap_ = GPUResourceViewHeap{ *foundation_, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, CBV_SRV_UAV_CAPACITY };
}

GPUResourceViewAllocator::GPUResourceViewAllocator(GPUResourceViewAllocator&& rhs) = default;

GPUResourceViewAllocator& GPUResourceViewAllocator::operator=(GPUResourceViewAllocator&& rhs) = default;

GPUResourceViewDirectHandle GPUResourceViewAllocator::AllocCBV(GPUResourceHandle const& resourceHandle, D3D12_CONSTANT_BUFFER_VIEW_DESC const& desc, D3D12_RESOURCE_STATES targetState)
{
    auto& device = foundation_->Device();

    auto const offset = cbvSrvUavHeap_.ProvideNextOffset();
    CD3DX12_CPU_DESCRIPTOR_HANDLE handle{ cbvSrvUavHeap_.CPUHeapStart(), static_cast<INT>(offset), static_cast<UINT>(cbvSrvUavHeap_.DescriptorSize()) };

    device->CreateConstantBufferView(&desc, handle);
    GPUConstantBufferView view{ offset, cbvSrvUavHeap_, resourceHandle, targetState };
    return RegisterResourceView(std::move(view));
}

GPUResourceViewDirectHandle GPUResourceViewAllocator::AllocSRV(GPUResourceHandle const& resourceHandle, D3D12_SHADER_RESOURCE_VIEW_DESC const& desc, D3D12_RESOURCE_STATES targetState)
{
    auto& device = foundation_->Device();

    auto const offset = cbvSrvUavHeap_.ProvideNextOffset();
    CD3DX12_CPU_DESCRIPTOR_HANDLE handle{ cbvSrvUavHeap_.CPUHeapStart(), static_cast<INT>(offset), static_cast<UINT>(cbvSrvUavHeap_.DescriptorSize()) };

    device->CreateShaderResourceView(resourceHandle.Resource().GetPtr(), &desc, handle);
    GPUShaderResourceView view{ offset, cbvSrvUavHeap_, resourceHandle, targetState };
    return RegisterResourceView(std::move(view));
}

GPUResourceViewDirectHandle GPUResourceViewAllocator::AllocUAV(GPUResourceHandle const& resourceHandle, D3D12_UNORDERED_ACCESS_VIEW_DESC const& desc, D3D12_RESOURCE_STATES targetState)
{
    auto& device = foundation_->Device();

    auto const offset = cbvSrvUavHeap_.ProvideNextOffset();
    CD3DX12_CPU_DESCRIPTOR_HANDLE handle{ cbvSrvUavHeap_.CPUHeapStart(), static_cast<INT>(offset), static_cast<UINT>(cbvSrvUavHeap_.DescriptorSize()) };

    device->CreateUnorderedAccessView(resourceHandle.Resource().GetPtr(), nullptr, &desc, handle);
    GPUUnorderedAccessView view{ offset, cbvSrvUavHeap_, resourceHandle, targetState };
    return RegisterResourceView(std::move(view));
}

GPUResourceViewDirectHandle GPUResourceViewAllocator::AllocRTV(GPUResourceHandle const& resourceHandle, D3D12_RENDER_TARGET_VIEW_DESC const& desc)
{
    auto& device = foundation_->Device();

    auto const offset = rtvHeap_.ProvideNextOffset();
    CD3DX12_CPU_DESCRIPTOR_HANDLE handle{ rtvHeap_.CPUHeapStart(), static_cast<INT>(offset), static_cast<UINT>(rtvHeap_.DescriptorSize()) };

    device->CreateRenderTargetView(resourceHandle.Resource().GetPtr(), &desc, handle);
    GPUGenericRenderTargetView view{ offset, rtvHeap_, resourceHandle };
    return RegisterResourceView(std::move(view));
}

GPUResourceViewDirectHandle GPUResourceViewAllocator::AllocSwapChainRTV(GPUResource& resource, D3D12_RENDER_TARGET_VIEW_DESC const& desc)
{
    auto& device = foundation_->Device();

    auto const offset = rtvHeap_.ProvideNextOffset();
    CD3DX12_CPU_DESCRIPTOR_HANDLE handle{ rtvHeap_.CPUHeapStart(), static_cast<INT>(offset), static_cast<UINT>(rtvHeap_.DescriptorSize()) };

    device->CreateRenderTargetView(resource.GetPtr(), &desc, handle);
    GPUSwapChainRenderTargetView view{ offset, rtvHeap_, resource };
    return RegisterResourceView(std::move(view));
}

GPUResourceViewDirectHandle GPUResourceViewAllocator::AllocDSV(GPUResourceHandle const& resourceHandle, D3D12_DEPTH_STENCIL_VIEW_DESC const& desc, D3D12_RESOURCE_STATES targetState)
{
    auto& device = foundation_->Device();

    auto const offset = dsvHeap_.ProvideNextOffset();
    CD3DX12_CPU_DESCRIPTOR_HANDLE handle{ dsvHeap_.CPUHeapStart(), static_cast<INT>(offset), static_cast<UINT>(dsvHeap_.DescriptorSize()) };

    device->CreateDepthStencilView(resourceHandle.Resource().GetPtr(), &desc, handle);
    GPUDepthStencilView view{ offset, dsvHeap_, resourceHandle, targetState };
    return RegisterResourceView(std::move(view));
}

void GPUResourceViewAllocator::SetDefaultHeaps(GPUEngine& executionEngine)
{
    ID3D12DescriptorHeap* heaps[] = { cbvSrvUavHeap_.Get() };
    executionEngine.Commit().SetDescriptorHeaps(1, heaps);
}

GPUResourceView& GPUResourceViewAllocator::AccessView(GPUResourceViewDirectHandle handle)
{
    return *(resourceViewsList_[handle.ID()]);
}

}