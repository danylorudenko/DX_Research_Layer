#include <Rendering\Data\GPURootMap.hpp>

GPURootMap::GPURootMap() = default;

GPURootMap::GPURootMap(GPURootMap const& rhs) = default;

GPURootMap::GPURootMap(GPURootMap&& rhs) = default;

GPURootMap& GPURootMap::operator=(GPURootMap const& rhs) = default;

GPURootMap& GPURootMap::operator=(GPURootMap&& rhs) = default;

void GPURootMap::ImportTableBindPoint(GPUResourceNativeView const& view)
{
    rootTableBindPoints_.push_back(view);
}

void GPURootMap::ImportResourceView(GPUResourceView const& view)
{
    rootResources_.push_back(view);
}

GPUResourceNativeView const& GPURootMap::BindPoint(int index) const
{
    return rootTableBindPoints_[index];
}

std::size_t GPURootMap::BindPointCount() const
{
    return rootTableBindPoints_.size();
}

GPUResourceView const& GPURootMap::RootResource(int index) const
{
    return rootResources_[index];
}

std::size_t GPURootMap::RootResourceCount() const
{
    return rootResources_.size();
}