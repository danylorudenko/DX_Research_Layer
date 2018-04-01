#include <Rendering\Resource\GPUStaticResourceAllocator.hpp>
#include <Rendering\GPUFoundation.hpp>

namespace DXRL
{

GPUStaticResourceAllocator::GPUStaticResourceAllocator() = default;

GPUStaticResourceAllocator::GPUStaticResourceAllocator(GPUFoundation& foundation) :
    foundation_{ &foundation }
{ }

GPUStaticResourceAllocator::GPUStaticResourceAllocator(GPUStaticResourceAllocator&&) = default;

GPUStaticResourceAllocator& GPUStaticResourceAllocator::operator=(GPUStaticResourceAllocator&&) = default;

GPUResourceHandle GPUStaticResourceAllocator::AllocDefault(D3D12_RESOURCE_DESC const& resourceDesc, D3D12_RESOURCE_STATES initialState)
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
        DXRL_THROW_IF_FAILED(result);
    }

    committedResources_.push_back(std::make_unique<GPUResource>(std::move(tempResourcePtr), initialState));
    return GPUResourceHandle{ committedResources_.size() - 1, *this };
}

GPUResourceHandle GPUStaticResourceAllocator::AllocUpload(D3D12_RESOURCE_DESC const& resourceDesc, D3D12_RESOURCE_STATES initialState)
{
    Microsoft::WRL::ComPtr<ID3D12Resource> tempResourcePtr{ nullptr };
    auto& device = foundation_->Device();

    {
        auto const result = device->CreateCommittedResource(
            &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
            D3D12_HEAP_FLAG_NONE,
            &resourceDesc,
            initialState,
            nullptr,
            IID_PPV_ARGS(tempResourcePtr.GetAddressOf()));
        DXRL_THROW_IF_FAILED(result);
    }

    committedResources_.push_back(std::make_unique<GPUResource>(std::move(tempResourcePtr), initialState));
    return GPUResourceHandle{ committedResources_.size() - 1, *this };
}

GPUResourceHandle GPUStaticResourceAllocator::AllocReadback(D3D12_RESOURCE_DESC const& resourceDesc, D3D12_RESOURCE_STATES initialState)
{
    return GPUResourceHandle{};
}

GPUResource& GPUStaticResourceAllocator::AccessResource(GPUResourceHandle const& id)
{
    return *(committedResources_[id.ID()]);
}

}