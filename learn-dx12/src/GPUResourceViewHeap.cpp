#include <Rendering\Data\Resource\ResourceView\GPUResourceViewHeap.hpp>

GPUResourceViewHeap::GPUResourceViewHeap() = default;

GPUResourceViewHeap::GPUResourceViewHeap(GPUFoundation const& foundation, D3D12_DESCRIPTOR_HEAP_TYPE type, std::size_t capacity) :
    type_{ type }, heapCapacity_{ capacity }
{
    auto& device = foundation.Device();

    descriptorSize_ = device->GetDescriptorHandleIncrementSize(type_);

    D3D12_DESCRIPTOR_HEAP_DESC heapDesc;
    heapDesc.NumDescriptors = static_cast<UINT>(heapCapacity_);
    heapDesc.Type = type_;
    heapDesc.NodeMask = 0;
    heapDesc.Flags = (type == D3D12_DESCRIPTOR_HEAP_TYPE_RTV || type == D3D12_DESCRIPTOR_HEAP_TYPE_DSV) ? D3D12_DESCRIPTOR_HEAP_FLAG_NONE : D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    {
        auto const result = device->CreateDescriptorHeap(
            &heapDesc,
            IID_PPV_ARGS(heapHandle_.GetAddressOf())
        );
        ThrowIfFailed(result);
    }
}

GPUResourceViewHeap::GPUResourceViewHeap(GPUResourceViewHeap&& rhs) = default;

GPUResourceViewHeap& GPUResourceViewHeap::operator=(GPUResourceViewHeap&& rhs) = default;

D3D12_CPU_DESCRIPTOR_HANDLE GPUResourceViewHeap::ProvideNextHandle()
{
    return CD3DX12_CPU_DESCRIPTOR_HANDLE{ heapHandle_->GetCPUDescriptorHandleForHeapStart(), static_cast<INT>(currentHeapOffset_++), static_cast<UINT>(descriptorSize_) };
}

D3D12_CPU_DESCRIPTOR_HANDLE GPUResourceViewHeap::CPUHeapStart() const
{
    return heapHandle_->GetCPUDescriptorHandleForHeapStart();
}

D3D12_GPU_DESCRIPTOR_HANDLE GPUResourceViewHeap::GPUHeapStart() const
{
    return heapHandle_->GetGPUDescriptorHandleForHeapStart();
}

std::size_t GPUResourceViewHeap::Capacity() const
{
    return heapCapacity_;
}

std::size_t GPUResourceViewHeap::DescriptorSize() const
{
    return descriptorSize_;
}

void GPUResourceViewHeap::Reset()
{
    currentHeapOffset_ = 0;
}