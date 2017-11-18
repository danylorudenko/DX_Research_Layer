#pragma once

#include <pch.hpp>

#include <Rendering\Data\GPUResource.hpp>

class GPUFrameResourceDescriptor
{
public:
    GPUFrameResourceDescriptor();
    GPUFrameResourceDescriptor(int frameCount, Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& heap, 
                               int descriptorSize, int* offsetsInHeap,
                               D3D12_RESOURCE_STATES state, char const* semantics, 
                               std::vector<GPUResource*> resource);
    GPUFrameResourceDescriptor(GPUFrameResourceDescriptor const&);
    GPUFrameResourceDescriptor(GPUFrameResourceDescriptor&&);

    GPUFrameResourceDescriptor& operator=(GPUFrameResourceDescriptor const&);
    GPUFrameResourceDescriptor& operator=(GPUFrameResourceDescriptor&&);

    D3D12_GPU_DESCRIPTOR_HANDLE GPUViewHandle(int frameIndex) const;
    D3D12_CPU_DESCRIPTOR_HANDLE CPUViewHandle(int frameIndex) const;

private:
    int frameCount_ = 0;

    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap_;
    std::vector<int> descriptorOffsets_;
    int descriptorSize_;

    D3D12_RESOURCE_STATES state_;
    std::string semantics_;

    std::vector<GPUResource*> describedResources_;
};