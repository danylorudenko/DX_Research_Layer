#pragma once

#include <pch.hpp>

#include <Data\GPUResource.hpp>

class GPUFrameResourceDescriptor
{
public:
    GPUFrameResourceDescriptor();
    GPUFrameResourceDescriptor(UINT frameCount, Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& heap, 
                               int descriptorSize, std::vector<int> offsetInHeap, 
                               D3D12_RESOURCE_STATES state, char const* semantics, std::vector<GPUResource*> resource);
    GPUFrameResourceDescriptor(GPUFrameResourceDescriptor const&);
    GPUFrameResourceDescriptor(GPUFrameResourceDescriptor&&);

    GPUFrameResourceDescriptor& operator=(GPUFrameResourceDescriptor const&);
    GPUFrameResourceDescriptor& operator=(GPUFrameResourceDescriptor&&);

    D3D12_GPU_DESCRIPTOR_HANDLE GPUViewHandle(UINT frameIndex) const;
    D3D12_CPU_DESCRIPTOR_HANDLE CPUViewHandle(UINT frameIndex) const;

private:
    UINT frameCount_;

    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap_;
    int descriptorSize_;
    std::vector<int> descriptorOffsets_;

    D3D12_RESOURCE_STATES state_;
    std::string semantics_;

    std::vector<GPUResource*> describedResources_;
};