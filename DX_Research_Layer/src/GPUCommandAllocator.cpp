#include <Rendering\GPUEngine\GPUCommandAllocator.hpp>

namespace DXRL
{

GPUCommandAllocator::GPUCommandAllocator() = default;

GPUCommandAllocator::GPUCommandAllocator(ID3D12Device* device, D3D12_COMMAND_LIST_TYPE type)
{
    {
        auto const result = device->CreateCommandAllocator(type, IID_PPV_ARGS(&commandAllocator_));
        DXRL_THROW_IF_FAILED(result);
    }
}

GPUCommandAllocator::GPUCommandAllocator(GPUCommandAllocator&& rhs) :
    commandAllocator_(std::move(rhs.commandAllocator_)),
    fenceTargetValue_(rhs.fenceTargetValue_)
{

}

GPUCommandAllocator& GPUCommandAllocator::operator=(GPUCommandAllocator&& rhs)
{
    commandAllocator_ = std::move(rhs.commandAllocator_);
    fenceTargetValue_ = rhs.fenceTargetValue_;

    return *this;
}

}