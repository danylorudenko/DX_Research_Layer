#include <Data\GPUGraphNode.hpp>

GPUGraphNode::GPUGraphNode(GPUEngine* engine, Microsoft::WRL::ComPtr<ID3D12PipelineState>& state, std::vector<char*>&& input, std::vector<char*>&& output) :
    pipelineState_(state),
    inputSemantics_(std::move(input)),
    outputSemantics_(std::move(output)),
    executionEngine_(engine)
{
}