#include <Data\GPUFrameRootTablesMap.hpp>

GPUFrameRootTablesMap::GPUFrameRootTablesMap() = default;

GPUFrameRootTablesMap::GPUFrameRootTablesMap(UINT frameCount, std::vector<BindPointRootDescriptorPair> const& map, std::vector<std::vector<StateAndResource>> const& describedResources) :
    frameCount_(frameCount), descriptorTable_(map), describedResources_(describedResources)
{ }

GPUFrameRootTablesMap::GPUFrameRootTablesMap(UINT frameCount, std::vector<BindPointRootDescriptorPair>&& map, std::vector<std::vector<StateAndResource>>&& describedResources) :
    frameCount_(std::move(frameCount)), descriptorTable_(std::move(map)), describedResources_(describedResources)
{ }

GPUFrameRootTablesMap::GPUFrameRootTablesMap(GPUFrameRootTablesMap const& rhs) = default;

GPUFrameRootTablesMap::GPUFrameRootTablesMap(GPUFrameRootTablesMap&& rhs) = default;

GPUFrameRootTablesMap& GPUFrameRootTablesMap::operator=(GPUFrameRootTablesMap const& rhs) = default;

GPUFrameRootTablesMap& GPUFrameRootTablesMap::operator=(GPUFrameRootTablesMap&& rhs) = default;