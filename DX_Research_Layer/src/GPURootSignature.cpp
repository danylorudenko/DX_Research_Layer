#include <Rendering\Data\GPURootSignature.hpp>
#include <Rendering\GPUEngine.hpp>
#include <Rendering\Data\Resource\ResourceView\GPUResourceView.hpp>

namespace DXRL
{

GPURootSignature::GPURootSignature() = default;

GPURootSignature::GPURootSignature(Microsoft::WRL::ComPtr<ID3D12RootSignature>& signature) :
    rootSignature_(signature)
{

}

GPURootSignature::GPURootSignature(GPURootSignature&&) = default;

GPURootSignature& GPURootSignature::operator=(GPURootSignature&&) = default;

void GPURootSignature::PushRootArgument(std::size_t bindSlot, GPUResourceViewTable const& table)
{
    rootArguments_.push_back(GPURootArgument{ bindSlot, table });
}

void GPURootSignature::BindPassRootSignature(GPUEngine* executionEngine)
{
    executionEngine->Commit().SetGraphicsRootSignature(rootSignature_.Get());
}

void GPURootSignature::BindPassDescriptorTables(GPUEngine* executionEngine, std::size_t frameIndex)
{
    //ID3D12DescriptorHeap* descriptorHeaps[] = { passRootDescriptorTablesMap_.ParentHeap() };
    //executionEngine->Commit().SetDescriptorHeaps(1, descriptorHeaps);

    auto const tablesCount = rootArguments_.size();
    for (std::size_t i = 0; i < tablesCount; i++) {
        executionEngine->Commit().SetGraphicsRootDescriptorTable(static_cast<UINT>(rootArguments_[i].bindSlot_), rootArguments_[i].table_.GPUHandle(frameIndex));
    }
}


void GPURootSignature::TransitionRootResources(GPUEngine* executionEngine, std::size_t frameIndex)
{
    D3D12_RESOURCE_BARRIER transitions[10];
    std::size_t transitionsCounter = 0;

    auto const argCount = rootArguments_.size();
    for (size_t i = 0; i < argCount; i++) {
        auto const tableSize = rootArguments_[i].table_.Size();
        for (size_t j = 0; j < tableSize; j++) {
            auto& rootView = rootArguments_[i].table_.TableMember(j).View(frameIndex);
            if (rootView.Resource().State() != rootView.TargetState()) {
                rootView.Resource().PrepareTransition(rootView.TargetState(), transitions[transitionsCounter++]);
            }
        }
    }

    if (transitionsCounter > 0) {
        executionEngine->Commit().ResourceBarrier(static_cast<UINT>(transitionsCounter), transitions);
    }
}

}