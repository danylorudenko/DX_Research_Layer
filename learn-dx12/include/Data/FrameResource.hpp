#pragma once

#include <pch.hpp>

using namespace Microsoft;

class FrameResource
{
public:
    FrameResource();
    FrameResource(
        ID3D12Device& device,
        WRL::ComPtr<ID3D12DescriptorHeap> sharedHeap, 
        UINT offsetInHeap,
        WRL::ComPtr<ID3D12Resource> frameBuffer);

    FrameResource(const FrameResource&) = delete;
    FrameResource& operator=(const FrameResource&) = delete;

    FrameResource(FrameResource&&);
    FrameResource& operator=(FrameResource&&);

    D3D12_CPU_DESCRIPTOR_HANDLE CPUDescriptorHandle() const;
    D3D12_GPU_DESCRIPTOR_HANDLE GPUDescriptorHandle() const;

    ID3D12Resource* FrameBuffer() const;

    ID3D12CommandAllocator* CommandAllocator() const;

    void SetTargetFenceValue(UINT64 targetValue);
    UINT64 CompletedFenceValue() const;
    UINT64 TargetFenceValue() const;
    ID3D12Fence* Fence() const;
    HANDLE FenceEvent() const;

private:
    WRL::ComPtr<ID3D12Resource> frameBuffer_;
    WRL::ComPtr<ID3D12DescriptorHeap> sharedRtvDescriptorHeap_;
    UINT offsetInRtvHeap_;

    WRL::ComPtr<ID3D12CommandAllocator> commandAllocator_;
    WRL::ComPtr<ID3D12Fence> frameFence_;

    UINT64 targetFenceValue_;
    HANDLE fenceEvent_;
};