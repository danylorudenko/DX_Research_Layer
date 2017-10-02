#include <Rendering\GPUCommandQueue.hpp>

GPUCommandQueue::GPUCommandQueue() = default;

GPUCommandQueue::GPUCommandQueue(ID3D12Device* device, D3D12_COMMAND_LIST_TYPE type, std::size_t allocatorCount)
{
    D3D12_COMMAND_QUEUE_DESC queueDesc;
    queueDesc.Type = type;
    queueDesc.NodeMask = 0;
    queueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;

    ThrowIfFailed(device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&commandQueue_)));

    // Create backing allocators
    for (size_t i = 0; i < allocatorCount; i++)
    {
        commandAllocators_.emplace_back(device, type);
        //commandAllocators_.back().Reset();
    }
}

GPUCommandQueue::GPUCommandQueue(GPUCommandQueue&& rhs)
{
    commandQueue_ = std::move(rhs.commandQueue_);
    commandAllocators_ = std::move(rhs.commandAllocators_);

    ZeroMemory(&rhs, sizeof(rhs));
}

GPUCommandQueue& GPUCommandQueue::operator=(GPUCommandQueue&& rhs)
{
    commandQueue_ = std::move(rhs.commandQueue_);
    commandAllocators_ = std::move(rhs.commandAllocators_);

    ZeroMemory(&rhs, sizeof(rhs));
    return *this;
}

void GPUCommandQueue::ExecuteCommandLists(ID3D12CommandList* commandLists, std::size_t count)
{
    ID3D12CommandList* const lists[] = { commandLists };
    commandQueue_->ExecuteCommandLists(static_cast<UINT>(count), lists);
    
    GPUCommandAllocator& currentAlloc = commandAllocators_[currentAllocator_];
    currentAlloc.SetFenceTargetValue(currentAlloc.FenceCompletedValue() + 1);
    currentAlloc.SendFenceGPUSignal(this->Get(), currentAlloc.FenceTargetValue());
}

GPUCommandAllocator& GPUCommandQueue::CurrentAlloc()
{
    return commandAllocators_[currentAllocator_];
}

GPUCommandAllocator& GPUCommandQueue::ProvideNextAlloc()
{
    currentAllocator_ = (currentAllocator_ + 1) % commandAllocators_.size();
    return commandAllocators_[currentAllocator_];
}