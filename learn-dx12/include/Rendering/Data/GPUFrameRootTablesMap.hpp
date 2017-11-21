#pragma once

#include <pch.hpp>

#include <Rendering\Data\GPUFrameResourceDescriptor.hpp>

class GPUFrameRootTablesMap
{
public:
    // Target state of the resource and the resource.
    using StateAndResource = std::pair<D3D12_RESOURCE_STATES, GPUFrameResource*>;
    
    GPUFrameRootTablesMap();
    GPUFrameRootTablesMap(int frameCount, std::vector<GPUFrameResourceDescriptor> const& map, std::vector<StateAndResource> const& describedResources);
    GPUFrameRootTablesMap(int frameCount, std::vector<GPUFrameResourceDescriptor>&& map, std::vector<StateAndResource>&& describedResources);
    GPUFrameRootTablesMap(GPUFrameRootTablesMap const&);
    GPUFrameRootTablesMap(GPUFrameRootTablesMap&&);

    GPUFrameRootTablesMap& operator=(GPUFrameRootTablesMap const&);
    GPUFrameRootTablesMap& operator=(GPUFrameRootTablesMap&&);

    D3D12_CPU_DESCRIPTOR_HANDLE DescriptorTableCPUHandle(int frameIndex, int bindingPoint) const;
    D3D12_GPU_DESCRIPTOR_HANDLE DescirptorTableGPUHandle(int frameIndex, int bindingPoint) const;

    int TableSize() const;
    int DescribedResourceCount(int frameIndex) const;
    GPUFrameResource* DescribedResource(int resourceIndex);
    D3D12_RESOURCE_STATES DescribedResourceTargetState(int resourceIndex) const;
    D3D12_RESOURCE_STATES DescribedResourceCurrentState(int frameIndex, int resourceIndex) const;

private:
    int frameCount_ = 0U;

    // Descriptor tables are binded separately with binding points.
    // If root signature is described with more than one table, so they are binded
    // to continious bindung points in continuity they appear in shader root signature.
    // Other parts of root signature (such as solo root descriptors and root constants) are also
    // dependant on this continuity.
    // Example: https://habrahabr.ru/company/intel/blog/277121/
    std::vector<GPUFrameResourceDescriptor> descriptorTable_;

    // vector of vectors: first level is about frame buffering.
    // First item in pair describes target state for the resouce.
    // Current state lies inside of the second pair item.
    std::vector<StateAndResource> describedResources_;
};