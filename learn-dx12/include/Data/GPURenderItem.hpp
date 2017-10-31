#pragma once

#include <pch.hpp>

#include <Data\GPUFrameResourceDescriptor.hpp>

class GPURenderItem
{
public:
    GPUFrameResourceDescriptor vertexBufferDescriptor_;
    UINT vertexCount_;

    GPUFrameResourceDescriptor indexBufferDescriptor_;
    UINT indexCount_;

    using BindPointDescriptorPair = std::pair<UINT, GPUFrameResourceDescriptor>;
    std::vector<BindPointDescriptorPair> perItemResourceDescriptors_;
};