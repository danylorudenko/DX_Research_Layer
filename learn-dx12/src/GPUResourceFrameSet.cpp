#include <Rendering\Data\Resource\GPUResourceFrameSet.hpp>

GPUResourceFrameSet::GPUResourceFrameSet() = default;

GPUResourceFrameSet::GPUResourceFrameSet(int framesCount, GPUResource* resources)
{
    for (int i = 0; i < framesCount; i++) {
        resources_.push_back(std::move(resources[i]));
    }
}

GPUResourceFrameSet::GPUResourceFrameSet(GPUResourceFrameSet&& rhs) = default;

GPUResourceFrameSet& GPUResourceFrameSet::operator=(GPUResourceFrameSet&& rhs) = default;

GPUResource& GPUResourceFrameSet::operator[](int const resourceIndex)
{
    return resources_[resourceIndex];
}