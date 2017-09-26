#include <Rendering\GPUWorker.hpp>

GPUWorker::GPUWorker(ID3D12Device* device, GPU_WORKER_TYPE type)
{
    std::size_t commandListAllocCount = 0;
    switch (type)
    {
    case GPU_WORKER_TYPE_DIRECT:
        commandListAllocCount = 3;
        break;
    case GPU_WORKER_TYPE_COPY:
        commandListAllocCount = 1;
        break;
    case GPU_WORKER_TYPE_COMPUTE:
        commandListAllocCount = 1;
        break;
    }

    commandList_.reset(new GPUCommandList{ device, static_cast<D3D12_COMMAND_LIST_TYPE>(type), commandListAllocCount });
    commandQueue_.reset(new GPUCommandQueue { device, static_cast<D3D12_COMMAND_LIST_TYPE>(type) });
}

GPUWorker::GPUWorker(GPUWorker&& rhs)
{
    commandList_ = std::move(rhs.commandList_);
    commandQueue_ = std::move(rhs.commandQueue_);

    ZeroMemory(&rhs, sizeof(rhs));
}

GPUWorker& GPUWorker::operator=(GPUWorker&& rhs)
{
    commandList_ = std::move(rhs.commandList_);
    commandQueue_ = std::move(rhs.commandQueue_);

    ZeroMemory(&rhs, sizeof(rhs));

    return *this;
}

void GPUWorker::Reset()
{
    // Only finalized worker is allowed to be reset.
    if (finalized_) {
        commandList_->Reset(*commandQueue_);
        finalized_ = false;
    }
}

void GPUWorker::Finalize()
{
    if (!finalized_) {
        commandList_->Close();
        commandQueue_->ExecuteCommandLists(commandList_->Get(), 1);
        finalized_ = true;
    }
}