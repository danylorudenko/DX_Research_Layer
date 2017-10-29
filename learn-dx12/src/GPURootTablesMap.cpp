#include <Data\GPURootTablesMap.hpp>

#include <algorithm>

GPURootTablesMap::GPURootTablesMap() = default;

GPURootTablesMap::GPURootTablesMap(std::vector<BindPointRootDescriptorPair> const& rhs) :
    descriptorTable_(rhs)
{ }

GPURootTablesMap::GPURootTablesMap(std::vector<BindPointRootDescriptorPair>&& table) :
    descriptorTable_(std::move(table))
{ }

GPURootTablesMap::GPURootTablesMap(GPURootTablesMap const&) = default;

GPURootTablesMap::GPURootTablesMap(GPURootTablesMap&&) = default;

GPURootTablesMap& GPURootTablesMap::operator=(GPURootTablesMap const&) = default;

GPURootTablesMap& GPURootTablesMap::operator=(GPURootTablesMap&&) = default;

D3D12_GPU_DESCRIPTOR_HANDLE GPURootTablesMap::DescirptorTableGPUHandle(UINT bindingPoint) const
{
    return CD3DX12_GPU_DESCRIPTOR_HANDLE::CD3DX12_GPU_DESCRIPTOR_HANDLE(descriptorTable_[bindingPoint].second.GPUViewHandle());
}

D3D12_CPU_DESCRIPTOR_HANDLE GPURootTablesMap::DescriptorTableCPUHandle(UINT bindingPoint) const
{
    return CD3DX12_CPU_DESCRIPTOR_HANDLE::CD3DX12_CPU_DESCRIPTOR_HANDLE(descriptorTable_[bindingPoint].second.CPUViewHandle());
}

UINT GPURootTablesMap::Size() const
{
    return descriptorTable_.size();
}