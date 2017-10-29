#pragma once

#include <pch.hpp>

#include <Data\GPUResourceDescriptor.hpp>

class GPURootTablesMap
{
public:
    using BindPointRootDescriptorPair = std::pair<UINT, GPUResourceDescriptor>;

    GPURootTablesMap();
    GPURootTablesMap(std::vector<BindPointRootDescriptorPair> const& map);
    GPURootTablesMap(std::vector<BindPointRootDescriptorPair>&& map);
    GPURootTablesMap(GPURootTablesMap const&);
    GPURootTablesMap(GPURootTablesMap&&);

    GPURootTablesMap& operator=(GPURootTablesMap const&);
    GPURootTablesMap& operator=(GPURootTablesMap&&);

    D3D12_CPU_DESCRIPTOR_HANDLE DescriptorTableCPUHandle(UINT bindingPoint) const;
    D3D12_GPU_DESCRIPTOR_HANDLE DescirptorTableGPUHandle(UINT bindingPoint) const;

    UINT Size() const;

private:
    // Descriptor tables are binded separately with binding points.
    // If root signature id described with more than one table, so they are binded
    // to continious bindung points in continuity they apper in shader root signature.
    // Other parts of root signature (such as solo root descriptors and root constants) are also
    // dependant on this continuity.
    // Example: https://habrahabr.ru/company/intel/blog/277121/
    std::vector<BindPointRootDescriptorPair> descriptorTable_;
};