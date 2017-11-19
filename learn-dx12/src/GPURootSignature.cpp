#include <Rendering\Data\GPURootSignature.hpp>
#include <Rendering\GPUEngine.hpp>

GPURootSignature::GPURootSignature() = default;

GPURootSignature::GPURootSignature(Microsoft::WRL::ComPtr<ID3D12RootSignature>& signature) :
    rootSignature_(signature)
{

}

GPURootSignature::GPURootSignature(GPURootSignature&&) = default;

GPURootSignature& GPURootSignature::operator=(GPURootSignature&&) = default;

void GPURootSignature::ImportPassFrameRootDescriptorTable(GPUFrameRootTablesMap const& descriptorTable)
{
    passRootDescriptorTablesMap_ = descriptorTable;
}

void GPURootSignature::SetPassRootSignature(GPUEngine* executionEngine)
{
    executionEngine->Commit().SetGraphicsRootSignature(rootSignature_.Get());
}

void GPURootSignature::SetPassRootSignatureDescriptorTables(GPUEngine* executionEngine, UINT frameIndex)
{
    int const tablesCount = static_cast<int>(passRootDescriptorTablesMap_.TableSize());
    for (int i = 0; i < tablesCount; i++) {
        // Errr, I hope it should work like this.
        executionEngine->Commit().SetGraphicsRootDescriptorTable(i, passRootDescriptorTablesMap_.DescirptorTableGPUHandle(frameIndex, static_cast<UINT>(i)));
    }
}


void GPURootSignature::TransitionRootResources(GPUEngine* executionEngine, UINT frameIndex)
{
    int const resourceNum = static_cast<int>(passRootDescriptorTablesMap_.DescribedResourceCount(frameIndex));
    assert(resourceNum < 10 && "Resource count reached static limit.");

    D3D12_RESOURCE_BARRIER barriers[10];
    for (int i = 0; i < resourceNum; i++) {
        barriers[i] = CD3DX12_RESOURCE_BARRIER::Transition(
            passRootDescriptorTablesMap_.DescribedResource(frameIndex, static_cast<UINT>(i))->Get(), 
            passRootDescriptorTablesMap_.DescribedResource(frameIndex, static_cast<UINT>(i))->State(),
            passRootDescriptorTablesMap_.DescribedResourceTargetState(frameIndex, static_cast<UINT>(i)));
    }

    executionEngine->Commit().ResourceBarrier(resourceNum, barriers);
}