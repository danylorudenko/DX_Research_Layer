#include <Rendering\GPUEngine.hpp>

GPUEngine::GPUEngine() = default;

GPUEngine::GPUEngine(ID3D12Device* device, GPU_ENGINE_TYPE type, UINT allocCount)
{
    commandQueue_ = GPUCommandQueue { device, static_cast<D3D12_COMMAND_LIST_TYPE>(type), allocCount };
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

void GPUEngine::Flush()
{
    commandList_.Close();
    commandList_.Execute(commandQueue_);
}

void GPUEngine::Reset()
{
    GPUCommandAllocator& allocContext = commandQueue_.ProvideNextAlloc();
    commandList_.Reset(allocContext);
}

void GPUEngine::FlushReset()
{
    Flush();
    Reset();
}
