#pragma once

#include <pch.hpp>

#include <Rendering\Data\FrameGraph\GPUGraphNode.hpp>
#include <Rendering\Data\GPURenderItem.hpp>

class GPUGraphicsGraphNode : public GPUGraphNode
{
public:
    GPUGraphicsGraphNode();
    GPUGraphicsGraphNode(GPUEngine* engine, GPURootSignature* rootSignature, GPUPipelineState* pipelineState, int frameBufferCount);
    GPUGraphicsGraphNode(GPUGraphicsGraphNode const&) = delete;
    GPUGraphicsGraphNode(GPUGraphicsGraphNode&& rhs);

    GPUGraphicsGraphNode& operator=(GPUGraphicsGraphNode const&) = delete;
    GPUGraphicsGraphNode& operator=(GPUGraphicsGraphNode&& rhs);

    void ImportRenderTargets(std::vector<GPUResourceFrameSetDescriptor> const& renderTargets);
    void ImportRenderTargets(std::vector<GPUResourceFrameSetDescriptor>&& renderTargets);
    void ImportRenderTarget(GPUResourceFrameSetDescriptor const& renderTarget);
    void ImportDepthStencilTarget(GPUResourceFrameSetDescriptor depthStencilDescriptor);

    void ImportRenderItem(GPURenderItem const& item);
    void ImportRenderItem(GPURenderItem&& item);

    virtual void Process(UINT64 frameIndex) override;

private:
    void BindRenderDepthStencilTargets(int frameIndex);
    void BindPipelineState();

    void TransitionRenderTargets(int frameIndex);
    void TransitionDepthStencilTarget(int frameIndex);

    void IterateRenderItems(int frameIndex);
    void BindRenderItemRootResources(GPURenderItem& item, int frameIndex);
    void BindRenderItemVertexBuffer(GPURenderItem& item);
    void BindRenderItemIndexBuffer(GPURenderItem& item);
    void DrawCallRenderItem(GPURenderItem& item);

    std::vector<GPURenderItem> renderItems_;

    std::vector<GPUResourceFrameSetDescriptor> renderTargets_;
    GPUResourceFrameSetDescriptor depthStencilTarget_;
};