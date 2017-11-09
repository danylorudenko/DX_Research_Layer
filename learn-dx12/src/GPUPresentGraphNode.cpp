#include <Data\FrameGraph\GPUPresentGraphNode.hpp>

GPUPresentGraphNode::GPUPresentGraphNode() = default;

GPUPresentGraphNode::GPUPresentGraphNode(Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain) :
    GPUGraphNode{}, swapChain_{swapChain}
{ }

GPUPresentGraphNode::GPUPresentGraphNode(GPUPresentGraphNode&& rhs) = default;

GPUPresentGraphNode& GPUPresentGraphNode::operator=(GPUPresentGraphNode&& rhs) = default;

void GPUPresentGraphNode::Process(UINT64 frameIndex) 
{
    if (SynchronizeFrames(frameIndex)) {
        swapChain_->Present(0, 0);
    }
}