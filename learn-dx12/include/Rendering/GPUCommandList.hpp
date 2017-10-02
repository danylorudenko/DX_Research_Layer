#pragma once

#include <pch.hpp>

#include <Rendering\GPUCommandAllocator.hpp>

class GPUCommandList
{
public:
    GPUCommandList(ID3D12Device* device, D3D12_COMMAND_LIST_TYPE type, ID3D12CommandAllocator* allocatorContext);
    GPUCommandList(const GPUCommandList&) = delete;
    GPUCommandList(GPUCommandList&& rhs);

    GPUCommandList& operator=(const GPUCommandList&) = delete;
    GPUCommandList& operator=(GPUCommandList&& rhs);

    ID3D12GraphicsCommandList* Get() const { return commandList_.Get(); }

    void Reset(GPUCommandAllocator& allocContext);
    void Execute(GPUCommandQueue& queueContext);
    void Close();
    bool Closed() { return closed_; }

private:
    Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList_ = nullptr;
    bool closed_ = false;
};