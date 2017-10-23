#pragma once

#include <pch.hpp>

class GPURootSignature
{
    GPURootSignature();
    GPURootSignature(Microsoft::WRL::ComPtr<ID3D12RootSignature>& signature);
    GPURootSignature(GPURootSignature const&) = delete;
    GPURootSignature(GPURootSignature&& rhs);

    GPURootSignature& operator=(GPURootSignature const&) = delete;
    GPURootSignature& operator=(GPURootSignature&& rhs);

private:
    Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_;
};