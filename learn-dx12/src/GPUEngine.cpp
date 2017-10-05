#include <Rendering\GPUEngine.hpp>

GPUEngine::GPUEngine() = default;

GPUEngine::GPUEngine(ID3D12Device* device, GPU_ENGINE_TYPE type)
{
    std::size_t const commandQueueAllocatorsCount = [=] {
        switch (type)
        {
            case GPU_ENGINE_TYPE_DIRECT: return 3;
            case GPU_ENGINE_TYPE_COPY: return 1;
            case GPU_ENGINE_TYPE_COMPUTE: return 1;
            default: return 1;
        }
    } ();

    commandQueue_ = GPUCommandQueue { device, static_cast<D3D12_COMMAND_LIST_TYPE>(type), commandQueueAllocatorsCount };
    commandList_ = GPUCommandList{ device, static_cast<D3D12_COMMAND_LIST_TYPE>(type), commandQueue_.CurrentAlloc().Get() };

    GPUCommandAllocator& allocContext = commandQueue_.ProvideNextAlloc();
    commandList_.Reset(allocContext);
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
    commandList_.Close();
    commandList_.Execute(commandQueue_);

    GPUCommandAllocator& allocContext = commandQueue_.ProvideNextAlloc();
    commandList_.Reset(allocContext);
}
