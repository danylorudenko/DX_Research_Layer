#include <Data\GPUFrameRootTablesMap.hpp>

GPUFrameRootTablesMap::GPUFrameRootTablesMap() = default;

GPUFrameRootTablesMap::GPUFrameRootTablesMap(UINT frameCount, std::vector<GPUFrameResourceDescriptor> const& map, FramesCollection<std::vector<StateAndResource>> const& describedResources) :
    frameCount_(frameCount), descriptorTable_(map), describedResources_(describedResources)
{ }

GPUFrameRootTablesMap::GPUFrameRootTablesMap(UINT frameCount, std::vector<GPUFrameResourceDescriptor>&& map, FramesCollection<std::vector<StateAndResource>>&& describedResources) :
    frameCount_(std::move(frameCount)), descriptorTable_(std::move(map)), describedResources_(describedResources)
{ }

GPUFrameRootTablesMap::GPUFrameRootTablesMap(GPUFrameRootTablesMap const& rhs) = default;

GPUFrameRootTablesMap::GPUFrameRootTablesMap(GPUFrameRootTablesMap&& rhs) = default;

GPUFrameRootTablesMap& GPUFrameRootTablesMap::operator=(GPUFrameRootTablesMap const& rhs) = default;

GPUFrameRootTablesMap& GPUFrameRootTablesMap::operator=(GPUFrameRootTablesMap&& rhs) = default;

D3D12_CPU_DESCRIPTOR_HANDLE GPUFrameRootTablesMap::DescriptorTableCPUHandle(UINT frameIndex, UINT bindingPoint) const
{
    return descriptorTable_[bindingPoint].CPUViewHandle(frameIndex);
}

D3D12_GPU_DESCRIPTOR_HANDLE GPUFrameRootTablesMap::DescirptorTableGPUHandle(UINT frameIndex, UINT bindingPoint) const
{
    return descriptorTable_[bindingPoint].GPUViewHandle(frameIndex);
}

UINT GPUFrameRootTablesMap::TableSize() const
{
    return descriptorTable_.size();
}

UINT GPUFrameRootTablesMap::DescribedResourceCount(UINT frameIndex) const
{
    return describedResources_[frameIndex].size();
}

GPUResource* GPUFrameRootTablesMap::DescribedResource(UINT frameIndex, UINT resourceIndex)
{
    return describedResources_[frameIndex][resourceIndex].second;
}

D3D12_RESOURCE_STATES GPUFrameRootTablesMap::DescribedResourceTargetState(UINT frameIndex, UINT resourceIndex) const
{
    return describedResources_[frameIndex][resourceIndex].first;
}

D3D12_RESOURCE_STATES GPUFrameRootTablesMap::DescribedResourceCurrentState(UINT frameIndex, UINT resourceIndex) const
{
    return describedResources_[frameIndex][resourceIndex].second->State();
}
