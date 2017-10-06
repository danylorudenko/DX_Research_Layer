#pragma once

#include <pch.hpp>

#include <Data\GPUResource.hpp>

class GPUFrameResource : public GPUResource
{
public:
    GPUFrameResource();
    GPUFrameResource(ID3D12Device* device,
        Microsoft::WRL::ComPtr<ID3D12Resource> renderTarget, UINT width, UINT height,
        Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtvSharedHeap, INT offsetInRtvHeap,
        DXGI_FORMAT depthStencilFormat,
        Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsvSharedHeap, INT offsetInDsvHeap);

    GPUFrameResource(const GPUFrameResource&) = delete;
    GPUFrameResource& operator=(const GPUFrameResource&) = delete;

    GPUFrameResource(GPUFrameResource&&);
    GPUFrameResource& operator=(GPUFrameResource&&);

    D3D12_CPU_DESCRIPTOR_HANDLE CPURtvDescriptorHandle() const;
    D3D12_GPU_DESCRIPTOR_HANDLE GPURtvDescriptorHandle() const;

    D3D12_CPU_DESCRIPTOR_HANDLE CPUDsvDescriptorHandle() const;
    D3D12_GPU_DESCRIPTOR_HANDLE GPUDsvDescriptorHandle() const;

    ID3D12Resource* FrameBuffer() const;

private:
    Microsoft::WRL::ComPtr<ID3D12Resource> depthStencilBuffer_;

    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> sharedRtvDescriptorHeap_;
    UINT offsetInRtvHeap_;

    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> sharedDsvDescriptorHeap_;
    UINT offsetInDsvHeap_;

};