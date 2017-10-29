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

    // This is done during graph setup.
    void ImportRootDescriptorTables(GPURootTablesMap descriptorTables);

    void SetRootSignature(GPUEngine* executionEngine);
    void SetRootSignatureDescriptorTables(GPUEngine* executionEngine, UINT frameIndex);
    void TransitionRootResources(GPUEngine* executionEngine, UINT frameIndex);

private:
    Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_;

    // Actually, collection of descriptors on the start of the table.
    GPURootTablesMap rootDescriptorTablesMap_;

    std::vector<GPUResourceDescriptor> renderTargets_;
    //bool descriptorsLoaded_;
};