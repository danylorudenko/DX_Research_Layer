#include <Rendering\Data\Resource\ResourceView\GPUResourceViewAllocator.hpp>

GPUResourceViewAllocator::GPUResourceViewAllocator() = default;

GPUResourceViewAllocator::GPUResourceViewAllocator(GPUFoundation& foundation) :
    foundation_{ &foundation }
{
    rtvHeap_ = GPUResourceViewHeap{ *foundation_, D3D12_DESCRIPTOR_HEAP_TYPE_RTV, RTV_HEAP_CAPACITY };
    dsvHeap_ = GPUResourceViewHeap{ *foundation_, D3D12_DESCRIPTOR_HEAP_TYPE_DSV, DSV_HEAP_CAPACITY };
    cbvSrvUavHeap_ = GPUResourceViewHeap{ *foundation_, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, CBV_SRV_UAV_CAPACITY };
}

GPUResourceViewAllocator::GPUResourceViewAllocator(GPUResourceViewAllocator&& rhs) = default;

GPUResourceViewAllocator& GPUResourceViewAllocator::operator=(GPUResourceViewAllocator&& rhs) = default;

GPUResourceViewDirectID GPUResourceViewAllocator::RegisterResourceView(GPUResourceView&& view)
{
    resourceViewsList_.emplace_back(std::make_unique<GPUResourceView>(std::move(view)));
    return GPUResourceViewDirectID{ resourceViewsList_.size() - 1 };
}

GPUResourceViewDirectID GPUResourceViewAllocator::AllocCBV(GPUResourceDirectID resourceID, D3D12_CONSTANT_BUFFER_VIEW_DESC const& desc, D3D12_RESOURCE_STATES targetState)
{
    auto& device = foundation_->Device();
    
    auto const offset = cbvSrvUavHeap_.ProvideNextOffset();
    CD3DX12_CPU_DESCRIPTOR_HANDLE handle{ cbvSrvUavHeap_.CPUHeapStart(), static_cast<INT>(offset), static_cast<UINT>(cbvSrvUavHeap_.DescriptorSize()) };

    device->CreateConstantBufferView(&desc, handle);
    GPUConstantBufferView view{ offset, cbvSrvUavHeap_, resourceID, targetState };
    return RegisterResourceView(std::move(view));
}

GPUResourceViewDirectID GPUResourceViewAllocator::AllocSRV(GPUResourceDirectID resourceID, D3D12_SHADER_RESOURCE_VIEW_DESC const& desc, D3D12_RESOURCE_STATES targetState)
{
    auto& device = foundation_->Device();

    auto const offset = cbvSrvUavHeap_.ProvideNextOffset();
    CD3DX12_CPU_DESCRIPTOR_HANDLE handle{ cbvSrvUavHeap_.CPUHeapStart(), static_cast<INT>(offset), static_cast<UINT>(cbvSrvUavHeap_.DescriptorSize()) };

    device->CreateShaderResourceView(foundation_->StaticResourceAllocator().AccessResource(resourceID).GetPtr(), &desc, handle);
    GPUShaderResourceView view{ offset, cbvSrvUavHeap_, resourceID, targetState };
    return RegisterResourceView(std::move(view));
}

GPUResourceViewDirectID GPUResourceViewAllocator::AllocUAV(GPUResourceDirectID resourceID, D3D12_UNORDERED_ACCESS_VIEW_DESC const& desc, D3D12_RESOURCE_STATES targetState)
{
    auto& device = foundation_->Device();

    auto const offset = cbvSrvUavHeap_.ProvideNextOffset();
    CD3DX12_CPU_DESCRIPTOR_HANDLE handle{ cbvSrvUavHeap_.CPUHeapStart(), static_cast<INT>(offset), static_cast<UINT>(cbvSrvUavHeap_.DescriptorSize()) };

    device->CreateUnorderedAccessView(foundation_->StaticResourceAllocator().AccessResource(resourceID).GetPtr(), nullptr, &desc, handle);
    GPUUnorderedAccessView view{ offset, cbvSrvUavHeap_, resourceID, targetState };
    return RegisterResourceView(std::move(view));
}