#pragma once

#include <pch.hpp>

#include <Rendering\GPUEngine.hpp>
#include <Data\GPURootSignature.hpp>
#include <Data\GPUPipelineState.hpp>
#include <Data\GPUGraphResourceProxy.hpp>

class GPUGraphNode
{
public:
    GPUGraphNode();
    GPUGraphNode(GPUEngine* engine, GPURootSignature* rootSignature, GPUPipelineState* pipelineState);
    GPUGraphNode(GPUGraphNode const&) = delete;
    GPUGraphNode(GPUGraphNode&& rhs);

    GPUGraphNode& operator=(GPUGraphNode const&) = delete;
    GPUGraphNode& operator=(GPUGraphNode&& rhs);

    void SetOutputProxy(GPUGraphResourceProxy* outputProxy);
    void SetOutputSemantics(char const** semantics, UINT const semanticsNum);

    void ImportResources(GPUGraphResourceProxy const& resourceProxy);
    void Preprocess();
    void Process();

private:
    void SetRootSignature();
    void TransitionResources();
    void SubmitProcessing();
    void OutputResourceReferences();

    GPUGraphResourceProxy* output_;
    std::vector<std::string> outputSemantics_;

    GPUEngine* executionEngine_;

    GPURootSignature* rootSignature_;
    GPUPipelineState* pipelineState_;

};