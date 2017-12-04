#include <Rendering\Data\FrameGraph\GPUPresentGraphNode.hpp>

GPUPresentGraphNode::GPUPresentGraphNode() = default;

GPUPresentGraphNode::GPUPresentGraphNode(Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain, GPUEngine* executionEngine) :
    GPUGraphNode{ executionEngine, nullptr, nullptr, 3 }, swapChain_{ swapChain }
{ }

GPUPresentGraphNode::GPUPresentGraphNode(GPUPresentGraphNode&& rhs) = default;

GPUPresentGraphNode& GPUPresentGraphNode::operator=(GPUPresentGraphNode&& rhs) = default;

void GPUPresentGraphNode::ImportRenderTarget(GPUResourceSet* renderTarget)
{
    renderTargets_.push_back(renderTarget);
}

void GPUPresentGraphNode::Process(UINT64 frameIndex) 
{
    int const localFrameIndex = frameIndex % 3;
    TransitionRenderTargetState(localFrameIndex);

    executionEngine_->FlushReset();
    swapChain_->Present(0, 0);
}

void GPUPresentGraphNode::TransitionRenderTargetState(int frameIndex)
{
    int const renderTargetCount = static_cast<int>(renderTargets_.size());
    for (int i = 0; i < renderTargetCount; i++) {
        if (renderTargets_[i]->State(frameIndex) != D3D12_RESOURCE_STATE_PRESENT) {
            renderTargets_[i]->Transition(frameIndex, executionEngine_->CommandList(), D3D12_RESOURCE_STATE_PRESENT);
        }
    }
}