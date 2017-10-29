#include <Data\GPUFrameResourceDescriptor.hpp>

GPUFrameResourceDescriptor::GPUFrameResourceDescriptor() = default;

GPUFrameResourceDescriptor::GPUFrameResourceDescriptor(UINT frameCount, Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& heap,
                                                       int descriptorSize, int* offsetInHeap,
                                                       D3D12_RESOURCE_STATES state, char const* semantics, GPUResource** resource) :
    frameCount_(frameCount), descriptorHeap_(heap),
    descriptorSize_(descriptorSize), state_(state), semantics_(semantics)
{
    descriptorOffsets_ = new int[frameCount_];
    for (int i = 0; i < frameCount_; i++) {
        descriptorOffsets_[i] = offsetInHeap[i];
    }

    describedResources_ = new GPUResource*[frameCount_];
    for (int i = 0; i < frameCount_; i++) {
        describedResources_[i] = resource[i];
    }
}

GPUFrameResourceDescriptor::GPUFrameResourceDescriptor(GPUFrameResourceDescriptor const& rhs) :
    frameCount_(rhs.frameCount_), descriptorHeap_(rhs.descriptorHeap_),
    descriptorSize_(rhs.descriptorSize_), state_(rhs.state_), semantics_(rhs.semantics_)
{
    descriptorOffsets_ = new int[frameCount_];
    for (int i = 0; i < frameCount_; i++) {
        descriptorOffsets_[i] = rhs.descriptorOffsets_[i];
    }

    /////////////////////////////////////////////////////////////////////////

    describedResources_ = new GPUResource*[frameCount_];
    for (int i = 0; i < frameCount_; i++) {
        describedResources_[i] = rhs.describedResources_[i];
    }
}

GPUFrameResourceDescriptor::GPUFrameResourceDescriptor(GPUFrameResourceDescriptor&& rhs) :
    frameCount_(std::move(rhs.frameCount_)), descriptorHeap_(std::move(rhs.descriptorHeap_)),
    descriptorSize_(std::move(rhs.descriptorSize_)), state_(std::move(rhs.state_)), semantics_(std::move(rhs.semantics_))
{
    descriptorOffsets_ = rhs.descriptorOffsets_;
    rhs.descriptorOffsets_ = nullptr;

    /////////////////////////////////////////////////////////////////////////
    
    describedResources_ = rhs.describedResources_;
    rhs.describedResources_ = nullptr;
}

GPUFrameResourceDescriptor& GPUFrameResourceDescriptor::operator=(GPUFrameResourceDescriptor const& rhs)
{
    frameCount_ = rhs.frameCount_;
    descriptorHeap_ = rhs.descriptorHeap_;
    descriptorSize_ = rhs.descriptorSize_;
    state_ = rhs.state_;
    semantics_ = rhs.semantics_;

    for (int i = 0; i < frameCount_; i++) {
        descriptorOffsets_[i] = rhs.descriptorOffsets_[i];
    }
    /////////////////////////////////////////////////////////////////////////
    
    for (int i = 0; i < frameCount_; i++) {
        describedResources_[i] = rhs.describedResources_[i];
    }
    
    return *this;
}

GPUFrameResourceDescriptor& GPUFrameResourceDescriptor::operator=(GPUFrameResourceDescriptor&& rhs)
{
    frameCount_ = std::move(rhs.frameCount_);
    descriptorHeap_ = std::move(rhs.descriptorHeap_);
    descriptorSize_ = std::move(rhs.descriptorSize_);
    state_ = std::move(rhs.state_);
    semantics_ = std::move(rhs.semantics_);

    descriptorOffsets_ = rhs.descriptorOffsets_;
    rhs.descriptorOffsets_ = nullptr;

    describedResources_ = rhs.describedResources_;
    rhs.describedResources_ = nullptr;

    return *this;
}

D3D12_GPU_DESCRIPTOR_HANDLE GPUFrameResourceDescriptor::GPUViewHandle(UINT frameIndex) const
{
    CD3DX12_GPU_DESCRIPTOR_HANDLE handle(descriptorHeap_->GetGPUDescriptorHandleForHeapStart(), descriptorOffsets_[frameIndex], descriptorSize_);
    return handle;
}

D3D12_CPU_DESCRIPTOR_HANDLE GPUFrameResourceDescriptor::CPUViewHandle(UINT frameIndex) const
{
    CD3DX12_CPU_DESCRIPTOR_HANDLE handle(descriptorHeap_->GetCPUDescriptorHandleForHeapStart(), descriptorOffsets_[frameIndex], descriptorSize_);
    return handle;
}