#include <Rendering\Data\GPUFrameResourceDescriptor.hpp>

GPUResourceFrameSetDescriptor::GPUResourceFrameSetDescriptor() = default;

GPUResourceFrameSetDescriptor::GPUResourceFrameSetDescriptor(int frameCount, Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& heap,
                                                       UINT descriptorSize, int* offsetsInHeap,
                                                       D3D12_RESOURCE_STATES state, char const* semantics, GPUResourceFrameSet* describedResource) :
    frameCount_(frameCount), descriptorHeap_(heap), descriptorSize_(descriptorSize), 
    state_(state), semantics_(semantics), describedResource_(describedResource)
{
    assert(frameCount == describedResource->FramesCount() && "Passed data count does not respond to the passed frame count.");

    for (int i = 0; i < frameCount_; i++) {
        descriptorOffsets_.push_back(offsetsInHeap[i]);
    }
}

GPUResourceFrameSetDescriptor::GPUResourceFrameSetDescriptor(GPUResourceFrameSetDescriptor const& rhs) = default;

GPUResourceFrameSetDescriptor::GPUResourceFrameSetDescriptor(GPUResourceFrameSetDescriptor&& rhs) = default;

GPUResourceFrameSetDescriptor& GPUResourceFrameSetDescriptor::operator=(GPUResourceFrameSetDescriptor const& rhs) = default;

GPUResourceFrameSetDescriptor& GPUResourceFrameSetDescriptor::operator=(GPUResourceFrameSetDescriptor&& rhs) = default;

D3D12_GPU_DESCRIPTOR_HANDLE GPUResourceFrameSetDescriptor::GPUViewHandle(int frameIndex) const
{
    assert(frameIndex < frameCount_ && "Handle getter recieved invalid frame index.");
    CD3DX12_GPU_DESCRIPTOR_HANDLE handle(descriptorHeap_->GetGPUDescriptorHandleForHeapStart(), descriptorOffsets_[frameIndex], descriptorSize_);
    return handle;
}

D3D12_CPU_DESCRIPTOR_HANDLE GPUResourceFrameSetDescriptor::CPUViewHandle(int frameIndex) const
{
    assert(frameIndex < frameCount_ && "Handle getter recieved invalid frame index.");
    CD3DX12_CPU_DESCRIPTOR_HANDLE handle(descriptorHeap_->GetCPUDescriptorHandleForHeapStart(), descriptorOffsets_[frameIndex], descriptorSize_);
    return handle;
}