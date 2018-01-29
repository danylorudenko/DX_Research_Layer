#pragma once

#include <pch.hpp>

struct GPUSubmeshInfo
{
    UINT geometryLocation;
    UINT indexLocation;

}; // struct SubmeshInfo

class GPUGeometryMesh
{
public:
    using SubmeshMap = std::map<std::string, GPUSubmeshInfo>;

    //GPUFrameResource vertices;
    D3D12_VERTEX_BUFFER_VIEW verticesView;

    //GPUFrameResource indicies;
    D3D12_INDEX_BUFFER_VIEW indiciesView;

private:
    SubmeshMap submeshMap_;

}; // class GeometryMesh
