#include <Rendering\GPUCommandQueue.hpp>

GPUCommandQueue::GPUCommandQueue(ID3D12Device* device, D3D12_COMMAND_LIST_TYPE type)
{
    D3D12_COMMAND_QUEUE_DESC queueDesc;
    queueDesc.Type = type;
    queueDesc.NodeMask = 0;
    queueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;

    ThrowIfFailed(device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&commandQueue_)));
}

GPUCommandQueue::GPUCommandQueue(GPUCommandQueue&& rhs)
{
    commandQueue_ = std::move(rhs.commandQueue_);

    ZeroMemory(&rhs, sizeof(rhs));
}

GPUCommandQueue& GPUCommandQueue::operator=(GPUCommandQueue&& rhs)
{
    commandQueue_ = std::move(rhs.commandQueue_);

    ZeroMemory(&rhs, sizeof(rhs));
    return *this;
}

void GPUCommandQueue::ExecuteCommandLists(ID3D12CommandList* commandLists, std::size_t count)
{
    ID3D12CommandList* const lists[] = { commandLists };
    commandQueue_->ExecuteCommandLists(count, lists);
}