#include <Data\GPUPipelineState.hpp>

GPUPipelineState::GPUPipelineState() = default;

GPUPipelineState::GPUPipelineState(Microsoft::WRL::ComPtr<ID3D12PipelineState>& state) :
    pipelineState_{ state }
{ }

GPUPipelineState::GPUPipelineState(GPUPipelineState&& rhs) = default;

GPUPipelineState& GPUPipelineState::operator=(GPUPipelineState&& rhs) = default;