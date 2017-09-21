#include <Rendering\GPUFence.hpp>

GPUFence::GPUFence(ID3D12Device* device)
{
    ThrowIfFailed(device->CreateFence(0U, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence_)));
    event_ = CreateEvent(nullptr, FALSE, FALSE, nullptr);
}

GPUFence::GPUFence(GPUFence&& rhs)
{
    fence_ = rhs.fence_;
    event_ = rhs.event_;

    ZeroMemory(&rhs, sizeof(rhs));
}

GPUFence& GPUFence::operator=(GPUFence&& rhs)
{
    fence_ = rhs.fence_;
    event_ = rhs.event_;

    ZeroMemory(&rhs, sizeof(rhs));
    return *this;
}

GPUFence::~GPUFence()
{
    CloseHandle(event_);
}

void GPUFence::WaitForQueue(GPUCommandQueue& queue)
{
    if (CompletedValue() < queue.FenceTargetValue()) {
        fence_->SetEventOnCompletion(queue.FenceTargetValue(), event_);
        WaitForSingleObject(event_, INFINITY);
    }
}