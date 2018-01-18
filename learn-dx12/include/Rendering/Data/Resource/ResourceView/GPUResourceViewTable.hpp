#pragma once

#include <pch.hpp>

#include <Rendering\Data\Resource\GPUResourceHandle.hpp>

class GPUResourceViewTable
{
public:
    GPUResourceViewTable();
    GPUResourceViewTable(std::size_t viewCount, GPUResourceViewHandle* views);
    GPUResourceViewTable(GPUResourceViewTable const& rhs);
    GPUResourceViewTable(GPUResourceViewTable&& rhs);

    GPUResourceViewTable& operator=(GPUResourceViewTable const& rhs);
    GPUResourceViewTable& operator=(GPUResourceViewTable&& rhs);

    GPUResourceViewHandle& TableMember(std::size_t memberIndex);
    D3D12_GPU_DESCRIPTOR_HANDLE GPUHandle(std::size_t frameIndex);

private:
    std::vector<GPUResourceViewHandle> tableMembers_;
};