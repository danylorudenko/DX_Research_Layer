#include <Rendering\Data\GPUFrameResourceView.hpp>

GPUFrameResourceView::GPUFrameResourceView() = default;

GPUFrameResourceView::GPUFrameResourceView(int frameCount, Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& heap,
                                                       UINT descriptorSize, int* offsetsInHeap,
                                                       D3D12_RESOURCE_STATES state, char const* semantics, GPUResourceSet* describedResource) :
    frameCount_(frameCount), descriptorHeap_(heap), descriptorSize_(descriptorSize), 
    state_(state), semantics_(semantics), describedResource_(describedResource)
{
    assert(frameCount == describedResource->FramesCount() && "Passed data count does not respond to the passed frame count.");

    for (int i = 0; i < frameCount_; i++) {
        descriptorOffsets_.push_back(offsetsInHeap[i]);
    }
}

GPUFrameResourceView::GPUFrameResourceView(GPUFrameResourceView const& rhs) = default;

GPUFrameResourceView::GPUFrameResourceView(GPUFrameResourceView&& rhs) = default;

GPUFrameResourceView& GPUFrameResourceView::operator=(GPUFrameResourceView const& rhs) = default;

GPUFrameResourceView& GPUFrameResourceView::operator=(GPUFrameResourceView&& rhs) = default;

D3D12_GPU_DESCRIPTOR_HANDLE GPUFrameResourceView::GPUViewHandle(int frameIndex) const
{
    assert(frameIndex < frameCount_ && "Handle getter recieved invalid frame index.");
    CD3DX12_GPU_DESCRIPTOR_HANDLE handle(descriptorHeap_->GetGPUDescriptorHandleForHeapStart(), descriptorOffsets_[frameIndex], descriptorSize_);
    return handle;
}

D3D12_CPU_DESCRIPTOR_HANDLE GPUFrameResourceView::CPUViewHandle(int frameIndex) const
{
    assert(frameIndex < frameCount_ && "Handle getter recieved invalid frame index.");
    CD3DX12_CPU_DESCRIPTOR_HANDLE handle(descriptorHeap_->GetCPUDescriptorHandleForHeapStart(), descriptorOffsets_[frameIndex], descriptorSize_);
    return handle;
}