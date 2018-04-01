#include <Rendering\FrameGraph\GPUPresentGraphNode.hpp>
#include <Rendering\Resource\ResourceView\GPUResourceView.hpp>

namespace DXRL
{

GPUPresentGraphNode::GPUPresentGraphNode() = default;

GPUPresentGraphNode::GPUPresentGraphNode(GPUSwapChain& swapChain, GPUEngine& executionEngine) :
    GPUGraphNode{ executionEngine }, swapChain_{ &swapChain }
{ }

GPUPresentGraphNode::GPUPresentGraphNode(GPUPresentGraphNode&& rhs) = default;

GPUPresentGraphNode& GPUPresentGraphNode::operator=(GPUPresentGraphNode&& rhs) = default;

void GPUPresentGraphNode::ImportRenderTarget(GPUResourceViewHandle const& renderTarget)
{
    renderTarget_ = renderTarget;
}

void GPUPresentGraphNode::Process(std::size_t frameIndex)
{
    TransitionRenderTargetState(frameIndex);

    executionEngine_->FlushReset();
    swapChain_->Present(0);
}

void GPUPresentGraphNode::TransitionRenderTargetState(std::size_t frameIndex)
{
    renderTarget_.View(frameIndex).Resource().Transition(*executionEngine_, D3D12_RESOURCE_STATE_PRESENT);
}

}