#pragma once

#include <pch.hpp>

#include <Rendering\Data\Resource\GPUResourceHandle.hpp>

namespace DXRL
{

enum GPU_SHADER_VISIBLE_RESOURCE_VIEW_TYPE
{
    GPU_SHADER_VISIBLE_RESOURCE_VIEW_TYPE_CBV,
    GPU_SHADER_VISIBLE_RESOURCE_VIEW_TYPE_SRV,
    GPU_SHADER_VISIBLE_RESOURCE_VIEW_TYPE_UAV
};

struct GPUShaderVisibleResourceViewDesc
{
    std::size_t frames_;
    GPUResourceHandle* resources_;
    D3D12_RESOURCE_STATES targetState_;

    GPU_SHADER_VISIBLE_RESOURCE_VIEW_TYPE type_;
    union {
        D3D12_CONSTANT_BUFFER_VIEW_DESC* cbvDescs_;
        D3D12_SHADER_RESOURCE_VIEW_DESC* srvDescs_;
        D3D12_UNORDERED_ACCESS_VIEW_DESC* uavDescs_;
    };
};

class GPUResourceViewTable
{
public:
    GPUResourceViewTable();
    GPUResourceViewTable(std::size_t viewCount, GPUResourceViewHandle* views);
    GPUResourceViewTable(GPUResourceViewTable const& rhs);
    GPUResourceViewTable(GPUResourceViewTable&& rhs);

    GPUResourceViewTable& operator=(GPUResourceViewTable const& rhs);
    GPUResourceViewTable& operator=(GPUResourceViewTable&& rhs);

    GPUResourceViewHandle& TableMember(std::size_t memberIndex);
    std::size_t Size() const;
    D3D12_GPU_DESCRIPTOR_HANDLE GPUHandle(std::size_t frameIndex);

private:
    std::vector<GPUResourceViewHandle> tableMembers_;
};

}