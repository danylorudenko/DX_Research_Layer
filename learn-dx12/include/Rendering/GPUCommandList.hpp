#pragma once

#include <pch.hpp>

#include <memory>

class GPUCommandList
{
public:
    GPUCommandList(ID3D12Device* device, D3D12_COMMAND_LIST_TYPE type, const std::size_t allocators);
    GPUCommandList(const GPUCommandList&) = delete;
    GPUCommandList(GPUCommandList&& rhs);

    GPUCommandList& operator=(const GPUCommandList&) = delete;
    GPUCommandList& operator=(GPUCommandList&& rhs);

    ~GPUCommandList() { delete[] commandAllocators_; }

    INT AllocatorCount() const { return allocatorCount_; }

    ID3D12CommandList* Get() const { return commandList_.Get(); }
    ID3D12CommandList& Ref() { return *commandList_.Get(); }

    void Reset();
    void Close() { commandList_->Close(); }

private:
    Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList_ = nullptr;
    bool closed_ = false;

    Microsoft::WRL::ComPtr<ID3D12CommandAllocator>* commandAllocators_;
    UINT64 currentAllocator = 0U;
    INT allocatorCount_ = 0;
};