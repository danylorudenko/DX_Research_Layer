#include <Rendering\GPUCommandAllocator.hpp>

GPUCommandAllocator::GPUCommandAllocator(ID3D12Device* device, D3D12_COMMAND_LIST_TYPE type)
{
    {
        auto const result = device->CreateCommandAllocator(type, IID_PPV_ARGS(&commandAllocator_));
        ThrowIfFailed(result);
    }
}

GPUCommandAllocator::GPUCommandAllocator(GPUCommandAllocator&& rhs) :
    fenceTargetValue_(std::move(rhs.fenceTargetValue_)),
    commandAllocator_(std::move(rhs.commandAllocator_))
{
    ZeroMemory(&rhs, sizeof(rhs));
}

GPUCommandAllocator& GPUCommandAllocator::operator=(GPUCommandAllocator&& rhs)
{
    commandAllocator_ = std::move(rhs.commandAllocator_);
    fenceTargetValue_ = rhs.fenceTargetValue_;

    ZeroMemory(&rhs, sizeof(rhs));

    return *this;
}

