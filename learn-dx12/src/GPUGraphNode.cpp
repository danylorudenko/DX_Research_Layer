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

void GPUGraphNode::SetOutputProxy(GPUGraphResourceProxy* outputProxy)
{
    output_ = outputProxy;
}

void GPUGraphNode::SetOutputSemantics(char const** semantics, UINT const semanticsNum)
{
    for (size_t i = 0; i < semanticsNum; i++) {
        outputSemantics_.push_back(semantics[i]);
    }
}

void GPUGraphNode::ImportResources(GPUGraphResourceProxy const& input)
{
    rootSignature_->ImportRootResources(input);
}

void GPUGraphNode::Preprocess()
{
    OutputResourceReferences();
}

void GPUGraphNode::Process()
{
    TransitionResources();
    SubmitProcessing();
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

void GPUGraphNode::OutputResourceReferences()
{
    /*std::size_t const outputNum = outputSemantics_.size();
    for (size_t i = 0; i < outputNum; i++) {
        output_->Insert(outputSemantics_[i].c_str(), )
    }*/
}
