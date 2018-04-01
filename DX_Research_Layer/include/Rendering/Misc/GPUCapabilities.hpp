#pragma once

#include <pch.hpp>

namespace DXRL
{

class GPUDelegate;

class GPUCapabilities
{
public:
    GPUCapabilities() = delete;
    GPUCapabilities(GPUCapabilities const&) = delete;
    GPUCapabilities(GPUCapabilities&&) = delete;

    GPUCapabilities& operator=(GPUCapabilities const&) = delete;
    GPUCapabilities& operator=(GPUCapabilities&&) = delete;

    static std::wstring ListAdapters(GPUDelegate& gpuAccess);

    static Microsoft::WRL::ComPtr<ID3D12Device> GenerateStandardDeviceQuery(GPUDelegate& gpuAccess);
};

}