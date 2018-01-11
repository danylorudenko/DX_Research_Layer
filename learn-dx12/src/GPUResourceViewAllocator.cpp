#include <Rendering\Data\Resource\ResourceView\GPUResourceViewAllocator.hpp>

GPUResourveViewAllocator::GPUResourveViewAllocator() = default;

GPUResourveViewAllocator::GPUResourveViewAllocator(GPUFoundation const& foundation) :
    foundation_{ &foundation }
{
    rtvHeap_ = GPUResourceViewHeap{ *foundation_, D3D12_DESCRIPTOR_HEAP_TYPE_RTV, RTV_HEAP_CAPACITY };
    dsvHeap_ = GPUResourceViewHeap{ *foundation_, D3D12_DESCRIPTOR_HEAP_TYPE_DSV, DSV_HEAP_CAPACITY };
    cbvSrvUavHeap_ = GPUResourceViewHeap{ *foundation_, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, CBV_SRV_UAV_CAPACITY };
}

GPUResourveViewAllocator::GPUResourveViewAllocator(GPUResourveViewAllocator&& rhs) = default;

GPUResourveViewAllocator& GPUResourveViewAllocator::operator=(GPUResourveViewAllocator&& rhs) = default;