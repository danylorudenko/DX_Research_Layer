#pragma once

#include <pch.hpp>

#include <Rendering\GPUCommandAllocator.hpp>
#include <Rendering\GPUCommandQueue.hpp>

namespace DXRL
{

class GPUCommandList
{
public:
    GPUCommandList();
    GPUCommandList(ID3D12Device* device, D3D12_COMMAND_LIST_TYPE type, std::size_t allocatorCount);
    GPUCommandList(GPUCommandList const&) = delete;
    GPUCommandList(GPUCommandList&& rhs);

    GPUCommandList& operator=(GPUCommandList const&) = delete;
    GPUCommandList& operator=(GPUCommandList&& rhs);

    ID3D12GraphicsCommandList* Get() const { return commandList_.Get(); }

    GPUCommandAllocator& CurrentAlloc();
    GPUCommandAllocator& ProvideNextAlloc();

    void Reset();
    void Execute(GPUCommandQueue& queueContext);
    void Close();

private:
    Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList_ = nullptr;

    std::vector<GPUCommandAllocator> commandAllocators_;
    int currentAllocator_ = 0ULL;
};

}