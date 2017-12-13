#pragma once

#include <pch.hpp>
#include <Rendering\Data\Resource\GPUResource.hpp>

class GPUResourceFrameSet
{
public:
    GPUResourceFrameSet();

    GPUResourceFrameSet(int framesCount, GPUResource* resources);
    GPUResourceFrameSet(GPUResourceFrameSet const&) = delete;
    GPUResourceFrameSet(GPUResourceFrameSet&& rhs);

    GPUResourceFrameSet& operator=(GPUResourceFrameSet const&) = delete;
    GPUResourceFrameSet& operator=(GPUResourceFrameSet&& rhs);
    
    std::size_t FramesCount() const;

    GPUResource& operator[](int const resourceIndex);

protected:
    std::vector<GPUResource*> resources_;
};