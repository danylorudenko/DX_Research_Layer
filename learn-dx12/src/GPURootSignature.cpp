#include <Data\GPURootSignature.hpp>

GPURootSignature::GPURootSignature() = default;

GPURootSignature::GPURootSignature(Microsoft::WRL::ComPtr<ID3D12RootSignature>& signature) :
    rootSignature_(signature)
{

}

GPURootSignature::GPURootSignature(GPURootSignature&&) = default;

GPURootSignature& GPURootSignature::operator=(GPURootSignature&&) = default;

void GPURootSignature::ImportResourcesSemantics(char const** semantics, UINT semanticsNum)
{
    rootParamsSemantics_.clear();
    for (size_t i = 0; i < semanticsNum; i++) {
        rootParamsSemantics_.push_back(*(semantics + i));
    }
}

void GPURootSignature::SetRootSignature(GPUEngine* executionEngine)
{
    executionEngine->Commit().SetGraphicsRootSignature(rootSignature_.Get());
}

void GPURootSignature::SetRootSignatureDescriptors(GPUEngine* executionEngine)
{
    std::size_t paramNum = rootDescritors_.size();
    for (size_t i = 0; i < paramNum; i++) {
        executionEngine->Commit().SetGraphicsRootDescriptorTable(i, rootDescritors_[i].GPUHandle());
    }
}

void GPURootSignature::TransitionRootResources(GPUEngine* executionEngine)
{
    std::size_t resourceNum = rootResources_.size();
    assert(resourceNum == rootResourceStates_.size() && "Resource count and states count aren't equal");
    assert(resourceNum < 10 && "Resource count reached static limit.");

    D3D12_RESOURCE_BARRIER barriers[10];
    for (size_t i = 0; i < resourceNum; i++) {
        barriers[i] = CD3DX12_RESOURCE_BARRIER::Transition(
            rootResources_[i]->Get(), rootResources_[i]->State(),
            rootResourceStates_[i]);
    }

    executionEngine->Commit().ResourceBarrier(resourceNum, barriers);
}