#include <Rendering\GPUCommandAllocator.hpp>

GPUCommandAllocator::GPUCommandAllocator(ID3D12Device* device, D3D12_COMMAND_LIST_TYPE type) :
    fence_(device)
{
    ThrowIfFailed(device->CreateCommandAllocator(type, IID_PPV_ARGS(&commandAllocator_)));
}

GPUCommandAllocator::GPUCommandAllocator(GPUCommandAllocator&& rhs) :
    fence_(std::move(rhs.fence_)),
    fenceTargetValue_(std::move(rhs.fenceTargetValue_)),
    commandAllocator_(std::move(rhs.commandAllocator_))
{
    ZeroMemory(&rhs, sizeof(rhs));
}

GPUCommandAllocator& GPUCommandAllocator::operator=(GPUCommandAllocator&& rhs)
{
    commandAllocator_ = std::move(rhs.commandAllocator_);
    fenceTargetValue_ = rhs.fenceTargetValue_;
    fence_ = std::move(rhs.fence_);

    ZeroMemory(&rhs, sizeof(rhs));

    return *this;
}

void GPUCommandAllocator::SendFenceGPUSignal(ID3D12CommandQueue* queue, UINT64 signalValue)
{
    ID3D12Fence* fence = fence_.Get();
    queue->Signal(fence, signalValue);
    SetFenceTargetValue(signalValue);
}

void GPUCommandAllocator::WaitForFence()
{
    Fence().WaitForValue(FenceTargetValue());
}
