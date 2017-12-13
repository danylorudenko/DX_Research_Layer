#include <Rendering\Data\Resource\GPUBuffer.hpp>

GPUBuffer::GPUBuffer() : GPUResource{}, bufferSize_{ 0 }
{ }

GPUBuffer::GPUBuffer(ID3D12Device* device, D3D12_RESOURCE_DESC const* resourceDesc, D3D12_RESOURCE_STATES initialState, std::size_t size) :
    GPUResource{},
    bufferSize_(0)
{
    {
        auto const result = device->CreateCommittedResource(
            &CD3DX12_HEAP_PROPERTIES{ D3D12_HEAP_TYPE_DEFAULT },
            D3D12_HEAP_FLAG_NONE,
            resourceDesc,
            initialState,
            nullptr,
            IID_PPV_ARGS(&resourceHandle_));
        ThrowIfFailed(result);
    }
    virtualState_ = initialState;
    bufferSize_ = size;
}

GPUBuffer::GPUBuffer(GPUBuffer&& rhs) = default;

GPUBuffer& GPUBuffer::operator=(GPUBuffer&& rhs) = default;

std::size_t GPUBuffer::Size() const
{
    return bufferSize_;
}

void GPUBuffer::Map(void** mapDest, D3D12_RANGE const* range)
{
    resourceHandle_->Map(0, range, mapDest);
}

void GPUBuffer::Unmap(D3D12_RANGE const* range)
{
    resourceHandle_->Unmap(0, range);
}