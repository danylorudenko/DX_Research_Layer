#pragma once

#include <pch.hpp>

class GPUDescriptorReference
{
public:
    GPUDescriptorReference();
    GPUDescriptorReference(Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& heap, int descriptorSize, int offsetInHeap);
    GPUDescriptorReference(GPUDescriptorReference const&& rhs);
    GPUDescriptorReference(GPUDescriptorReference&& rhs);

    GPUDescriptorReference& operator=(GPUDescriptorReference const& rhs);
    GPUDescriptorReference& operator=(GPUDescriptorReference&& rhs);

    D3D12_GPU_DESCRIPTOR_HANDLE GPUHandle() const;
    D3D12_CPU_DESCRIPTOR_HANDLE CPUHandle() const;

private:
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap_;
    int descriptorSize_;
    int descriptorOffset_;
};