#include <Data\GPURootSignature.hpp>

GPURootSignature::GPURootSignature() = default;

GPURootSignature::GPURootSignature(Microsoft::WRL::ComPtr<ID3D12RootSignature>& signature) :
    rootSignature_(signature)
{

}

GPURootSignature::GPURootSignature(GPURootSignature&&) = default;

GPURootSignature& GPURootSignature::operator=(GPURootSignature&&) = default;

void GPURootSignature::SetRootSignature(GPUEngine* executionEngine)
{
    executionEngine->Commit().SetGraphicsRootSignature(rootSignature_.Get());
}


void GPURootSignature::TransitionRootResources(GPUEngine* executionEngine, UINT frameIndex)
{
    std::size_t const resourceNum = rootDescriptorTablesMap_.DescribedResourceCount(frameIndex);
    assert(resourceNum < 10 && "Resource count reached static limit.");

    D3D12_RESOURCE_BARRIER barriers[10];
    for (std::size_t i = 0; i < resourceNum; i++) {
        barriers[i] = CD3DX12_RESOURCE_BARRIER::Transition(
            rootDescriptorTablesMap_.DescribedResource(i)->Get(), rootDescriptorTablesMap_.DescribedResource(i)->State(),
            rootDescriptorTablesMap_.DescribedResourceState(i));
    }

    executionEngine->Commit().ResourceBarrier(resourceNum, barriers);
}