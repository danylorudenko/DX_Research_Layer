#include <Data\FrameResource.hpp>

FrameResource::FrameResource() = default;

FrameResource::FrameResource(
    ID3D12Device& device,
    WRL::ComPtr<ID3D12DescriptorHeap> sharedHeap,
    UINT offsetInHeap,
    WRL::ComPtr<ID3D12Resource> frameBuffer) :

    sharedRtvDescriptorHeap_(sharedHeap),
    offsetInRtvHeap_(offsetInHeap),
    frameBuffer_(frameBuffer),
    targetFenceValue_(0)
{
    auto result = device.CreateCommandAllocator(
        D3D12_COMMAND_LIST_TYPE_DIRECT,
        IID_PPV_ARGS(&commandAllocator_)
    );
    ThrowIfFailed(result);

    result = device.CreateFence(
        0,
        D3D12_FENCE_FLAG_NONE,
        IID_PPV_ARGS(&frameFence_)
    );
    ThrowIfFailed(result);

    fenceEvent_ = CreateEvent(nullptr, false, false, nullptr);
}

FrameResource::FrameResource(FrameResource&&) = default;

FrameResource& FrameResource::operator=(FrameResource&&) = default;

ID3D12CommandAllocator* FrameResource::CommandAllocator() const
{
    return commandAllocator_.Get();
}

D3D12_CPU_DESCRIPTOR_HANDLE FrameResource::CPUDescriptorHandle() const
{
    auto heapHandle = sharedRtvDescriptorHeap_->GetCPUDescriptorHandleForHeapStart();
    heapHandle.ptr += offsetInRtvHeap_;

    return heapHandle;
}

D3D12_GPU_DESCRIPTOR_HANDLE FrameResource::GPUDescriptorHandle() const
{
    auto heapHandle = sharedRtvDescriptorHeap_->GetGPUDescriptorHandleForHeapStart();
    heapHandle.ptr += offsetInRtvHeap_;

    return heapHandle;
}

ID3D12Resource* FrameResource::FrameBuffer() const
{
    return frameBuffer_.Get();
}

ID3D12Fence* FrameResource::Fence() const
{
    return frameFence_.Get();
}

void FrameResource::SetTargetFenceValue(UINT64 targetValue)
{
    targetFenceValue_ = targetValue;
}

UINT64 FrameResource::CompletedFenceValue() const
{
    return frameFence_->GetCompletedValue();
}

UINT64 FrameResource::TargetFenceValue() const
{
    return targetFenceValue_;
}

HANDLE FrameResource::FenceEvent() const
{
    return fenceEvent_;
}