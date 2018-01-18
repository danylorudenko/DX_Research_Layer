#include <Rendering\Data\Resource\ResourceView\GPUResourceViewTable.hpp>

GPUResourceViewTable::GPUResourceViewTable() = default;

GPUResourceViewTable::GPUResourceViewTable(std::size_t viewCount, GPUResourceViewHandle* handles)
{
    tableMembers_.resize(viewCount);
    for (size_t i = 0; i < viewCount; i++) {
        tableMembers_[i] = handles[i];
    }
}

GPUResourceViewTable::GPUResourceViewTable(GPUResourceViewTable const& rhs) = default;

GPUResourceViewTable::GPUResourceViewTable(GPUResourceViewTable&& rhs) = default;

GPUResourceViewTable& GPUResourceViewTable::operator=(GPUResourceViewTable const& rhs) = default;

GPUResourceViewTable& GPUResourceViewTable::operator=(GPUResourceViewTable&& rhs) = default;

GPUResourceViewHandle& GPUResourceViewTable::TableMember(std::size_t memberIndex)
{
    return tableMembers_[memberIndex];
}

D3D12_GPU_DESCRIPTOR_HANDLE GPUResourceViewTable::GPUHandle(std::size_t frameIndex)
{
    // I'M HERE
    //return tableMembers_[0].View(frameIndex).
}
