#include <Data\FrameResource.hpp>

GPUFrameResource::GPUFrameResource() = default;

GPUFrameResource::GPUFrameResource(ID3D12Device& device, WRL::ComPtr<ID3D12DescriptorHeap> sharedHeap, UINT offsetInHeap, WRL::ComPtr<ID3D12Resource> frameBuffer) :
    sharedRtvDescriptorHeap_(sharedHeap),
    offsetInRtvHeap_(offsetInHeap),
    frameBuffer_(frameBuffer)
{
    device.CreateRenderTargetView(frameBuffer_.Get(), nullptr, CPUDescriptorHandle());
}

GPUFrameResource::GPUFrameResource(GPUFrameResource&&) = default;

GPUFrameResource& GPUFrameResource::operator=(GPUFrameResource&&) = default;

D3D12_CPU_DESCRIPTOR_HANDLE GPUFrameResource::CPUDescriptorHandle() const
{
    auto heapHandle = sharedRtvDescriptorHeap_->GetCPUDescriptorHandleForHeapStart();
    heapHandle.ptr += offsetInRtvHeap_;

    return heapHandle;
}

D3D12_GPU_DESCRIPTOR_HANDLE GPUFrameResource::GPUDescriptorHandle() const
{
    auto heapHandle = sharedRtvDescriptorHeap_->GetGPUDescriptorHandleForHeapStart();
    heapHandle.ptr += offsetInRtvHeap_;

    return heapHandle;
}

ID3D12Resource* GPUFrameResource::FrameBuffer() const
{
    return frameBuffer_.Get();
}
