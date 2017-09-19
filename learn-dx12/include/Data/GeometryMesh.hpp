#pragma once

#include <pch.hpp>

#include <map>

using Microsoft::WRL::ComPtr;

class GeometryMesh
{
    using SubmeshMap = std::map<std::string, SubgeometryInfo>;

private:
    ComPtr<ID3DBlob> geometryCPUHanle_;
    ComPtr<ID3DBlob> indexCPUHandle_;

    ComPtr<ID3D12Resource> geometryGPUHandle_;
    ComPtr<ID3D12Resource> indexGPUHandle_;

    SubmeshMap submeshMap_;

}; // class GeometryMesh

namespace
{

struct SubgeometryInfo
{
    UINT geometryLocation;
    UINT indexLocation;

}; // struct SubgeometryInfo

} // namespace