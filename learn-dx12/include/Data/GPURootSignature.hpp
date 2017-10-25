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

    void ImportResourcesSemantics(char const** semantics, UINT const semanticsNum);
    void ImportRootResources(GPUGraphResourceProxy const& resourceProxy);
    void ImportRootResourceStates(D3D12_RESOURCE_STATES const* states, UINT const statesNum);

    void ImportRootDescriptorBindings(GPUDescriptorReference const* descriptors, UINT const descriptorsNum);

    void SetRootSignature(GPUEngine* executionEngine);
    void SetRootSignatureDescriptorBindings(GPUEngine* executionEngine);
    void TransitionRootResources(GPUEngine* executionEngine);

private:
    Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_;

    std::vector<std::string> rootParamsSemantics_;
    std::vector<GPUResource*> rootResources_;
    std::vector<D3D12_RESOURCE_STATES> rootResourceStates_;

    std::vector<GPUDescriptorReference> rootDescritors_;
    //bool descriptorsLoaded_;
};