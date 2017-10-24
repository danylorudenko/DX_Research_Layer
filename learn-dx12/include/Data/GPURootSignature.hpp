#pragma once

#include <pch.hpp>

#include <Rendering\GPUEngine.hpp>
#include <Data\GPUResource.hpp>
#include <Data\GPUDescriptorReference.hpp>
#include <Data\GPUGraphResourceProxy.hpp>

class GPURootSignature
{
public:
    GPURootSignature();
    GPURootSignature(Microsoft::WRL::ComPtr<ID3D12RootSignature>& signature);
    GPURootSignature(GPURootSignature const&) = delete;
    GPURootSignature(GPURootSignature&& rhs);

    GPURootSignature& operator=(GPURootSignature const&) = delete;
    GPURootSignature& operator=(GPURootSignature&& rhs);

    ID3D12RootSignature* Get() const { return rootSignature_.Get(); }

    void ImportResourcesSemantics(char const** semantics, UINT semanticsNum);
    void ImportRootResources(GPUGraphResourceProxy& resourceProxy);
    void ImportRootDescriptors(GPUDescriptorReference* descriptors, UINT descriptorsNum);
    void ImportRootResourceStates(D3D12_RESOURCE_STATES states, UINT statesNum);

    void SetRootSignature(GPUEngine* executionEngine);
    void SetRootSignatureDescriptors(GPUEngine* executionEngine);
    void TransitionRootResources(GPUEngine* executionEngine);

private:
    Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_;

    std::vector<std::string> rootParamsSemantics_;
    std::vector<GPUResource*> rootResources_;
    std::vector<D3D12_RESOURCE_STATES> rootResourceStates_;

    std::vector<GPUDescriptorReference> rootDescritors_;
    //bool descriptorsLoaded_;
};