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
    GPUGraphNode(GPUEngine* engine, Microsoft::WRL::ComPtr<ID3D12PipelineState>& pipelineState);
    GPUGraphNode(GPUGraphNode const&) = delete;
    GPUGraphNode(GPUGraphNode&& rhs);

    GPUGraphNode& operator=(GPUGraphNode const&) = delete;
    GPUGraphNode& operator=(GPUGraphNode&& rhs);

    void ImportInputs();
    void Process();
    void Wait();

private:
    std::vector<GPUGraphResourceProxy*> parents_;
    std::vector<GPUGraphResourceProxy*> children_;

    GPUEngine* executionEngine_;

    GPURootSignature rootSignature_;
    GPUPipelineState pipelineState_;

};