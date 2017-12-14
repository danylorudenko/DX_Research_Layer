#pragma once

#include <pch.hpp>

#include <Rendering\Data\Resource\GPUResource.hpp>

class GPUResourceView
{
public:
    GPUResourceView();
    GPUResourceView(Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& heap, std::int32_t descriptorOffset, std::uint32_t descriptorSize, D3D12_RESOURCE_STATES targetState, GPUResource* describedResource);

    GPUResourceView(GPUResourceView const& rhs);
    GPUResourceView(GPUResourceView&& rhs);

    GPUResourceView& operator=(GPUResourceView const& rhs);
    GPUResourceView& operator=(GPUResourceView&& rhs);

    D3D12_GPU_DESCRIPTOR_HANDLE GPUHandle() const;
    D3D12_CPU_DESCRIPTOR_HANDLE CPUHandle() const;

    GPUResource const* Resource() const;

private:
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap_;
    std::int32_t descriptorOffset_;
    std::uint32_t descriptorSize_;

    D3D12_RESOURCE_STATES targetState_;

    GPUResource* describedResource_;
};