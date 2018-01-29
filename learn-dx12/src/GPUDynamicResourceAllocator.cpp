#include <Rendering\Data\Resource\GPUDynamicResourceAllocator.hpp>

namespace DXRL
{

GPUDynamicResourcePtr::GPUDynamicResourcePtr() = default;

GPUDynamicResourcePtr::GPUDynamicResourcePtr(GPUDynamicResourcePtr const& rhs) = default;

GPUDynamicResourcePtr::GPUDynamicResourcePtr(GPUDynamicResourcePtr&& rhs) = default;

GPUDynamicResourcePtr& GPUDynamicResourcePtr::operator=(GPUDynamicResourcePtr const& rhs) = default;

GPUDynamicResourcePtr& GPUDynamicResourcePtr::operator=(GPUDynamicResourcePtr&& rhs) = default;

////////////////////////////////////////////////////////////////////////////////////////////////////////

GPUDynamicResourceAllocator::GPUDynamicResourceAllocator() = default;

GPUDynamicResourceAllocator::GPUDynamicResourceAllocator(GPUFoundation& foundation, D3D12_HEAP_TYPE type, std::size_t size, std::size_t chunkSize) :
    defaultHeap_{ nullptr }, heapSize_{ 0 }, heapType_{ (D3D12_HEAP_TYPE)0 }, chunkSize_{ 0 }
{
    auto const& device = foundation.Device();

    D3D12_HEAP_DESC heapDesc{};
    heapDesc.Alignment = D3D12_DEFAULT_MSAA_RESOURCE_PLACEMENT_ALIGNMENT;
    heapDesc.Flags = D3D12_HEAP_FLAG_NONE;

    heapDesc.Properties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_NOT_AVAILABLE;
    heapDesc.Properties.CreationNodeMask = 0;
    heapDesc.Properties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    heapDesc.Properties.Type = type;
    heapDesc.Properties.VisibleNodeMask = 0;

    heapDesc.SizeInBytes = 1 << 26; // 67,108,864 bytes.

    {
        // Attampt to create GPU heap.

        auto const result = device->CreateHeap(&heapDesc, IID_PPV_ARGS(defaultHeap_.GetAddressOf()));
        ThrowIfFailed(result);
        heapSize_ = size;
        heapType_ = type;
        chunkSize_ = chunkSize;
    }

    {
        // Chunks metadata initialization.

        auto const chunkCount = size / chunkSize;
        chunksMetadata_.resize(chunkCount);

        for (size_t i = 0; i < chunkCount - 1; i++) {
            chunksMetadata_[i] = i + 1;
        }
        chunksMetadata_[chunkCount - 1] = 0;
    }
}

GPUDynamicResourceAllocator::GPUDynamicResourceAllocator(GPUDynamicResourceAllocator&& rhs) = default;

GPUDynamicResourceAllocator& GPUDynamicResourceAllocator::operator=(GPUDynamicResourceAllocator&& rhs) = default;

D3D12_HEAP_TYPE GPUDynamicResourceAllocator::Type() const
{
    return heapType_;
}

GPUDynamicResourcePtr GPUDynamicResourceAllocator::AllocDynamicResource(D3D12_RESOURCE_DESC const& resourceDesc, D3D12_RESOURCE_STATES initialStates)
{
    return GPUDynamicResourcePtr{};
}

void GPUDynamicResourceAllocator::FreeDynamicResource(GPUDynamicResourcePtr ptr)
{

}

}