#include <Rendering\Data\FrameGraph\GPUPresentGraphNode.hpp>
#include <Rendering\Data\Resource\ResourceView\GPUResourceView.hpp>

GPUPresentGraphNode::GPUPresentGraphNode() = default;

GPUPresentGraphNode::GPUPresentGraphNode(Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain, GPUEngine* executionEngine) :
    GPUGraphNode{ executionEngine, GPURootSignature{}, GPUPipelineState{} }, swapChain_{ swapChain }
{ }

GPUPresentGraphNode::GPUPresentGraphNode(GPUPresentGraphNode&& rhs) = default;

GPUPresentGraphNode& GPUPresentGraphNode::operator=(GPUPresentGraphNode&& rhs) = default;

void GPUPresentGraphNode::ImportRenderTarget(GPUResourceViewHandle* renderTarget)
{
    //renderTargets_.push_back(renderTarget);
}

void GPUPresentGraphNode::Process(std::size_t frameIndex) 
{
    TransitionRenderTargetState(frameIndex);

    executionEngine_->FlushReset();
    swapChain_->Present(0, 0);
}

void GPUPresentGraphNode::TransitionRenderTargetState(std::size_t frameIndex)
{
    auto const renderTargetCount = renderTargets_.size();
    assert(renderTargetCount <= 5 && "Render targets are limited to 5");

    D3D12_RESOURCE_BARRIER transitions[5];
    std::size_t transitionCounter = 0;
    for (int i = 0; i < renderTargetCount; i++) {
        auto& resourceView = renderTargets_[i].View(frameIndex);
        if (resourceView.Resource().State() != D3D12_RESOURCE_STATE_PRESENT) {
            //renderTargets_[i]->Transition(frameIndex, executionEngine_->CommandList(), D3D12_RESOURCE_STATE_PRESENT);
        }
    }
}