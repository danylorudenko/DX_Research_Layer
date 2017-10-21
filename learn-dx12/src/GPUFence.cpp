#include <Rendering\GPUFence.hpp>
#include <Rendering\GPUCommandAllocator.hpp>

GPUFence::GPUFence() = default;

GPUFence::GPUFence(ID3D12Device* device)
{
    ThrowIfFailed(device->CreateFence(0U, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence_)));
    event_ = CreateEvent(nullptr, FALSE, FALSE, nullptr);
}

GPUFence::GPUFence(GPUFence&& rhs)
{
    fence_ = std::move(rhs.fence_);
    event_ = std::move(rhs.event_);

    ZeroMemory(&rhs, sizeof(rhs));
}

GPUFence& GPUFence::operator=(GPUFence&& rhs)
{
    fence_ = std::move(rhs.fence_);
    event_ = std::move(rhs.event_);

    ZeroMemory(&rhs, sizeof(rhs));
    return *this;
}

GPUFence::~GPUFence()
{
    CloseHandle(event_);
}

void GPUFence::WaitForValue(UINT64 value)
{
    if (CompletedValue() < value) {
        fence_->SetEventOnCompletion(value, event_);
        WaitForSingleObject(event_, INFINITE);
    }
}