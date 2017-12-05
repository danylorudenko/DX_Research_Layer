#include <Rendering\Data\GPUFrameResource.hpp>

GPUFrameResource::GPUFrameResource() = default;

GPUFrameResource::GPUFrameResource(int framesCount, ID3D12Device* device, std::size_t size, D3D12_RESOURCE_DESC* resourceDesc, D3D12_RESOURCE_STATES state) :
    framesCount_(framesCount),
    states_(framesCount, state),
    size_(size),
    capacity_(size)
{
    for(int i = 0; i < framesCount; i++)
    {
        resources_.push_back(Microsoft::WRL::ComPtr<ID3D12Resource>{nullptr});
        auto const result = device->CreateCommittedResource(
            &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
            D3D12_HEAP_FLAG_NONE,
            resourceDesc,
            state,
            nullptr,
            IID_PPV_ARGS(&resources_[i])
        );
        ThrowIfFailed(result);
        gpuAddresses_.push_back(D3D12_GPU_VIRTUAL_ADDRESS{});
    }
}

GPUFrameResource::GPUFrameResource(int framesCount, std::size_t resourceSize, Microsoft::WRL::ComPtr<ID3D12Resource>* resources, D3D12_RESOURCE_STATES state) :
    framesCount_(framesCount),
    states_(framesCount, state),
    size_(resourceSize),
    capacity_(resourceSize)
    
{
    for (int i = 0; i < framesCount_; i++) {
        resources_.push_back(resources[i]);
        gpuAddresses_.push_back(D3D12_GPU_VIRTUAL_ADDRESS{});
    }
}

GPUFrameResource::GPUFrameResource(GPUFrameResource&& rhs) = default;

GPUFrameResource& GPUFrameResource::operator=(GPUFrameResource&& rhs) = default;

void GPUFrameResource::CreateResources(int framesCount, ID3D12Device* device, std::size_t size, D3D12_RESOURCE_DESC* resourceDesc, D3D12_RESOURCE_STATES initialState)
{
    resources_.clear();
    framesCount_ = framesCount;
    for (int i = 0; i < framesCount_; i++) {
        auto result = device->CreateCommittedResource(
            &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
            D3D12_HEAP_FLAG_NONE,
            &CD3DX12_RESOURCE_DESC::Buffer(size),
            initialState,
            nullptr,
            IID_PPV_ARGS(&resources_[i]));

        ThrowIfFailed(result);
        states_[i] = initialState;
        gpuAddresses_[i] = D3D12_GPU_VIRTUAL_ADDRESS{};
    }
    
    size_ = size;
    capacity_ = size;
}

D3D12_GPU_VIRTUAL_ADDRESS GPUFrameResource::GPUVirtualAddress(int frameIndex)
{
    return 
        gpuAddresses_[frameIndex] != 0 
        ? gpuAddresses_[frameIndex] 
        : gpuAddresses_[frameIndex] = resources_[frameIndex]->GetGPUVirtualAddress();
}

void GPUFrameResource::UpdateData(int frameIndex, ID3D12GraphicsCommandList* commandList, std::size_t offsetInDest, GPUFrameResource& src, int srcFrameIndex, std::size_t offsetInSrc, std::size_t numBytes)
{
    commandList->CopyBufferRegion(resources_[frameIndex].Get(), offsetInDest, src.Get(srcFrameIndex), offsetInSrc, numBytes);
}

void GPUFrameResource::Transition(int frameIndex, ID3D12GraphicsCommandList* commandList, D3D12_RESOURCE_STATES state)
{
    commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(resources_[frameIndex].Get(), states_[frameIndex], state));
    states_[frameIndex] = state;
}

D3D12_RESOURCE_STATES GPUFrameResource::State(int frameIndex) const
{
    return states_[frameIndex];
}
