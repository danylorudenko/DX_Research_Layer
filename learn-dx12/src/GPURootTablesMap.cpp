#include <Data\GPURootTablesMap.hpp>

#include <algorithm>

GPURootTablesMap::GPURootTablesMap() = default;

GPURootTablesMap::GPURootTablesMap(std::vector<BindPointAndTableHandlePair>&& table) :
    descriptorTable_(std::move(table))
{
    std::sort(descriptorTable_.begin(), descriptorTable_.end(), 
              [](BindPointAndTableHandlePair const& lhs, BindPointAndTableHandlePair const& rhs) {
        return lhs.second.CPUViewHandle().ptr < rhs.second.CPUViewHandle().ptr;
    });
}

GPURootTablesMap::GPURootTablesMap(GPURootTablesMap const&) = default;

GPURootTablesMap::GPURootTablesMap(GPURootTablesMap&&) = default;

GPURootTablesMap& GPURootTablesMap::operator=(GPURootTablesMap const&) = default;

GPURootTablesMap& GPURootTablesMap::operator=(GPURootTablesMap&&) = default;

D3D12_GPU_DESCRIPTOR_HANDLE GPURootTablesMap::TableStartGPUHandle() const
{
    return CD3DX12_GPU_DESCRIPTOR_HANDLE::CD3DX12_GPU_DESCRIPTOR_HANDLE(descriptorTable_[0].second.GPUViewHandle());
}

D3D12_CPU_DESCRIPTOR_HANDLE GPURootTablesMap::TableStartCPUHandle() const
{
    return CD3DX12_CPU_DESCRIPTOR_HANDLE::CD3DX12_CPU_DESCRIPTOR_HANDLE(descriptorTable_[0].second.CPUViewHandle());
}

UINT GPURootTablesMap::Size() const
{
    return descriptorTable_.size();
}