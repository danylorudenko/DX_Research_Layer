#include <Data\GPUGraphNode.hpp>

GPUGraphNode::GPUGraphNode() = default;

GPUGraphNode::GPUGraphNode(GPUEngine* engine, GPURootSignature* rootSignature, GPUPipelineState* pipelineState) :
    executionEngine_(engine),
    rootSignature_(rootSignature),
    pipelineState_(pipelineState)
{

}

GPUGraphNode::GPUGraphNode(GPUGraphNode&&) = default;

GPUGraphNode& GPUGraphNode::operator=(GPUGraphNode&&) = default;

void GPUGraphNode::SetOutputNode(GPUGraphNode* outputProxy)
{
    output_ = outputProxy;
}

void GPUGraphNode::SetOutputResources(char const** semantics, GPUResource* resources, UINT const num)
{
    for (size_t i = 0; i < num; i++) {
        outputMap_[semantics[i]] = resources;
    }
}

GPUResource* GPUGraphNode::RequestResource(std::string const& semantics) const
{
    return outputMap_.at(semantics);
}

void GPUGraphNode::ImportResources(GPUGraphNode const& input)
{
    rootSignature_->ImportRootResources(input);
}

void GPUGraphNode::Process()
{
    TransitionResources();
    SubmitProcessing();

    TriggerChildren();
}

void GPUGraphNode::TriggerChildren()
{
    output_->ImportResources(*this);
    output_->Process();
}

void GPUGraphNode::SetRootSignature()
{
    rootSignature_->SetRootSignature(executionEngine_);
    rootSignature_->SetRootSignatureDescriptorBindings(executionEngine_);
}

void GPUGraphNode::TransitionResources()
{
    rootSignature_->TransitionRootResources(executionEngine_);
}

