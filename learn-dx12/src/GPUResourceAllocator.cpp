#include <Rendering\Data\Resource\GPUResourceAllocator.hpp>
#include <Rendering\Data\Resource\GPUResourceFrameTable.hpp>

GPUResourceDirectID::GPUResourceDirectID() = default;

GPUResourceDirectID::GPUResourceDirectID(std::size_t ID) :
    ID_{ ID }
{ }

GPUResourceDirectID::GPUResourceDirectID(GPUResourceDirectID const&) = default;

GPUResourceDirectID::GPUResourceDirectID(GPUResourceDirectID&&) = default;

GPUResourceDirectID& GPUResourceDirectID::operator=(GPUResourceDirectID const&) = default;

GPUResourceDirectID& GPUResourceDirectID::operator=(GPUResourceDirectID&&) = default;

/////////////////////////////////////////////////////////////////////////////////////////

GPUResourceAllocator::GPUResourceAllocator() = default;

GPUResourceAllocator::GPUResourceAllocator(GPUFoundation const& foundation) :
    foundation_{ &foundation }
{ }

GPUResourceAllocator::GPUResourceAllocator(GPUResourceAllocator&&) = default;

GPUResourceAllocator& GPUResourceAllocator::operator=(GPUResourceAllocator&&) = default;

GPUResourceDirectID GPUResourceAllocator::Alloc(D3D12_RESOURCE_DESC const& resourceDesc, D3D12_RESOURCE_STATES initialState)
{
    Microsoft::WRL::ComPtr<ID3D12Resource> tempResourcePtr{ nullptr };
    auto& device = foundation_->Device();

    {
        auto const result = device->CreateCommittedResource(
            &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
            D3D12_HEAP_FLAG_NONE,
            &resourceDesc,
            initialState,
            nullptr,
            IID_PPV_ARGS(tempResourcePtr.GetAddressOf()));
        
        ThrowIfFailed(result);
    }

    committedResources_.push_back(std::make_unique<GPUResource>(std::move(tempResourcePtr), D3D12_RESOURCE_STATE_COMMON, L""));
    return GPUResourceDirectID{ committedResources_.size() - 1 };
}
