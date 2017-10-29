#include <Data\GPURootTablesMap.hpp>

#include <algorithm>

GPURootTablesMap::GPURootTablesMap() = default;

GPURootTablesMap::GPURootTablesMap(std::vector<BindPointRootDescriptorPair> const& rhs, std::vector<StateAndResource> const& describedResources) :
    descriptorTable_(rhs), describedResources_(describedResources)
{ }

GPURootTablesMap::GPURootTablesMap(std::vector<BindPointRootDescriptorPair>&& table, std::vector<StateAndResource>&& describedResources) :
    descriptorTable_(std::move(table)), describedResources_(std::move(describedResources))
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

UINT GPURootTablesMap::TableSize() const
{
    return descriptorTable_.size();
}

UINT GPURootTablesMap::DescribedResourceCount() const
{
    return describedResources_.size();
}

GPUResource* GPURootTablesMap::DescribedResource(UINT index)
{
    return describedResources_[index].second;
}

D3D12_RESOURCE_STATES GPURootTablesMap::DescribedResourceState(UINT index) const
{
    return describedResources_[index].first;
}