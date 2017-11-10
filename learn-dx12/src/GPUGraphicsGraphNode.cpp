#include <Rendering\Data\FrameGraph\GPUGraphicsGraphNode.hpp>

GPUGraphicsGraphNode::GPUGraphicsGraphNode() = default;

GPUGraphicsGraphNode::GPUGraphicsGraphNode(GPUEngine* engine, GPURootSignature* rootSignature, GPUPipelineState* pipelineState) :
    GPUGraphNode(engine, rootSignature, pipelineState)
{ }

GPUGraphicsGraphNode::GPUGraphicsGraphNode(GPUGraphicsGraphNode&& rhs) :
    GPUGraphNode(std::move(rhs))
{ }

GPUGraphicsGraphNode& GPUGraphicsGraphNode::operator=(GPUGraphicsGraphNode&& rhs)
{
    GPUGraphNode::operator=(std::move(rhs));
    return *this;
}

void GPUGraphicsGraphNode::Process(UINT64 frameIndex)
{
    if (SynchronizeFrames(frameIndex)) {
        SetPassRootSignature();
        TransitionPassResources();

        IterateRenderItems();
    }
}

void GPUGraphicsGraphNode::IterateRenderItems()
{
    auto const itemCount = renderItems_.size();
    for (size_t i = 0; i < itemCount; i++) {
        auto item = renderItems_[i];
        BindRenderItemIndexBuffer(item);
        BindRenderItemVertexBuffer(item);
        BindRenderItemRootResources(item);
    }
}

void GPUGraphicsGraphNode::SetRenderTargets(std::vector<GPUFrameResourceDescriptor> const& renderTargets)
{
    renderTargets_ = renderTargets;
}

void GPUGraphicsGraphNode::SetRenderTargets(std::vector<GPUFrameResourceDescriptor>&& renderTargets)
{
    renderTargets_ = std::move(renderTargets);
}

void GPUGraphicsGraphNode::SetDepthStencilTarget(GPUFrameResourceDescriptor depthStencilDescriptor)
{
    depthStencilTarget_ = depthStencilDescriptor;
}

void GPUGraphicsGraphNode::BindRenderItemRootResources(GPURenderItem& item)
{
    auto const resCount = item.perItemResourceDescriptors_.size();
    for (size_t i = 0; i < resCount; i++) {
        auto resDesc = item.perItemResourceDescriptors_[i];
        executionEngine_->Commit().SetGraphicsRootDescriptorTable(resDesc.first, resDesc.second.GPUViewHandle(lastFrameIndex_));
    }
}

void GPUGraphicsGraphNode::BindRenderItemVertexBuffer(GPURenderItem& item)
{
    executionEngine_->Commit().IASetVertexBuffers(0, 1, &item.vertexBufferDescriptor_);
}

void GPUGraphicsGraphNode::BindRenderItemIndexBuffer(GPURenderItem& item)
{
    executionEngine_->Commit().IASetIndexBuffer(&item.indexBufferDescriptor_);
}