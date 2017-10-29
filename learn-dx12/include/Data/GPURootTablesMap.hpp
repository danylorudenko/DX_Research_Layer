#pragma once

#include <pch.hpp>

#include <Data\GPUResourceDescriptor.hpp>

class GPURootTablesMap
{
public:
    using BindPointRootDescriptorPair = std::pair<UINT, GPUResourceDescriptor>;
    using StateAndResource = std::pair<D3D12_RESOURCE_STATES, GPUResource*>;

    GPURootTablesMap();
    GPURootTablesMap(std::vector<BindPointRootDescriptorPair> const& map, std::vector<StateAndResource> const& describedResources);
    GPURootTablesMap(std::vector<BindPointRootDescriptorPair>&& map, std::vector<StateAndResource>&& describedResources);
    GPURootTablesMap(GPURootTablesMap const&);
    GPURootTablesMap(GPURootTablesMap&&);

    GPURootTablesMap& operator=(GPURootTablesMap const&);
    GPURootTablesMap& operator=(GPURootTablesMap&&);

    D3D12_CPU_DESCRIPTOR_HANDLE DescriptorTableCPUHandle(UINT bindingPoint) const;
    D3D12_GPU_DESCRIPTOR_HANDLE DescirptorTableGPUHandle(UINT bindingPoint) const;

    UINT TableSize() const;
    UINT DescribedResourceCount() const;
    GPUResource* DescribedResource(UINT index);
    D3D12_RESOURCE_STATES DescribedResourceState(UINT index) const;

private:
    // Descriptor tables are binded separately with binding points.
    // If root signature id described with more than one table, so they are binded
    // to continious bindung points in continuity they apper in shader root signature.
    // Other parts of root signature (such as solo root descriptors and root constants) are also
    // dependant on this continuity.
    // Example: https://habrahabr.ru/company/intel/blog/277121/
    std::vector<BindPointRootDescriptorPair> descriptorTable_;
    std::vector<StateAndResource> describedResources_;
};