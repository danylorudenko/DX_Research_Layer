#pragma once

#include <pch.hpp>

#include <Rendering\Data\Resource\View\GPUResourceNativeView.hpp>

class GPURootMap
{
public:
    GPURootMap();
    GPURootMap(GPURootMap const& rhs);
    GPURootMap(GPURootMap&& rhs);

    GPURootMap& operator=(GPURootMap const& rhs);
    GPURootMap& operator=(GPURootMap&& rhs);

    void ImportTableBindPoint(GPUResourceNativeView const& view);
    void ImportResourceView(GPUResourceView const& view);

    GPUResourceNativeView const& BindPoint(int index) const;
    std::size_t BindPointCount() const;

    GPUResourceView const& RootResource(int index) const;
    std::size_t RootResourceCount() const;

private:
    std::vector<GPUResourceNativeView> rootTableBindPoints_;
    std::vector<GPUResourceView> rootResources_;
};