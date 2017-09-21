#pragma once

#include <pch.hpp>

template<std::size_t COMMAND_ALLOCATOR_COUNT>
class GPUCommandList
{
public:
    GPUCommandList(ID3D12Device* device, D3D12_COMMAND_LIST_TYPE type);
    GPUCommandList(const GPUCommandList&) = delete;
    GPUCommandList(GPUCommandList&& rhs);

    GPUCommandList& operator=(const GPUCommandList&) = delete;
    GPUCommandList& operator=(GPUCommandList&& rhs);

    constexpr std::size_t AllocatorCount() const { return COMMAND_ALLOCATOR_COUNT; }

    void Begin();
    void End();

    ID3D12CommandList* Get() const { return commandList_.Get(); }
    ID3D12CommandList& Ref() { return *commandList_.Get(); }


private:
    Microsoft::WRL::ComPtr<ID3D12CommandList> commandList_ = nullptr;
    Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocators_[COMMAND_ALLOCATOR_COUNT];

    UINT64 currentAllocator = 0U;
};