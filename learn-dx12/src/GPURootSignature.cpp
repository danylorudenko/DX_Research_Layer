#include <Data\GPURootSignature.hpp>

GPURootSignature::GPURootSignature() = default;

GPURootSignature::GPURootSignature(Microsoft::WRL::ComPtr<ID3D12RootSignature>& signature) :
    rootSignature_(signature)
{

}

GPURootSignature::GPURootSignature(GPURootSignature&&) = default;

GPURootSignature& GPURootSignature::operator=(GPURootSignature&&) = default;

void GPURootSignature::ImportResourcesSemantics(char const** semantics, UINT const semanticsNum)
{
    rootParamsSemantics_.clear();
    for (size_t i = 0; i < semanticsNum; i++) {
        rootParamsSemantics_.push_back(*(semantics + i));
    }
}

void GPURootSignature::ImportRootResources(GPUGraphResourceProxy const& resourceProxy)
{
    std::size_t const resourceNum = rootParamsSemantics_.size();
    for (size_t i = 0; i < resourceNum; i++) {
        auto* resource = resourceProxy.Request(rootParamsSemantics_[i].c_str());
        assert(resource != nullptr && "Root signature can't import requested resource.");

        rootResources_.push_back(resource);
    }
}

void GPURootSignature::ImportRootDescriptorBindings(GPUDescriptorReference const* descriptors, UINT const descriptorNum)
{
    for (size_t i = 0; i < descriptorNum; i++) {
        rootDescritors_.push_back(descriptors[i]);
    }
}

void GPURootSignature::ImportRootResourceStates(D3D12_RESOURCE_STATES const* states, UINT const statesNum)
{
    for (size_t i = 0; i < statesNum; i++) {
        rootResourceStates_.push_back(states[i]);
    }
}

void GPURootSignature::SetRootSignature(GPUEngine* executionEngine)
{
    executionEngine->Commit().SetGraphicsRootSignature(rootSignature_.Get());
}

void GPURootSignature::SetRootSignatureDescriptorBindings(GPUEngine* executionEngine)
{
    std::size_t const paramNum = rootDescritors_.size();
    for (size_t i = 0; i < paramNum; i++) {
        executionEngine->Commit().SetGraphicsRootDescriptorTable(i, rootDescritors_[i].GPUHandle());
    }
}

void GPURootSignature::TransitionRootResources(GPUEngine* executionEngine)
{
    std::size_t const resourceNum = rootResources_.size();
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