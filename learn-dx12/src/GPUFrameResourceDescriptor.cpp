#include <Rendering\Data\GPUFrameResourceDescriptor.hpp>

GPUFrameResourceDescriptor::GPUFrameResourceDescriptor() = default;

GPUFrameResourceDescriptor::GPUFrameResourceDescriptor(int frameCount, Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& heap,
                                                       int descriptorSize, int* offsetsInHeap,
                                                       D3D12_RESOURCE_STATES state, char const* semantics, std::vector<GPUResource*> describedResources) :
    frameCount_(frameCount), descriptorHeap_(heap),
    descriptorSize_(descriptorSize), state_(state), semantics_(semantics)
{
    assert(frameCount == describedResources.size() && "Passed data count does not respond to the passed frame count.");

    for (int i = 0; i < frameCount_; i++) {
        descriptorOffsets_.push_back(offsetsInHeap[i]);
    }

    for (int i = 0; i < frameCount_; i++) {
        describedResources_.push_back(describedResources[i]);
    }
}

GPUFrameResourceDescriptor::GPUFrameResourceDescriptor(GPUFrameResourceDescriptor const& rhs) = default;

GPUFrameResourceDescriptor::GPUFrameResourceDescriptor(GPUFrameResourceDescriptor&& rhs) = default;

GPUFrameResourceDescriptor& GPUFrameResourceDescriptor::operator=(GPUFrameResourceDescriptor const& rhs) = default;

GPUFrameResourceDescriptor& GPUFrameResourceDescriptor::operator=(GPUFrameResourceDescriptor&& rhs) = default;

D3D12_GPU_DESCRIPTOR_HANDLE GPUFrameResourceDescriptor::GPUViewHandle(int frameIndex) const
{
    assert(frameIndex < frameCount_ && "Handle getter recieved invalid frame index.");
    CD3DX12_GPU_DESCRIPTOR_HANDLE handle(descriptorHeap_->GetGPUDescriptorHandleForHeapStart(), descriptorOffsets_[frameIndex], descriptorSize_);
    return handle;
}

D3D12_CPU_DESCRIPTOR_HANDLE GPUFrameResourceDescriptor::CPUViewHandle(int frameIndex) const
{
    assert(frameIndex < frameCount_ && "Handle getter recieved invalid frame index.");
    CD3DX12_CPU_DESCRIPTOR_HANDLE handle(descriptorHeap_->GetCPUDescriptorHandleForHeapStart(), descriptorOffsets_[frameIndex], descriptorSize_);
    return handle;
}