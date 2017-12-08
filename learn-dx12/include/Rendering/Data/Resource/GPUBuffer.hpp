#pragma once

#include <pch.hpp>

#include <Rendering\Data\Resource\GPUResource.hpp>

class GPUBuffer : public GPUResource
{
public:
    GPUBuffer();
    GPUBuffer(ID3D12Device* device, D3D12_RESOURCE_DESC const* resourceDesc, D3D12_RESOURCE_STATES initialState, std::size_t size);

    GPUBuffer(GPUBuffer const&) = delete;
    GPUBuffer(GPUBuffer&& rhs);

    GPUBuffer& operator=(GPUBuffer const&) = delete;
    GPUBuffer& operator=(GPUBuffer&& rhs);

    std::size_t Size() const;
    
    void Map(void** mapDest, D3D12_RANGE const& range);
    void Unmap(D3D12_RANGE const& rangle);

protected:
    std::size_t bufferSize_;
};