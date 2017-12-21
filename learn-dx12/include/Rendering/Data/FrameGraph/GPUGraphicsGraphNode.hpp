#pragma once

#include <pch.hpp>

#include <Rendering\Data\FrameGraph\GPUGraphNode.hpp>
#include <Rendering\Data\GPURenderItem.hpp>

struct Color
{
    Color(float r, float g, float b, float a);
    float color[4];
};

class GPUGraphicsGraphNode : public GPUGraphNode
{
public:
    GPUGraphicsGraphNode();
    GPUGraphicsGraphNode(GPUEngine* engine, GPURootSignature* rootSignature, GPUPipelineState* pipelineState, int frameBufferCount);
    GPUGraphicsGraphNode(GPUGraphicsGraphNode const&) = delete;
    GPUGraphicsGraphNode(GPUGraphicsGraphNode&& rhs);

    GPUGraphicsGraphNode& operator=(GPUGraphicsGraphNode const&) = delete;
    GPUGraphicsGraphNode& operator=(GPUGraphicsGraphNode&& rhs);

    void ImportRenderTargets(std::vector<GPUFrameResourceDescriptor> const& renderTargets);
    void ImportRenderTargets(std::vector<GPUFrameResourceDescriptor>&& renderTargets);
    void ImportRenderTarget(GPUFrameResourceDescriptor const& renderTarget);
    void ImportDepthStencilTarget(GPUFrameResourceDescriptor depthStencilDescriptor);
    void ImportClearColors(Color const* clearColors, std::size_t colorCount);
    void ImportViewportScissor(D3D12_VIEWPORT const& viewport, D3D12_RECT const& scissorRect);

    void ImportRenderItem(GPURenderItem const& item);
    void ImportRenderItem(GPURenderItem&& item);

    virtual void Process(int frameIndex) override;

private:
    void BindRenderDepthStencilTargets(int frameIndex);
    void BindPipelineState();
    void BindViewportScissor();

    void TransitionRenderTargets(int frameIndex);
    void TransitionDepthStencilTarget(int frameIndex);

    void ClearRenderTargets(int frameIndex);
    void ClearDepthStencilTargets(int frameIndex);

    void IterateRenderItems(int frameIndex);
    void BindRenderItemRootResources(GPURenderItem& item, int frameIndex);
    void BindRenderItemVertexBuffer(GPURenderItem& item);
    void BindRenderItemIndexBuffer(GPURenderItem& item);
    void DrawCallRenderItem(GPURenderItem& item);

    std::vector<GPURenderItem> renderItems_;

    std::vector<GPUFrameResourceDescriptor> renderTargets_;
    std::vector<Color> clearColors_;

    GPUFrameResourceDescriptor depthStencilTarget_;

    D3D12_VIEWPORT viewportRect_;
    D3D12_RECT scissorRect_;

};