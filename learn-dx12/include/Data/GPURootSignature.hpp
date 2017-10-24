#pragma once

#include <pch.hpp>

#include <Data\GPUDescriptorReference.hpp>

class GPURootSignature
{
    GPURootSignature();
    GPURootSignature(Microsoft::WRL::ComPtr<ID3D12RootSignature>& signature);
    GPURootSignature(GPURootSignature const&) = delete;
    GPURootSignature(GPURootSignature&& rhs);

    GPURootSignature& operator=(GPURootSignature const&) = delete;
    GPURootSignature& operator=(GPURootSignature&& rhs);

    ID3D12RootSignature* Get() const { return rootSignature_.Get(); }

private:
    Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_;

    std::map<std::string, GPUDescriptorReference> rootParamValues_;
};