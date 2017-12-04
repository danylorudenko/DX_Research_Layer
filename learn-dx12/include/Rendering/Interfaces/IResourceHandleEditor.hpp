#pragma once

#include <pch.hpp>

#include <Rendering\Data\GPUResource.hpp>

class IResourceHandleEditor
{
public:
    virtual Microsoft::WRL::ComPtr<ID3D12Resource>& GetResourceHandleRef(GPUResource& resource) = 0;
};