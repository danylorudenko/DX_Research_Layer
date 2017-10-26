#pragma once

#include <pch.hpp>

#include <Rendering\GPUEngine.hpp>
#include <Data\GPURootTablesMap.hpp>
#include <Data\GPUGraphNode.hpp>

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
    void ImportRootResources(GPUGraphNode const& resourceProxy);

    void ImportRootDescriptorTables(std::vector<GPURootTablesMap>&& descriptorTables);

    void SetRootSignature(GPUEngine* executionEngine);
    void SetRootSignatureDescriptorTables(GPUEngine* executionEngine);
    void TransitionRootResources(GPUEngine* executionEngine);

private:
    Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_;

    std::vector<std::string> rootResourcesSemantics_;
    std::vector<GPUResourceDescriptor> rootResourseDescriptors_;
    std::vector<GPURootTablesMap> rootDescriptorTablesMap_;
    //bool descriptorsLoaded_;
};