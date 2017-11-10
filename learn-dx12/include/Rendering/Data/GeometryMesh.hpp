#pragma once

#include <pch.hpp>

#include <map>
#include <Rendering\Data\GPUResource.hpp>

namespace
{
struct SubmeshInfo
{
    UINT geometryLocation;
    UINT indexLocation;

}; // struct SubmeshInfo
} // namespace

class GeometryMesh
{
public:
    using SubmeshMap = std::map<std::string, SubmeshInfo>;

    GPUResource vertices;
    D3D12_VERTEX_BUFFER_VIEW verticesView;

    GPUResource indicies;
    D3D12_INDEX_BUFFER_VIEW indiciesView;

private:
    SubmeshMap submeshMap_;

}; // class GeometryMesh
