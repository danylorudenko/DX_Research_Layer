#pragma once

#include <pch.hpp>

#include <Rendering\Interfaces\IResourceHandleEditor.hpp>
#include <Rendering\Interfaces\IResourceStateEditor.hpp>
#include <Rendering\Interfaces\IResourceStateGetter.hpp>
#include <Rendering\Interfaces\IResourceSizeEditor.hpp>
#include <Rendering\Interfaces\IResourceSizeGetter.hpp>
#include <Rendering\Interfaces\IResourceCapacityEditor.hpp>
#include <Rendering\Interfaces\IResourceCapacityGetter.hpp>
#include <Rendering\Interfaces\IResourceVirtualGPUAddressGetter.hpp>

struct GPUResourceDesc
{
    Microsoft::WRL::ComPtr<ID3D12Resource> resourceHandle = nullptr;
    D3D12_RESOURCE_STATES state = D3D12_RESOURCE_STATE_COMMON;
    std::size_t size = 0;
    std::size_t capcatity = 0;
};

class GPUResource
{
public:
    GPUResource();
    GPUResource(GPUResourceDesc const* resourceDesc);

    GPUResource(GPUResource const& rhs) = delete;
    GPUResource(GPUResource&& rhs);

    GPUResource& operator=(GPUResource const& rhs) = delete;
    GPUResource& operator=(GPUResource&& rhs);
    
    Microsoft::WRL::ComPtr<ID3D12Resource>& AccessHandle(IResourceHandleEditor const&);
    
    std::size_t& AccessSize(IResourceSizeEditor const&);
    std::size_t& AccessCapacity(IResourceCapacityEditor const&);
    D3D12_RESOURCE_STATES& AccessVirtualState(IResourceStateEditor const&);
    
    std::size_t GetSize(IResourceSizeGetter const&) const;
    std::size_t GetCapacity(IResourceCapacityGetter const&) const;
    D3D12_RESOURCE_STATES GetVirtualState(IResourceStateGetter const&) const;

    D3D12_GPU_VIRTUAL_ADDRESS GetGPUVirtualAddress(IResourceVirtualGPUAddressGetter const&);


private:
    Microsoft::WRL::ComPtr<ID3D12Resource> resourceHandle_;
    D3D12_RESOURCE_STATES virtualState_;
    std::size_t size_;
    std::size_t capacity_;
};