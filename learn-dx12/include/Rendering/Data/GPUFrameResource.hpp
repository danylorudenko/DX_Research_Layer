#pragma once

#include <pch.hpp>

class GPUFrameResource
{
public:
    GPUFrameResource();
    GPUFrameResource(int framesCount, ID3D12Device* device, std::size_t size, D3D12_RESOURCE_DESC* resourceDesc, D3D12_RESOURCE_STATES state = D3D12_RESOURCE_STATE_COMMON);
    GPUFrameResource(int framesCount, std::size_t resourceSize, std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> resources, D3D12_RESOURCE_STATES state = D3D12_RESOURCE_STATE_COMMON);
    GPUFrameResource(GPUFrameResource const&) = delete;
    GPUFrameResource(GPUFrameResource&& rhs);

    GPUFrameResource& operator=(GPUFrameResource const&) = delete;
    GPUFrameResource& operator=(GPUFrameResource&& rhs);
    
    ID3D12Resource* Get(int frameIndex) const { return resources_[frameIndex].Get(); }
    std::size_t Size() const { return size_; };
    std::size_t Capacity() const { return capacity_; }
    int FramesCount() const { return framesCount_; }

    D3D12_GPU_VIRTUAL_ADDRESS GPUVirtualAddress(int frameIndex) const { return gpuAddresses_[frameIndex]; }

    void CreateResources(int framesCount, ID3D12Device* device, std::size_t size, D3D12_RESOURCE_DESC* resourceDesc, D3D12_RESOURCE_STATES initialState = D3D12_RESOURCE_STATE_COMMON);
    void UpdateData(int frameIndex, ID3D12GraphicsCommandList* commandList, std::size_t offsetInDest, GPUFrameResource& src, int srcFrameIndex, std::size_t offsetInSrc, std::size_t numBytes);

    void Transition(int frameIndex, ID3D12GraphicsCommandList* commandList, D3D12_RESOURCE_STATES state);
    D3D12_RESOURCE_STATES State(int frameIndex) const;

protected:
    int framesCount_ = 0;
    std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> resources_;
    std::vector<D3D12_GPU_VIRTUAL_ADDRESS> gpuAddresses_;
    std::vector<D3D12_RESOURCE_STATES> states_;
    std::size_t size_ = 0;
    std::size_t capacity_ = 0;
};