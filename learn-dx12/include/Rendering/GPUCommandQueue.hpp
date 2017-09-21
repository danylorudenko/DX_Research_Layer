#pragma once

#include <pch.hpp>

#include <Rendering\GPUFence.hpp>

class GPUCommandQueue
{
public:
    GPUCommandQueue(ID3D12Device* device, D3D12_COMMAND_LIST_TYPE type);
    GPUCommandQueue(const GPUCommandQueue&) = delete;
    GPUCommandQueue(GPUCommandQueue&& rhs);

    GPUCommandQueue& operator=(const GPUCommandQueue&) = delete;
    GPUCommandQueue& operator=(GPUCommandQueue&& rhs);


    ID3D12CommandQueue* Get() { return commandQueue_.Get(); }

    void ExecuteCommandLists(ID3D12GraphicsCommandList* commandLists, std::size_t count = 1);

private:
    Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue_ = nullptr;
};