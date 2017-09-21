#pragma once

#include <pch.hpp>

#include <Rendering\GPUFence.hpp>

class GPUCommandQueue
{
public:
    void ExecuteCommandLists(std::size_t count, ID3D12CommandList* commandList);
    void WaitForFence();

private:
    Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue_;

    GPUFence fence_;
};