#include <Rendering\Resource\ResourceView\GPUResourceViewTable.hpp>
#include <Rendering\Resource\ResourceView\GPUResourceView.hpp>

namespace DXRL
{

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

std::size_t GPUResourceViewTable::Size() const
{
    return tableMembers_.size();
}

D3D12_GPU_DESCRIPTOR_HANDLE GPUResourceViewTable::GPUHandle(std::size_t frameIndex)
{
    return reinterpret_cast<GPUShaderVisibleResourceView*>(&(tableMembers_[0].View(frameIndex)))->GPUHandle();
}

}