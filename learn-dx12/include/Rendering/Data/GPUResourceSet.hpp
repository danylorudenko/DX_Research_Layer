#pragma once

#include <pch.hpp>

#include <Rendering\Data\GPUResource.hpp>

class GPUResourceSet
{
public:
    GPUResourceSet();
    GPUResourceSet(int framesCount, ID3D12Device* device, std::size_t size, D3D12_RESOURCE_DESC* resourceDesc, D3D12_RESOURCE_STATES state);
    GPUResourceSet(int framesCount, std::size_t resourceSize, Microsoft::WRL::ComPtr<ID3D12Resource>* resources, D3D12_RESOURCE_STATES state);
    GPUResourceSet(GPUResourceSet const&) = delete;
    GPUResourceSet(GPUResourceSet&& rhs);

    GPUResourceSet& operator=(GPUResourceSet const&) = delete;
    GPUResourceSet& operator=(GPUResourceSet&& rhs);
    
    Microsoft::WRL::ComPtr<ID3D12Resource>& AccessHandle(int resourceIndex, IResourceHandleEditor const&) const;

    std::size_t& AccessSize(std::size_t resourceIndex, IResourceSizeEditor const&);
    std::size_t GetSize(std::size_t resourceIndex, IResourceSizeGetter const&) const;
    
    std::size_t& AccessCapacity(std::size_t resourceIndex, IResourceCapacityEditor const&);
    std::size_t GetCapacity(std::size_t resourceIndex, IResourceCapacityGetter const&) const;
    
    D3D12_RESOURCE_STATES& AccessState(std::size_t resourceIndex, IResourceStateEditor const&);
    D3D12_RESOURCE_STATES GetState(std::size_t resourceIndex, IResourceStateGetter const&);

    D3D12_GPU_VIRTUAL_ADDRESS GetGPUVirtualAddress(std::size_t resourceIndex, IResourceVirtualGPUAddressGetter const&) const;
    std::size_t ResourceCount() const;

protected:
    std::size_t resourceCount = 0;
    std::vector<GPUResource> resources_;
};