#include <Rendering\Data\Resource\ResourceView\GPUResourceViewHeap.hpp>
#include <Rendering\GPUFoundation.hpp>

GPUResourceViewHeap::GPUResourceViewHeap() = default;

GPUResourceViewHeap::GPUResourceViewHeap(GPUFoundation& foundation, D3D12_DESCRIPTOR_HEAP_TYPE type, std::size_t capacity) :
    type_{ type }, heapCapacity_{ capacity }
{
    auto& device = foundation.Device();

    descriptorSize_ = device->GetDescriptorHandleIncrementSize(type_);

    D3D12_DESCRIPTOR_HEAP_DESC heapDesc;
    heapDesc.NumDescriptors = static_cast<UINT>(heapCapacity_);
    heapDesc.Type = type_;
    heapDesc.NodeMask = 0;
    heapDesc.Flags = (type == type == D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) ? D3D12_DESCRIPTOR_HEAP_FLAG_NONE : D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
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

std::size_t GPUResourceViewHeap::ProvideNextOffset()
{
    return currentHeapOffset_;
}

D3D12_CPU_DESCRIPTOR_HANDLE GPUResourceViewHeap::CPUHeapStart() const
{
    return heapHandle_->GetCPUDescriptorHandleForHeapStart();
}

D3D12_GPU_DESCRIPTOR_HANDLE GPUResourceViewHeap::GPUHeapStart() const
{
    assert(type_ == D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV && "GPU handle can't be obtained from non shader-visible heap.");
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

ID3D12DescriptorHeap* GPUResourceViewHeap::Get() const
{
    return heapHandle_.Get();
}

void GPUResourceViewHeap::Reset()
{
    currentHeapOffset_ = 0;
}