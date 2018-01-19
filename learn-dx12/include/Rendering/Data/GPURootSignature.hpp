#pragma once

#include <pch.hpp>

#include <Rendering\Data\Resource\ResourceView\GPUResourceViewTable.hpp>

class GPUEngine;

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
    void PushRootArgument(std::size_t bindSlot, GPUResourceViewTable const& table);
    
    void BindPassRootSignature(GPUEngine* executionEngine);
    void BindPassDescriptorTables(GPUEngine* executionEngine, std::size_t frameIndex);
    void TransitionRootResources(GPUEngine* executionEngine, std::size_t frameIndex);

private:
    Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_;

    struct GPURootArgument
    {
        std::size_t bindSlot_;
        GPUResourceViewTable table_;
    };
    std::vector<GPURootArgument> rootArguments_;
};