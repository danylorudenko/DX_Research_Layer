#include <Data\GPURootSignature.hpp>

GPURootSignature::GPURootSignature() = default;

GPURootSignature::GPURootSignature(Microsoft::WRL::ComPtr<ID3D12RootSignature>& signature) :
    rootSignature_(signature)
{

}

GPURootSignature::GPURootSignature(GPURootSignature&&) = default;

GPURootSignature& GPURootSignature::operator=(GPURootSignature&&) = default;

void GPURootSignature::AppendRootParameterValue(char const* semantics, GPUDescriptorReference& descriptor)
{
    rootParamValues_.push_back(std::make_pair(semantics, descriptor));
}

void GPURootSignature::ClearRootParameterValues()
{
    rootParamValues_.clear();
}

void GPURootSignature::SetRootSignature(GPUEngine* executionEngine)
{
    executionEngine->Commit().SetGraphicsRootSignature(rootSignature_.Get());
}

void GPURootSignature::SetRootSignatureParams(GPUEngine* executionEngine)
{
    executionEngine->Commit().qwjksrgnqhj4rjdgq
}