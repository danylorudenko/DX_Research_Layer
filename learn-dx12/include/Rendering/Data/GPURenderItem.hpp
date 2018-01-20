#pragma once

#include <pch.hpp>

struct GPURenderItem
{
    D3D12_VERTEX_BUFFER_VIEW vertexBufferDescriptor_;
    UINT vertexCount_;
    D3D_PRIMITIVE_TOPOLOGY primitiveTopology_;

    bool hasIndexBuffer_;
    D3D12_INDEX_BUFFER_VIEW indexBufferDescriptor_;
    UINT indexCount_;

    struct GPUDynamicRootArgument
    {
        std::size_t bindSlot_;
        GPUResourceViewTable itemTable_;
    };

    GPUDynamicRootArgument dynamicArg_;
};