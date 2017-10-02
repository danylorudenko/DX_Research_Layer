#include <Rendering\GPUEngine.hpp>

GPUEngine::GPUEngine(ID3D12Device* device, GPU_WORKER_TYPE type)
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

    commandQueue_.reset(new GPUCommandQueue { device, static_cast<D3D12_COMMAND_LIST_TYPE>(type), commandListAllocCount });
    commandList_.reset(new GPUCommandList{ device, static_cast<D3D12_COMMAND_LIST_TYPE>(type), commandQueue_->CurrentAlloc().Get() });
}

GPUEngine::GPUEngine(GPUEngine&& rhs)
{
    commandList_ = std::move(rhs.commandList_);
    commandQueue_ = std::move(rhs.commandQueue_);

    ZeroMemory(&rhs, sizeof(rhs));
}

GPUEngine& GPUEngine::operator=(GPUEngine&& rhs)
{
    commandList_ = std::move(rhs.commandList_);
    commandQueue_ = std::move(rhs.commandQueue_);

    ZeroMemory(&rhs, sizeof(rhs));

    return *this;
}

void GPUEngine::Reset()
{
    // Only finalized worker is allowed to be reset.
    if (finalized_) {
        auto& allocContext = commandQueue_->ProvideNextAlloc();
        commandList_->Reset(allocContext);
        finalized_ = false;
    }
}

void GPUEngine::Finalize()
{
    if (!finalized_) {
        commandList_->Close();
        commandList_->Execute(*commandQueue_);
        finalized_ = true;
    }
}