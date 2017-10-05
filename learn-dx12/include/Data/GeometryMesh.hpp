#pragma once

#include <pch.hpp>

#include <map>
#include <Data\GPUResource.hpp>

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
    GPUResource indicies;
    SubmeshMap submeshMap_;

}; // class GeometryMesh
