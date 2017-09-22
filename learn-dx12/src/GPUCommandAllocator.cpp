#include <Rendering\GPUCommandAllocator.hpp>

GPUCommandAllocator::GPUCommandAllocator(ID3D12Device* device, D3D12_COMMAND_LIST_TYPE type) :
    fence_(device)
{
    ThrowIfFailed(device->CreateCommandAllocator(type, IID_PPV_ARGS(&commandAllocator_)));
}

GPUCommandAllocator::GPUCommandAllocator(GPUCommandAllocator&& rhs) :
    fence_(std::move(rhs.fence_))
{
    commandAllocator_ = std::move(rhs.commandAllocator_);

    ZeroMemory(&rhs, sizeof(rhs));
}

GPUCommandAllocator& GPUCommandAllocator::operator=(GPUCommandAllocator&& rhs)
{
    commandAllocator_ = std::move(rhs.commandAllocator_);
    fence_ = std::move(rhs.fence_);

    ZeroMemory(&rhs, sizeof(rhs));

    return *this;
}
