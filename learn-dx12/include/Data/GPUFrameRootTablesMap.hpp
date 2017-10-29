#pragma once

#include <pch.hpp>

#include <Data\GPUResourceDescriptor.hpp>

class GPUFrameRootTablesMap
{
public:
    using BindPointRootDescriptorPair = std::pair<UINT, GPUResourceDescriptor>;
    using StateAndResource = std::pair<D3D12_RESOURCE_STATES, GPUResource*>;

    GPUFrameRootTablesMap();
    GPUFrameRootTablesMap(UINT frameCount, std::vector<BindPointRootDescriptorPair> const& map, std::vector<StateAndResource> const& describedResources);
    GPUFrameRootTablesMap(UINT frameCount, std::vector<BindPointRootDescriptorPair>&& map, std::vector<StateAndResource>&& describedResources);
    GPUFrameRootTablesMap(GPUFrameRootTablesMap const&);
    GPUFrameRootTablesMap(GPUFrameRootTablesMap&&);

    GPUFrameRootTablesMap& operator=(GPUFrameRootTablesMap const&);
    GPUFrameRootTablesMap& operator=(GPUFrameRootTablesMap&&);

    D3D12_CPU_DESCRIPTOR_HANDLE DescriptorTableCPUHandle(UINT frameIndex, UINT bindingPoint) const;
    D3D12_GPU_DESCRIPTOR_HANDLE DescirptorTableGPUHandle(UINT frameIndex, UINT bindingPoint) const;

    UINT TableSize(UINT frameIndex) const;
    UINT DescribedResourceCount(UINT frameIndex) const;
    GPUResource* DescribedResource(UINT frameIndex, UINT resourceIndex);
    D3D12_RESOURCE_STATES DescribedResourceState(UINT frameIndex, UINT resourceIndex) const;

    ~GPUFrameRootTablesMap();

private:
    // Descriptor tables are binded separately with binding points.
    // If root signature id described with more than one table, so they are binded
    // to continious bindung points in continuity they apper in shader root signature.
    // Other parts of root signature (such as solo root descriptors and root constants) are also
    // dependant on this continuity.
    // Example: https://habrahabr.ru/company/intel/blog/277121/

    // vector of vectors: first level is about frame buffering.

    std::vector<std::vector<BindPointRootDescriptorPair>> descriptorTable_;
    std::vector<std::vector<StateAndResource>> describedResources_;
};