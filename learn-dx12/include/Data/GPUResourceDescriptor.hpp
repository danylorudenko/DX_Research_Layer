#pragma once

#include <pch.hpp>

#include <Data\GPUResource.hpp>

class GPUResourceDescriptor
{
public:
    GPUResourceDescriptor();
    GPUResourceDescriptor(Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& heap, int descriptorSize, int offsetInHeap, D3D12_RESOURCE_STATES state, char const* semantics, GPUResource* resource);
    GPUResourceDescriptor(GPUResourceDescriptor const& rhs);
    GPUResourceDescriptor(GPUResourceDescriptor&& rhs);

    GPUResourceDescriptor& operator=(GPUResourceDescriptor const& rhs);
    GPUResourceDescriptor& operator=(GPUResourceDescriptor&& rhs);

    D3D12_GPU_DESCRIPTOR_HANDLE GPUViewHandle() const;
    D3D12_CPU_DESCRIPTOR_HANDLE CPUViewHandle() const;

    std::string const& Semantics() const;

private:
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap_;
    int descriptorSize_;
    int descriptorOffset_;

    D3D12_RESOURCE_STATES state_;
    std::string semantics_;

    GPUResource* describedResource_ = nullptr;
};