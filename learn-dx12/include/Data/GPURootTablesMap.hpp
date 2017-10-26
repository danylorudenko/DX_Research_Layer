#pragma once

#include <pch.hpp>

#include <Data\GPUResourceDescriptor.hpp>

class GPURootTablesMap
{
public:
    using BindPointAndTableHandlePair = std::pair<UINT, GPUResourceDescriptor>;

    GPURootTablesMap();
    GPURootTablesMap(std::vector<BindPointAndTableHandlePair>&& map);
    GPURootTablesMap(GPURootTablesMap const&);
    GPURootTablesMap(GPURootTablesMap&&);

    GPURootTablesMap& operator=(GPURootTablesMap const&);
    GPURootTablesMap& operator=(GPURootTablesMap&&);

    D3D12_GPU_DESCRIPTOR_HANDLE TableStartGPUHandle() const;
    D3D12_CPU_DESCRIPTOR_HANDLE TableStartCPUHandle() const;

    UINT Size() const;

private:
    std::vector<BindPointAndTableHandlePair> descriptorTable_;
};