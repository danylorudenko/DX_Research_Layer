#pragma once

#include <pch.hpp>

#include <Rendering\GPUEngine.hpp>

class GPUGraphNode
{
public:
    GPUGraphNode(GPUEngine* engine, Microsoft::WRL::ComPtr<ID3D12PipelineState>& pipelineState, std::vector<char*>&& input, std::vector<char*>&& output);

    void Input(GPUGraphNode* inputNode);
    void Process();
    void Output(GPUGraphNode* outputNode);

private:
    GPUEngine* executionEngine_;

    Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState_;
    std::vector<char*> inputSemantics_;
    std::vector<char*> outputSemantics_;
};