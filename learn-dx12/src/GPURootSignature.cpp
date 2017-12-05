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

void GPURootSignature::BindPassRootSignature(GPUEngine* executionEngine)
{
    executionEngine->Commit().SetGraphicsRootSignature(rootSignature_.Get());
}

void GPURootSignature::BindPassRootSignatureDescriptorTables(GPUEngine* executionEngine, int frameIndex)
{
    ID3D12DescriptorHeap* descriptorHeaps[] = { passRootDescriptorTablesMap_.ParentHeap() };
    executionEngine->Commit().SetDescriptorHeaps(1, descriptorHeaps);
    
    int const tablesCount = static_cast<int>(passRootDescriptorTablesMap_.TableSize());
    for (int i = 0; i < tablesCount; i++) {
        // Errr, I hope it should work like this.
        executionEngine->Commit().SetGraphicsRootDescriptorTable(i, passRootDescriptorTablesMap_.DescirptorTableGPUHandle(frameIndex, static_cast<UINT>(i)));
    }
}


void GPURootSignature::TransitionRootResources(GPUEngine* executionEngine, int frameIndex)
{
    int const resourceNum = static_cast<int>(passRootDescriptorTablesMap_.DescribedResourceCount(frameIndex));
    assert(resourceNum < 10 && "Resource count reached static limit.");

    for (int i = 0; i < resourceNum; i++) {
        if (passRootDescriptorTablesMap_.DescribedResourceCurrentState(frameIndex, i) != passRootDescriptorTablesMap_.DescribedResourceTargetState(i)) {
            passRootDescriptorTablesMap_.DescribedResource(i)->Transition(frameIndex, executionEngine->CommandList(), passRootDescriptorTablesMap_.DescribedResourceTargetState(i));
        }
    }
}