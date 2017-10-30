#pragma once

#include <pch.hpp>

#include <Data\GPUFrameResourceDescriptor.hpp>

class GPUFrameRootTablesMap
{
public:
    using StateAndResource = std::pair<D3D12_RESOURCE_STATES, GPUResource*>;

    GPUFrameRootTablesMap();
    GPUFrameRootTablesMap(UINT frameCount, std::vector<std::vector<GPUFrameResourceDescriptor>> const& map, std::vector<std::vector<StateAndResource>> const& describedResources);
    GPUFrameRootTablesMap(UINT frameCount, std::vector<std::vector<GPUFrameResourceDescriptor>>&& map, std::vector<std::vector<StateAndResource>>&& describedResources);
    GPUFrameRootTablesMap(GPUFrameRootTablesMap const&);
    GPUFrameRootTablesMap(GPUFrameRootTablesMap&&);

    GPUFrameRootTablesMap& operator=(GPUFrameRootTablesMap const&);
    GPUFrameRootTablesMap& operator=(GPUFrameRootTablesMap&&);

    D3D12_CPU_DESCRIPTOR_HANDLE DescriptorTableCPUHandle(UINT frameIndex, UINT bindingPoint) const;
    D3D12_GPU_DESCRIPTOR_HANDLE DescirptorTableGPUHandle(UINT frameIndex, UINT bindingPoint) const;

    UINT TableSize() const;
    UINT DescribedResourceCount(UINT frameIndex) const;
    GPUResource* DescribedResource(UINT frameIndex, UINT resourceIndex);
    D3D12_RESOURCE_STATES DescribedResourceState(UINT frameIndex, UINT resourceIndex) const;

private:
    UINT frameCount_;

    // Descriptor tables are binded separately with binding points.
    // If root signature id described with more than one table, so they are binded
    // to continious bindung points in continuity they appear in shader root signature.
    // Other parts of root signature (such as solo root descriptors and root constants) are also
    // dependant on this continuity.
    // Example: https://habrahabr.ru/company/intel/blog/277121/
    std::vector<std::vector<GPUFrameResourceDescriptor>> descriptorTable_;

    // vector of vectors: first level is about frame buffering.
    std::vector<std::vector<StateAndResource>> describedResources_;
};