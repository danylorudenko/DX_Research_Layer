#include <Rendering\PipelineState\GPUPipelineState.hpp>

namespace DXRL
{

GPUPipelineState::GPUPipelineState() = default;

GPUPipelineState::GPUPipelineState(Microsoft::WRL::ComPtr<ID3D12PipelineState>& state) :
    pipelineState_{ state }
{ }

GPUPipelineState::GPUPipelineState(GPUPipelineState&& rhs) = default;

GPUPipelineState& GPUPipelineState::operator=(GPUPipelineState&& rhs) = default;

}