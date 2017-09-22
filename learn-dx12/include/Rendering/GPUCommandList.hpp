#pragma once

#include <pch.hpp>

#include <vector>

#include <Rendering\GPUCommandAllocator.hpp>

class GPUCommandList
{
public:
    GPUCommandList(ID3D12Device* device, D3D12_COMMAND_LIST_TYPE type, const std::size_t allocators);
    GPUCommandList(const GPUCommandList&) = delete;
    GPUCommandList(GPUCommandList&& rhs);

    GPUCommandList& operator=(const GPUCommandList&) = delete;
    GPUCommandList& operator=(GPUCommandList&& rhs);

    std::size_t AllocatorCount() const { return allocatorCount_; }

    ID3D12GraphicsCommandList* Get() const { return commandList_.Get(); }
    ID3D12CommandList& Ref() { return *commandList_.Get(); }

    void Reset(GPUCommandQueue& queueContext);
    void Close() { commandList_->Close(); closed_ = true; }
    bool Closed() { return closed_; }

private:
    Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList_ = nullptr;
    bool closed_ = false;

    std::vector<GPUCommandAllocator> commandAllocators_;
    UINT64 currentAllocator_ = 0U;
    std::size_t const allocatorCount_;
};