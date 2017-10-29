#pragma once

#include <pch.hpp>

#include <Data\GPUResourceDescriptor.hpp>

class GPUPipelineState
{
public:
    GPUPipelineState();
    GPUPipelineState(Microsoft::WRL::ComPtr<ID3D12PipelineState>& state);
    GPUPipelineState(GPUPipelineState const&) = delete;
    GPUPipelineState(GPUPipelineState&& rhs);

    GPUPipelineState& operator=(GPUPipelineState const&) = delete;
    GPUPipelineState& operator=(GPUPipelineState&& rhs);

    
    void SetOutputResources(char const** semantics, GPUResource* resources, UINT const num);

private:
    Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState_;

    std::vector<GPUResourceDescriptor> renderTargets_;
    GPUResourceDescriptor depthStencilTarget_;
};