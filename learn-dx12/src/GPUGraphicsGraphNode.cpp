#include <Rendering\Data\FrameGraph\GPUGraphicsGraphNode.hpp>

GPUGraphicsGraphNode::GPUGraphicsGraphNode() = default;

GPUGraphicsGraphNode::GPUGraphicsGraphNode(GPUEngine* engine, GPURootSignature* rootSignature, GPUPipelineState* pipelineState, int frameBufferCount) :
    GPUGraphNode(engine, rootSignature, pipelineState, frameBufferCount)
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
    int const frameIndexLocal = frameIndex % frameBufferCount_;
    
    BindPipelineState();
    BindPassRootSignature(frameIndexLocal);

    TransitionRenderTargets(frameIndexLocal);
    TransitionDepthStencilTarget(frameIndexLocal);
    TransitionPassResources(frameIndexLocal);

    BindRenderDepthStencilTargets(frameIndexLocal);

    IterateRenderItems(frameIndexLocal);
}

void GPUGraphicsGraphNode::IterateRenderItems(int frameIndex)
{
    int const itemCount = static_cast<int>(renderItems_.size());
    for (int i = 0; i < itemCount; i++) {
        auto item = renderItems_[i];
        BindRenderItemVertexBuffer(item);
        BindRenderItemIndexBuffer(item);
        BindRenderItemRootResources(item, frameIndex);
        DrawCallRenderItem(item);
    }
}

void GPUGraphicsGraphNode::ImportRenderTargets(std::vector<GPUResourceFrameSetDescriptor> const& renderTargets)
{
    renderTargets_ = renderTargets;
}

void GPUGraphicsGraphNode::ImportRenderTargets(std::vector<GPUResourceFrameSetDescriptor>&& renderTargets)
{
    renderTargets_ = std::move(renderTargets);
}

void GPUGraphicsGraphNode::ImportRenderTarget(GPUResourceFrameSetDescriptor const& renderTarget)
{
    renderTargets_.push_back(renderTarget);
}

void GPUGraphicsGraphNode::ImportDepthStencilTarget(GPUResourceFrameSetDescriptor depthStencilDescriptor)
{
    depthStencilTarget_ = depthStencilDescriptor;
}

void GPUGraphicsGraphNode::ImportRenderItem(GPURenderItem const& renderItem)
{
    renderItems_.push_back(renderItem);
}

void GPUGraphicsGraphNode::ImportRenderItem(GPURenderItem&& renderItem)
{
    renderItems_.push_back(std::move(renderItem));
}

void GPUGraphicsGraphNode::BindRenderDepthStencilTargets(int frameIndex)
{    
    auto const renderTargetsCount = static_cast<int>(renderTargets_.size());
    assert(renderTargetsCount <= 5 && "More than 5 render targets is not currently supported.");

    D3D12_CPU_DESCRIPTOR_HANDLE renderTargetHandles[5];
    for (int i = 0; i < renderTargetsCount; i++) {
        renderTargetHandles[i] = renderTargets_[i].CPUViewHandle(frameIndex);
    }

    D3D12_CPU_DESCRIPTOR_HANDLE depthStencilHandle = depthStencilTarget_.CPUViewHandle(frameIndex);
    auto* depthStencilHandlePtr = depthStencilTarget_.IsValid() ? &depthStencilHandle : nullptr;
    
    if (renderTargetsCount > 0) {
        executionEngine_->Commit().OMSetRenderTargets(renderTargetsCount, renderTargetHandles, false, depthStencilHandlePtr);
    }
}

void GPUGraphicsGraphNode::BindPipelineState()
{
    executionEngine_->Commit().SetPipelineState(pipelineState_->Get());
}

void GPUGraphicsGraphNode::TransitionRenderTargets(int frameIndex)
{
    auto const renderTargetsCount = static_cast<int>(renderTargets_.size());
    assert(renderTargetsCount <= 5 && "More that 5 render targets is not currently supported.");
    
    for (int i = 0; i < renderTargetsCount; i++) {
        if (renderTargets_[i].DescribedResource()->State(frameIndex) != D3D12_RESOURCE_STATE_RENDER_TARGET) {
            renderTargets_[i].DescribedResource()->Transition(frameIndex, executionEngine_->CommandList(), D3D12_RESOURCE_STATE_RENDER_TARGET);
        }
    }
}

void GPUGraphicsGraphNode::TransitionDepthStencilTarget(int frameIndex)
{
    if (depthStencilTarget_.DescribedResource()->State(frameIndex) != D3D12_RESOURCE_STATE_DEPTH_WRITE) {
        depthStencilTarget_.DescribedResource()->Transition(frameIndex, executionEngine_->CommandList(), D3D12_RESOURCE_STATE_DEPTH_WRITE);
    }
}

void GPUGraphicsGraphNode::BindRenderItemRootResources(GPURenderItem& item, int frameIndex)
{
    int const resCount = static_cast<int>(item.perItemResourceDescriptors_.size());
    for (int i = 0; i < resCount; i++) {
        auto const& resDesc = item.perItemResourceDescriptors_[i];
        executionEngine_->Commit().SetGraphicsRootDescriptorTable(resDesc.first, resDesc.second.GPUViewHandle(frameIndex));
    }
}

void GPUGraphicsGraphNode::BindRenderItemVertexBuffer(GPURenderItem& item)
{
    executionEngine_->Commit().IASetPrimitiveTopology(item.primitiveTopology_);
    executionEngine_->Commit().IASetVertexBuffers(0, 1, &item.vertexBufferDescriptor_);
}

void GPUGraphicsGraphNode::BindRenderItemIndexBuffer(GPURenderItem& item)
{
    if (item.hasIndexBuffer_) {
        executionEngine_->Commit().IASetIndexBuffer(&item.indexBufferDescriptor_);
    }
}

void GPUGraphicsGraphNode::DrawCallRenderItem(GPURenderItem& item)
{
    executionEngine_->Commit().DrawInstanced(item.vertexCount_, 1, 0, 0);
}