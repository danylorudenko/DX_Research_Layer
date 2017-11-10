#pragma once

#include <pch.hpp>

#include <Rendering\Data\GPUFrameResourceDescriptor.hpp>

class GPUFrameRootTablesMap
{
public:
    using StateAndResource = std::pair<D3D12_RESOURCE_STATES, GPUResource*>;
    
    template<typename T>
    using FramesCollection = std::vector<T>;

    GPUFrameRootTablesMap();
    GPUFrameRootTablesMap(UINT frameCount, std::vector<GPUFrameResourceDescriptor> const& map, FramesCollection<std::vector<StateAndResource>> const& describedResources);
    GPUFrameRootTablesMap(UINT frameCount, std::vector<GPUFrameResourceDescriptor>&& map, FramesCollection<std::vector<StateAndResource>>&& describedResources);
    GPUFrameRootTablesMap(GPUFrameRootTablesMap const&);
    GPUFrameRootTablesMap(GPUFrameRootTablesMap&&);

    GPUFrameRootTablesMap& operator=(GPUFrameRootTablesMap const&);
    GPUFrameRootTablesMap& operator=(GPUFrameRootTablesMap&&);

    D3D12_CPU_DESCRIPTOR_HANDLE DescriptorTableCPUHandle(UINT frameIndex, UINT bindingPoint) const;
    D3D12_GPU_DESCRIPTOR_HANDLE DescirptorTableGPUHandle(UINT frameIndex, UINT bindingPoint) const;

    UINT TableSize() const;
    UINT DescribedResourceCount(UINT frameIndex) const;
    GPUResource* DescribedResource(UINT frameIndex, UINT resourceIndex);
    D3D12_RESOURCE_STATES DescribedResourceTargetState(UINT frameIndex, UINT resourceIndex) const;
    D3D12_RESOURCE_STATES DescribedResourceCurrentState(UINT frameIndex, UINT resourceIndex) const;

private:
    UINT frameCount_ = 0U;

    // Descriptor tables are binded separately with binding points.
    // If root signature id described with more than one table, so they are binded
    // to continious bindung points in continuity they appear in shader root signature.
    // Other parts of root signature (such as solo root descriptors and root constants) are also
    // dependant on this continuity.
    // Example: https://habrahabr.ru/company/intel/blog/277121/
    std::vector<GPUFrameResourceDescriptor> descriptorTable_;

    // vector of vectors: first level is about frame buffering.
    // First item in pair describes target state for the resouce.
    // Current state lies inside of the second pair item.
    FramesCollection<std::vector<StateAndResource>> describedResources_;
};