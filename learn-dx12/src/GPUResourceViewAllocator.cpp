#include <Rendering\Data\Resource\ResourceView\GPUResourceViewAllocator.hpp>

GPUResourceViewAllocator::GPUResourceViewAllocator() = default;

GPUResourceViewAllocator::GPUResourceViewAllocator(GPUFoundation const& foundation) :
    foundation_{ &foundation }
{
    rtvHeap_ = GPUResourceViewHeap{ *foundation_, D3D12_DESCRIPTOR_HEAP_TYPE_RTV, RTV_HEAP_CAPACITY };
    dsvHeap_ = GPUResourceViewHeap{ *foundation_, D3D12_DESCRIPTOR_HEAP_TYPE_DSV, DSV_HEAP_CAPACITY };
    cbvSrvUavHeap_ = GPUResourceViewHeap{ *foundation_, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, CBV_SRV_UAV_CAPACITY };
}

GPUResourceViewAllocator::GPUResourceViewAllocator(GPUResourceViewAllocator&& rhs) = default;

GPUResourceViewAllocator& GPUResourceViewAllocator::operator=(GPUResourceViewAllocator&& rhs) = default;

GPUResourceViewDirectID GPUResourceViewAllocator::PushViewToList(GPUResourceView&& view)
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
    return PushViewToList(std::move(view));
}