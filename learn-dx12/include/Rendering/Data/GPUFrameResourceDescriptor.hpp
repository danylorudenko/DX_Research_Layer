#pragma once

#include <pch.hpp>

#include <Rendering\Data\GPUFrameResource.hpp>

class GPUFrameResourceDescriptor
{
public:
    GPUFrameResourceDescriptor();
    GPUFrameResourceDescriptor(int frameCount, Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& heap, 
                               UINT descriptorSize, int* offsetsInHeap,
                               D3D12_RESOURCE_STATES state, char const* semantics, 
                               GPUFrameResource* resource);
    GPUFrameResourceDescriptor(GPUFrameResourceDescriptor const&);
    GPUFrameResourceDescriptor(GPUFrameResourceDescriptor&&);

    GPUFrameResourceDescriptor& operator=(GPUFrameResourceDescriptor const&);
    GPUFrameResourceDescriptor& operator=(GPUFrameResourceDescriptor&&);

    bool IsValid() const { return describedResource_ != nullptr; }

    D3D12_GPU_DESCRIPTOR_HANDLE GPUViewHandle(int frameIndex) const;
    D3D12_CPU_DESCRIPTOR_HANDLE CPUViewHandle(int frameIndex) const;

    GPUFrameResource* DescribedResource() { return describedResource_; }

private:
    int frameCount_ = 0;

    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap_ = nullptr;
    std::vector<int> descriptorOffsets_;
    UINT descriptorSize_ = 0;

    D3D12_RESOURCE_STATES state_;
    std::string semantics_;

    GPUFrameResource* describedResource_ = nullptr;
};