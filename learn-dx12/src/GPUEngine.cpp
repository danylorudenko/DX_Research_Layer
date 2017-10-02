#include <Rendering\GPUEngine.hpp>

GPUEngine::GPUEngine() = default;

GPUEngine::GPUEngine(ID3D12Device* device, GPU_ENGINE_TYPE type)
{
    std::size_t commandQueueAllocatorsCount = 0;
    switch (type)
    {
    case GPU_ENGINE_TYPE_DIRECT:
        commandQueueAllocatorsCount = 3;
        break;
    case GPU_ENGINE_TYPE_COPY:
        commandQueueAllocatorsCount = 1;
        break;
    case GPU_ENGINE_TYPE_COMPUTE:
        commandQueueAllocatorsCount = 1;
        break;
    }

    commandQueue_ = GPUCommandQueue { device, static_cast<D3D12_COMMAND_LIST_TYPE>(type), commandQueueAllocatorsCount };
    commandList_ = GPUCommandList{ device, static_cast<D3D12_COMMAND_LIST_TYPE>(type), commandQueue_.CurrentAlloc().Get() };
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
        GPUCommandAllocator& allocContext = commandQueue_.ProvideNextAlloc();
        commandList_.Reset(allocContext);
        finalized_ = false;
    }
}

void GPUEngine::Finalize()
{
    if (!finalized_) {
        commandList_.Close();
        commandList_.Execute(commandQueue_);
        finalized_ = true;
    }
}