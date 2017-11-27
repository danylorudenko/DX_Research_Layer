#pragma once

#include <pch.hpp>

class GPUPipelineState
{
public:
    GPUPipelineState();
    GPUPipelineState(Microsoft::WRL::ComPtr<ID3D12PipelineState>& state);
    GPUPipelineState(GPUPipelineState const&) = delete;
    GPUPipelineState(GPUPipelineState&& rhs);

    GPUPipelineState& operator=(GPUPipelineState const&) = delete;
    GPUPipelineState& operator=(GPUPipelineState&& rhs);

    ID3D12PipelineState* Get() { return pipelineState_.Get(); }

private:
    Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState_;
};