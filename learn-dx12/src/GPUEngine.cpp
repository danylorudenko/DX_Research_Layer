#include <Rendering\GPUEngine.hpp>

GPUEngine::GPUEngine() = default;

GPUEngine::GPUEngine(ID3D12Device* device, GPU_ENGINE_TYPE type, UINT allocCount)
{
    commandQueue_ = GPUCommandQueue { device, static_cast<D3D12_COMMAND_LIST_TYPE>(type) };
    commandList_ = GPUCommandList { device, static_cast<D3D12_COMMAND_LIST_TYPE>(type), allocCount };
    fence_ = GPUFence{ device };

    Reset();
}

GPUEngine::GPUEngine(GPUEngine&& rhs)
{
    commandList_ = std::move(rhs.commandList_);
    commandQueue_ = std::move(rhs.commandQueue_);
    fence_ = std::move(rhs.fence_);
}

GPUEngine& GPUEngine::operator=(GPUEngine&& rhs)
{
    commandList_ = std::move(rhs.commandList_);
    commandQueue_ = std::move(rhs.commandQueue_);
    fence_ = std::move(rhs.fence_);

    return *this;
}

void GPUEngine::Flush()
{
    commandList_.Close();
    commandList_.Execute(commandQueue_);
    SendFenceGPUSignal(commandList_.CurrentAlloc().FenceTargetValue());
}

void GPUEngine::Reset()
{
    WaitForFenceValue(commandList_.CurrentAlloc().FenceTargetValue());
    commandList_.Reset();
}

void GPUEngine::FlushReset()
{
    Flush();
    Reset();
}

void GPUEngine::SendFenceGPUSignal(UINT64 signalValue)
{
    commandQueue_.Get()->Signal(fence_.Get(), signalValue);
}

void GPUEngine::WaitForFenceValue(UINT64 value)
{
    fence_.WaitForValue(value);
}
