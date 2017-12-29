#pragma once

#include <pch.hpp>

class GPUFoundation;

class GPUCapabilities
{
public:
    GPUCapabilities();
    GPUCapabilities(GPUCapabilities const&) = delete;
    GPUCapabilities(GPUCapabilities&& rhs);

    GPUCapabilities& operator=(GPUCapabilities const&) = delete;
    GPUCapabilities& operator=(GPUCapabilities&& rhs);

    std::wstring ListAdapters(GPUFoundation& gpuAccess);

    Microsoft::WRL::ComPtr<ID3D12Device> GenerateStandardDeviceQuery(GPUFoundation& gpuAccess);
};