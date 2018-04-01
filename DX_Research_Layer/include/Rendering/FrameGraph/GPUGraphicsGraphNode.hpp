#pragma once

#include <pch.hpp>

#include <Rendering\FrameGraph\GPUGraphNode.hpp>
#include <Rendering\Data\GPURenderItem.hpp>
#include <Rendering\Resource\ResourceView\GPUResourceView.hpp>
#include <Rendering\Resource\GPUResourceHandle.hpp>
#include <Rendering\PipelineState\GPUDepthStencilSettings.hpp>

namespace DXRL
{

struct Color
{
    Color(float r, float g, float b, float a);
    float color[4];
};

class GPUGraphicsGraphNode : public GPUGraphNode
{
public:
    GPUGraphicsGraphNode();
    GPUGraphicsGraphNode(GPUEngine& engine, GPUPipelineState&& pipelineState, GPURootSignature&& rootSignature);
    GPUGraphicsGraphNode(GPUGraphicsGraphNode const&) = delete;
    GPUGraphicsGraphNode(GPUGraphicsGraphNode&& rhs);

    GPUGraphicsGraphNode& operator=(GPUGraphicsGraphNode const&) = delete;
    GPUGraphicsGraphNode& operator=(GPUGraphicsGraphNode&& rhs);

    void ImportRenderTargets(std::size_t count, GPUResourceViewHandle const* renderTargets);
    void ImportDepthStencilTarget(GPUResourceViewHandle const& depthStencilView);
    void ImportDepthStencilSettings(GPUDepthStencilSettings const& settings);
    void ImportClearColors(Color const* clearColors, std::size_t colorCount);
    void ImportViewportScissor(D3D12_VIEWPORT const& viewport, D3D12_RECT const& scissorRect);

    void ImportRenderItem(GPURenderItem const& item);
    void ImportRenderItem(GPURenderItem&& item);

    // TEMPORARY HACK
    Math::Transform& GetRenderItemTransform(std::size_t index);

    virtual void Process(std::size_t frameIndex) override;

private:
    void BindPassRoot(std::size_t frameIndex);
    void BindRenderDepthStencilTargets(std::size_t frameIndex);
    void BindPipelineState();
    void BindViewportScissor();

    void TransitionPassResources(std::size_t frameIndex);
    void TransitionRenderTargets(std::size_t frameIndex);

    void ClearRenderTargets(std::size_t frameIndex);
    void ClearDepthStencilTargets(std::size_t frameIndex);

    void IterateRenderItems(std::size_t frameIndex);
    void UpdateRenderItemTransform(GPURenderItem& item, std::size_t frameIndex);
    void BindRenderItemRootResources(GPURenderItem& item, std::size_t frameIndex);
    void BindRenderItemVertexBuffer(GPURenderItem& item);
    void BindRenderItemIndexBuffer(GPURenderItem& item);
    void DrawCallRenderItem(GPURenderItem& item);


    GPURootSignature rootSignature_;
    GPUPipelineState pipelineState_;

    std::vector<GPURenderItem> renderItems_;

    std::vector<GPUResourceViewHandle> renderTargets_;
    std::vector<Color> clearColors_;

    GPUResourceViewHandle depthStencilTarget_;
    GPUDepthStencilSettings depthStencilSettings_;

    D3D12_VIEWPORT viewportRect_;
    D3D12_RECT scissorRect_;

};

}