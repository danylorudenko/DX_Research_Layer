#pragma once

#include <pch.hpp>

#include <Math\Transform.hpp>
#include <Rendering\Data\Resource\GPUResourceHandle.hpp>

namespace DXRL
{

class GPURenderItem
{
public:
    GPURenderItem();
    GPURenderItem(GPURenderItem const& rhs);
    GPURenderItem(GPURenderItem&& rhs);

    GPURenderItem& operator=(GPURenderItem const& rhs);
    GPURenderItem& operator=(GPURenderItem&& rhs);

    void InsertVertexView(std::size_t vertexCount, GPUResourceHandle const& vertexBuffer, D3D12_VERTEX_BUFFER_VIEW const& view, D3D_PRIMITIVE_TOPOLOGY topology);
    void InsertIndexView(std::size_t indexCount, GPUResourceHandle const& indexBuffer, D3D12_INDEX_BUFFER_VIEW const& view);

    //Transform& Transform();

private:
    Transform transform_;
    Transform const* parent_ = nullptr;

    D3D_PRIMITIVE_TOPOLOGY primitiveTopology_;

    GPUResourceHandle vertexBuffer_;
    GPUResourceHandle indexBuffer_;

    D3D12_VERTEX_BUFFER_VIEW vertexBufferDescriptor_;
    D3D12_INDEX_BUFFER_VIEW indexBufferDescriptor_;

    UINT vertexCount_ = 0;
    UINT indexCount_ = 0;

    struct GPUDynamicRootArgument
    {
        std::size_t bindSlot_;
        GPUResourceViewTable itemTable_;
    };

    GPUDynamicRootArgument dynamicArg_;
};

}