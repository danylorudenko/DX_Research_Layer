#pragma once

#include <pch.hpp>

class GPUFrameGraph
{
public:
    GPUFrameGraph();
    GPUFrameGraph(Microsoft::WRL::ComPtr<ID3D12PipelineState>& pipelineState);


private:
    Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_;
    Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState_;
    std::vector<GPUFrameGraph> children_;
};