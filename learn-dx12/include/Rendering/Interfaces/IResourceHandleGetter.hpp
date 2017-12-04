#pragma once

#include <pch.hpp>

#include <Rendering\Data\GPUResource.hpp>

class IResourceHandleGetter
{
public:
    virtual Microsoft::WRL::ComPtr<ID3D12Resource>& GetResourceHandle(GPUResource& resource) = 0;
};