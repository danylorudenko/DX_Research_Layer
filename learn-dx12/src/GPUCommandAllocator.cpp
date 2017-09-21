#include <Rendering\GPUCommandAllocator.hpp>

GPUCommandAllocator::GPUCommandAllocator(ID3D12Device* device, D3D12_COMMAND_LIST_TYPE type) :
    fence_(device)
{
    ThrowIfFailed(device->CreateCommandAllocator(type, IID_PPV_ARGS(&commandAllocator_)));
}