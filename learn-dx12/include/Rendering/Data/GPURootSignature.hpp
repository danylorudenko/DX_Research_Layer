#pragma once

#include <pch.hpp>

#include <Rendering\GPUEngine.hpp>
#include <Rendering\Data\GPUFrameResourceDescriptor.hpp>
#include <Rendering\Data\GPUFrameRootTablesMap.hpp>
#include <Rendering\Data\FrameGraph\GPUGraphNode.hpp>

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

    // This is done during graph setup.
    void ImportPassFrameRootDescriptorTable(GPUFrameRootTablesMap const& descriptorTable);
    
    void SetPassRootSignature(GPUEngine* executionEngine);
    void SetPassRootSignatureDescriptorTables(GPUEngine* executionEngine, UINT frameIndex);
    void TransitionRootResources(GPUEngine* executionEngine, UINT frameIndex);

private:
    Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_;

    // Actually, collection of descriptors on the start of the table and resources these tables describe.
    GPUFrameRootTablesMap passRootDescriptorTablesMap_;
};