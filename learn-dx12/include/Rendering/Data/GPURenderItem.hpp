#pragma once

#include <pch.hpp>

#include <Rendering\Data\GPUFrameResourceDescriptor.hpp>

class GPURenderItem
{
public:
    D3D12_VERTEX_BUFFER_VIEW vertexBufferDescriptor_;
    UINT vertexCount_;
    D3D_PRIMITIVE_TOPOLOGY primitiveTopology_;

    bool hasIndexBuffer_;
    D3D12_INDEX_BUFFER_VIEW indexBufferDescriptor_;
    UINT indexCount_;

    using BindPointDescriptorPair = std::pair<UINT, GPUFrameResourceDescriptor>;
    std::vector<BindPointDescriptorPair> perItemResourceDescriptors_;
};