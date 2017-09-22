#include <Rendering\GPUCommandList.hpp>

GPUCommandList::GPUCommandList(ID3D12Device* device, D3D12_COMMAND_LIST_TYPE type, const std::size_t allocators) :
    allocatorCount_(allocators)
{
    for (size_t i = 0; i < allocatorCount_; i++) {
        commandAllocators_.emplace_back(device, type);
    }
    commandAllocators_.shrink_to_fit();

    ThrowIfFailed(device->CreateCommandList(0, type, commandAllocators_[currentAllocator_].Get(), nullptr, IID_PPV_ARGS(&commandList_)));
    Close();
}

GPUCommandList::GPUCommandList(GPUCommandList&& rhs) :
    allocatorCount_(rhs.allocatorCount_)
{
    commandAllocators_ = std::move(rhs.commandAllocators_);
    commandAllocators_.shrink_to_fit();

    commandList_ = std::move(rhs.commandList_);

    ZeroMemory(&rhs, sizeof(rhs));
}

GPUCommandList& GPUCommandList::operator=(GPUCommandList&& rhs)
{
    auto& AC = const_cast<std::size_t&>(allocatorCount_);
    AC = rhs.allocatorCount_;
    currentAllocator_ = std::move(rhs.currentAllocator_);
    commandAllocators_ = std::move(rhs.commandAllocators_);

    commandList_ = std::move(rhs.commandList_);

    closed_ = rhs.closed_;

    ZeroMemory(&rhs, sizeof(rhs));

    return *this;
}

void GPUCommandList::Reset(GPUCommandQueue& queueContext)
{
    currentAllocator_ = (currentAllocator_ + 1) % allocatorCount_;

    auto& allocator = commandAllocators_[currentAllocator_];
    allocator.Fence().WaitForQueue(queueContext, allocator);
    
    commandList_->Reset(allocator.Get(), nullptr);
    allocator.SetFenceTargetValue(allocator.FenceCompletedValue() + 1);
}