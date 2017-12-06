#pragma once

#include <pch.hpp>

class GPUResourceFrameSet
{
public:
    GPUResourceFrameSet();
    GPUResourceFrameSet(int framesCount, ID3D12Device* device, std::size_t size, D3D12_RESOURCE_DESC* resourceDesc, D3D12_RESOURCE_STATES state);
    GPUResourceFrameSet(int framesCount, std::size_t resourceSize, Microsoft::WRL::ComPtr<ID3D12Resource>* resources, D3D12_RESOURCE_STATES state);
    GPUResourceFrameSet(GPUResourceFrameSet const&) = delete;
    GPUResourceFrameSet(GPUResourceFrameSet&& rhs);

    GPUResourceFrameSet& operator=(GPUResourceFrameSet const&) = delete;
    GPUResourceFrameSet& operator=(GPUResourceFrameSet&& rhs);
    
    ID3D12Resource* Get(int frameIndex) const { return resources_[frameIndex].Get(); }
    std::size_t Size() const { return size_; };
    std::size_t Capacity() const { return capacity_; }
    int FramesCount() const { return framesCount_; }

    D3D12_GPU_VIRTUAL_ADDRESS GPUVirtualAddress(int frameIndex);

    void CreateResources(int framesCount, ID3D12Device* device, std::size_t size, D3D12_RESOURCE_DESC* resourceDesc, D3D12_RESOURCE_STATES initialState);
    void UpdateData(int frameIndex, ID3D12GraphicsCommandList* commandList, std::size_t offsetInDest, GPUResourceFrameSet& src, int srcFrameIndex, std::size_t offsetInSrc, std::size_t numBytes);

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