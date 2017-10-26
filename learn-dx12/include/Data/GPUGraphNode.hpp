#pragma once

#include <pch.hpp>

#include <Rendering\GPUEngine.hpp>
#include <Data\GPURootSignature.hpp>
#include <Data\GPUPipelineState.hpp>
#include <Data\GPUResourceDescriptor.hpp>

class GPUGraphNode
{
public:
    GPUGraphNode();
    GPUGraphNode(GPUEngine* engine, GPURootSignature* rootSignature, GPUPipelineState* pipelineState);
    GPUGraphNode(GPUGraphNode const&) = delete;
    GPUGraphNode(GPUGraphNode&& rhs);

    GPUGraphNode& operator=(GPUGraphNode const&) = delete;
    GPUGraphNode& operator=(GPUGraphNode&& rhs);

    void SetOutputNode(GPUGraphNode* outputProxy);
    GPUResourceDescriptor* RequestOutputResource(std::string const& semantics) const;

    void ImportResources(GPUGraphNode const& input);
    void Process();

private:
    void SetRootSignature();
    void TransitionResources();
    void SubmitProcessing();
    void TriggerChildren();

    GPUEngine* executionEngine_;

    GPURootSignature* rootSignature_;
    GPUPipelineState* pipelineState_;

    GPUGraphNode* output_;

};