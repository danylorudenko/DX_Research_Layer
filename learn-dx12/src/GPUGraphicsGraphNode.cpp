#include <Rendering\Data\FrameGraph\GPUGraphicsGraphNode.hpp>

Color::Color(float r, float g, float b, float a) : 
    color{ r, g, b, a }
{ }

GPUGraphicsGraphNode::GPUGraphicsGraphNode() = default;

GPUGraphicsGraphNode::GPUGraphicsGraphNode(GPUEngine* engine, GPURootSignature&& rootSignature, GPUPipelineState&& pipelineState) :
    GPUGraphNode{ engine, std::move(rootSignature), std::move(pipelineState) }
{ }

GPUGraphicsGraphNode::GPUGraphicsGraphNode(GPUGraphicsGraphNode&& rhs) :
    GPUGraphNode(std::move(rhs))
{ }

GPUGraphicsGraphNode& GPUGraphicsGraphNode::operator=(GPUGraphicsGraphNode&& rhs)
{
    GPUGraphNode::operator=(std::move(rhs));
    return *this;
}

void GPUGraphicsGraphNode::Process(std::size_t frameIndex)
{
    BindPipelineState();
    BindViewportScissor();
    BindPassRoot(frameIndex);

    BindRenderDepthStencilTargets(frameIndex);
    
    TransitionRenderTargets(frameIndex);
    TransitionPassResources(frameIndex);

    ClearRenderTargets(frameIndex);
    ClearDepthStencilTargets(frameIndex);

    IterateRenderItems(frameIndex);
}

void GPUGraphicsGraphNode::IterateRenderItems(std::size_t frameIndex)
{
    auto const itemCount = renderItems_.size();
    for (std::size_t i = 0; i < itemCount; i++) {
        auto item = renderItems_[i];
        BindRenderItemVertexBuffer(item);
        BindRenderItemIndexBuffer(item);
        BindRenderItemRootResources(item, frameIndex);
        DrawCallRenderItem(item);
    }
}

void GPUGraphicsGraphNode::ImportRenderTargets(std::size_t count, GPUResourceViewHandle const* renderTargets)
{
    renderTargets_.clear();
    for (std::size_t i = 0; i < count; i++) {
        renderTargets_.emplace_back(renderTargets[i]);
    }
}

void GPUGraphicsGraphNode::ImportDepthStencilTarget(GPUResourceViewHandle const& depthStencilDescriptor)
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

void GPUGraphicsGraphNode::ImportClearColors(Color const* clearColors, std::size_t colorCount)
{
    assert(colorCount <= renderTargets_.size() && "Can't have more clear colors than there are render targets.");
    assert(clearColors_.empty() && "Clear colors list is immutable once set.");

    for (size_t i = 0; i < colorCount; i++) {
        clearColors_.push_back(clearColors[i]);
    }
}

void GPUGraphicsGraphNode::ImportViewportScissor(D3D12_VIEWPORT const& viewport, D3D12_RECT const& scissorRect)
{
    viewportRect_ = viewport;
    scissorRect_ = scissorRect;
}

void GPUGraphicsGraphNode::BindRenderDepthStencilTargets(std::size_t frameIndex)
{    
    auto const renderTargetsCount = static_cast<int>(renderTargets_.size());
    assert(renderTargetsCount <= 5 && "More than 5 render targets is not currently supported.");

    D3D12_CPU_DESCRIPTOR_HANDLE renderTargetHandles[5];
    for (int i = 0; i < renderTargetsCount; i++) {
        renderTargetHandles[i] = renderTargets_[i].View(frameIndex).CPUHandle();
    }

    D3D12_CPU_DESCRIPTOR_HANDLE depthStencilHandle = depthStencilTarget_.View(frameIndex).CPUHandle();
    
    if (renderTargetsCount > 0) {
        executionEngine_->Commit().OMSetRenderTargets(renderTargetsCount, renderTargetHandles, false, depthStencilHandle.ptr != 0 ? &depthStencilHandle : nullptr);
    }
}

void GPUGraphicsGraphNode::BindPipelineState()
{
    executionEngine_->Commit().SetPipelineState(pipelineState_.Get());
}

void GPUGraphicsGraphNode::BindViewportScissor()
{
    executionEngine_->Commit().RSSetViewports(1, &viewportRect_);
    executionEngine_->Commit().RSSetScissorRects(1, &scissorRect_);
}

void GPUGraphicsGraphNode::TransitionRenderTargets(std::size_t frameIndex)
{
    auto const renderTargetsCount = renderTargets_.size();
    assert(renderTargetsCount <= 5 && "More that 5 render targets is not currently supported.");

    D3D12_RESOURCE_BARRIER transitions[6];
    std::size_t transitionsCounter = 0;
    for (std::size_t i = 0; i < renderTargetsCount; i++) {
        auto& resourceView = renderTargets_[i].View(frameIndex);
        if (resourceView.Resource().State() != D3D12_RESOURCE_STATE_RENDER_TARGET) {
            resourceView.Resource().PrepareTransition(D3D12_RESOURCE_STATE_RENDER_TARGET, transitions[transitionsCounter++]);
        }
    }

    auto& depthStencilView = depthStencilTarget_.View(frameIndex);
    if (depthStencilView.CPUHandle().ptr != 0) {
        depthStencilView.Resource().PrepareTransition(depthStencilView.TargetState(), transitions[transitionsCounter++]);
    }

    executionEngine_->Commit().ResourceBarrier(static_cast<UINT>(transitionsCounter), transitions);
}


void GPUGraphicsGraphNode::ClearRenderTargets(std::size_t frameIndex)
{
    std::size_t const clearColorsCount = clearColors_.size();
    for (size_t i = 0; i < clearColorsCount; i++) {
        executionEngine_->Commit().ClearRenderTargetView(renderTargets_[i].View(frameIndex).CPUHandle(), clearColors_[i].color, 0, nullptr);
    }
}

void GPUGraphicsGraphNode::ClearDepthStencilTargets(std::size_t frameIndex)
{

}

void GPUGraphicsGraphNode::BindRenderItemRootResources(GPURenderItem& item, std::size_t frameIndex)
{
    executionEngine_->Commit().SetGraphicsRootDescriptorTable(item.dynamicArg_.bindSlot_, item.dynamicArg_.itemTable_.GPUHandle(frameIndex));
}

void GPUGraphicsGraphNode::BindRenderItemVertexBuffer(GPURenderItem& item)
{
    executionEngine_->Commit().IASetPrimitiveTopology(item.primitiveTopology_);
    executionEngine_->Commit().IASetVertexBuffers(0, 1, &item.vertexBufferDescriptor_);
}

void GPUGraphicsGraphNode::BindRenderItemIndexBuffer(GPURenderItem& item)
{
    executionEngine_->Commit().IASetIndexBuffer(&item.indexBufferDescriptor_);
}

void GPUGraphicsGraphNode::DrawCallRenderItem(GPURenderItem& item)
{
    executionEngine_->Commit().DrawInstanced(item.vertexCount_, 1, 0, 0);
}