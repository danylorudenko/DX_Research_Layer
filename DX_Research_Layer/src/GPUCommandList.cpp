#include <pch.hpp>

#include <Rendering\GPUEngine\GPUCommandList.hpp>
#include <Rendering\GPUEngine\GPUCommandQueue.hpp>

namespace DXRL
{

GPUCommandList::GPUCommandList() = default;

GPUCommandList::GPUCommandList(ID3D12Device* device, D3D12_COMMAND_LIST_TYPE type, std::size_t allocatorCount)
{
    // Create backing allocators
    for (size_t i = 0; i < allocatorCount; i++) {
        commandAllocators_.push_back(GPUCommandAllocator{ device, type });
    }

    device->CreateCommandList(0, type, CurrentAlloc().Get(), nullptr, IID_PPV_ARGS(&commandList_));
    Close();
}

GPUCommandList::GPUCommandList(GPUCommandList&& rhs)
{
    commandList_ = std::move(rhs.commandList_);
    commandAllocators_ = std::move(rhs.commandAllocators_);
    currentAllocator_ = rhs.currentAllocator_;
}

GPUCommandList& GPUCommandList::operator=(GPUCommandList&& rhs)
{
    commandList_ = std::move(rhs.commandList_);
    commandAllocators_ = std::move(rhs.commandAllocators_);
    currentAllocator_ = rhs.currentAllocator_;

    return *this;
}

GPUCommandAllocator& GPUCommandList::CurrentAlloc()
{
    return commandAllocators_[currentAllocator_];
}

GPUCommandAllocator& GPUCommandList::ProvideNextAlloc()
{
    currentAllocator_ = (currentAllocator_ + 1) % commandAllocators_.size();
    return commandAllocators_[currentAllocator_];
}

void GPUCommandList::Reset()
{
    auto& allocContext = ProvideNextAlloc();
    allocContext.Reset();
    commandList_->Reset(allocContext.Get(), nullptr);
}

void GPUCommandList::Execute(GPUCommandQueue& queueContext)
{
    queueContext.ExecuteCommandLists(commandList_.Get(), 1);
    CurrentAlloc().SetFenceTargetValue(CurrentAlloc().FenceTargetValue() + 1);
}

void GPUCommandList::Close()
{
    commandList_->Close();
}

}