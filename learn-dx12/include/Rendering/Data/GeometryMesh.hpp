#pragma once

#include <pch.hpp>

#include <map>
#include <Rendering\Data\GPUResourceFrameSet.hpp>

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

    GPUResourceFrameSet vertices;
    D3D12_VERTEX_BUFFER_VIEW verticesView;

    GPUResourceFrameSet indicies;
    D3D12_INDEX_BUFFER_VIEW indiciesView;

private:
    SubmeshMap submeshMap_;

}; // class GeometryMesh
