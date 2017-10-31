#include <Data\GPURootSignature.hpp>

GPURootSignature::GPURootSignature() = default;

GPURootSignature::GPURootSignature(Microsoft::WRL::ComPtr<ID3D12RootSignature>& signature) :
    rootSignature_(signature)
{

}

GPURootSignature::GPURootSignature(GPURootSignature&&) = default;

GPURootSignature& GPURootSignature::operator=(GPURootSignature&&) = default;

void GPURootSignature::ImportFrameRootDescriptorTable(GPUFrameRootTablesMap const& descriptorTable)
{
    rootDescriptorTablesMap_ = descriptorTable;
}

void GPURootSignature::SetRootSignature(GPUEngine* executionEngine)
{
    executionEngine->Commit().SetGraphicsRootSignature(rootSignature_.Get());
}

void GPURootSignature::SetRootSignatureDescriptorTables(GPUEngine* executionEngine, UINT frameIndex)
{
    auto const tablesCount = rootDescriptorTablesMap_.TableSize();
    for (size_t i = 0; i < tablesCount; i++) {
        // Errr, I hope it should work like this.
        executionEngine->Commit().SetGraphicsRootDescriptorTable(i, rootDescriptorTablesMap_.DescirptorTableGPUHandle(frameIndex, i));
    }
}


void GPURootSignature::TransitionRootResources(GPUEngine* executionEngine, UINT frameIndex)
{
    std::size_t const resourceNum = rootDescriptorTablesMap_.DescribedResourceCount(frameIndex);
    assert(resourceNum < 10 && "Resource count reached static limit.");

    D3D12_RESOURCE_BARRIER barriers[10];
    for (std::size_t i = 0; i < resourceNum; i++) {
        barriers[i] = CD3DX12_RESOURCE_BARRIER::Transition(
            rootDescriptorTablesMap_.DescribedResource(frameIndex, i)->Get(), 
            rootDescriptorTablesMap_.DescribedResource(frameIndex, i)->State(),
            rootDescriptorTablesMap_.DescribedResourceTargetState(frameIndex, i));
    }

    executionEngine->Commit().ResourceBarrier(resourceNum, barriers);
}