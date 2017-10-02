#include <Rendering\GPUCommandList.hpp>
#include <Rendering\GPUCommandQueue.hpp>

GPUCommandList::GPUCommandList(ID3D12Device* device, D3D12_COMMAND_LIST_TYPE type, ID3D12CommandAllocator* allocatorContext)
{
    ThrowIfFailed(device->CreateCommandList(0, type, allocatorContext, nullptr, IID_PPV_ARGS(&commandList_)));
    Close();
}

GPUCommandList::GPUCommandList(GPUCommandList&& rhs)
{
    commandList_ = std::move(rhs.commandList_);

    ZeroMemory(&rhs, sizeof(rhs));
}

GPUCommandList& GPUCommandList::operator=(GPUCommandList&& rhs)
{
    commandList_ = std::move(rhs.commandList_);
    closed_ = rhs.closed_;

    ZeroMemory(&rhs, sizeof(rhs));
    return *this;
}

void GPUCommandList::Reset(GPUCommandAllocator& allocContext)
{
    allocContext.WaitForFence();
    allocContext.Reset();
    commandList_->Reset(allocContext.Get(), nullptr);
}

void GPUCommandList::Execute(GPUCommandQueue& queueContext)
{
    queueContext.ExecuteCommandLists(commandList_.Get(), 1);
}
