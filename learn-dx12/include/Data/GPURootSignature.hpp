#pragma once

#include <pch.hpp>

#include <Rendering\GPUEngine.hpp>
#include <Data\GPUDescriptorReference.hpp>

class GPURootSignature
{
public:
    using OrderedNamedList = std::vector<std::pair<std::string, GPUDescriptorReference>>;

    GPURootSignature();
    GPURootSignature(Microsoft::WRL::ComPtr<ID3D12RootSignature>& signature);
    GPURootSignature(GPURootSignature const&) = delete;
    GPURootSignature(GPURootSignature&& rhs);

    GPURootSignature& operator=(GPURootSignature const&) = delete;
    GPURootSignature& operator=(GPURootSignature&& rhs);

    ID3D12RootSignature* Get() const { return rootSignature_.Get(); }

    void AppendRootParameterValue(char const* semantics, GPUDescriptorReference& descriptor);
    void ClearRootParameterValues();

    void SetRootSignature(GPUEngine* executionEngine);
    void SetRootSignatureParams(GPUEngine* executionEngine);

private:
    Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_;
    OrderedNamedList rootParamValues_;
};