#include <Rendering\Data\GPUResourceViewAllocator.hpp>

GPUResourceViewAllocator::GPUResourceViewAllocator() = default;

GPUResourceViewAllocator::GPUResourceViewAllocator(Microsoft::WRL::ComPtr<ID3D12Device> device,
                                               int rtvCapacity,
                                               int dsvCapacity,
                                               int CbvSrvUavCapacity)
{
    device_ = device;
    
    heapRtvIncrementalSize_ = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    heapDsvIncrementalSize_ = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
    heapCbvSrvUavIncrementalSize_ = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

    heapCapacityRtv_ = rtvCapacity;
    heapCapacityDsv_ = dsvCapacity;
    heapCapacityCbvSrvUav_ = CbvSrvUavCapacity;

    //////////////////////////////

    D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};
    rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    rtvHeapDesc.NumDescriptors = heapCapacityRtv_;
    rtvHeapDesc.NodeMask = 0;
    rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    {
        HRESULT result = device_->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&heapRtv_));
        ThrowIfFailed(result);
    }

    //////////////////////////////
    
    D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
    dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
    dsvHeapDesc.NumDescriptors = heapCapacityDsv_;
    dsvHeapDesc.NodeMask = 0;
    dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    {
        HRESULT result = device_->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&heapDsv_));
        ThrowIfFailed(result);
    }

    //////////////////////////////

    D3D12_DESCRIPTOR_HEAP_DESC srvUavCbvHeapDesc{};
    srvUavCbvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    srvUavCbvHeapDesc.NumDescriptors = heapCapacityCbvSrvUav_;
    srvUavCbvHeapDesc.NodeMask = 0;
    srvUavCbvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    {
        HRESULT result = device_->CreateDescriptorHeap(&srvUavCbvHeapDesc, IID_PPV_ARGS(&heapCbvSrvUav_));
        ThrowIfFailed(result);
    }
}

GPUResourceViewAllocator::GPUResourceViewAllocator(GPUResourceViewAllocator&& rhs) = default;

GPUResourceViewAllocator& GPUResourceViewAllocator::operator=(GPUResourceViewAllocator&& rhs) = default;

void GPUResourceViewAllocator::Reset()
{
    heapRtvLastDescriptorOffset_ = 0;
    heapDsvLastDescriptorOffset_ = 0;
    heapCbvSrvUavLastDescriptorOffset_ = 0;
}

GPUFrameResourceView GPUResourceViewAllocator::AllocRtvLinear(GPUFrameResource* resources,
                                                             D3D12_RENDER_TARGET_VIEW_DESC* viewDesc,
                                                             D3D12_RESOURCE_STATES state,
                                                             char const* semantics,
                                                             int frameCount)
{
    assert(frameCount <= 3 && "More frames for descriptors is not currently allowed.");
    
    int descriptorsOffsets[3];
    for (int i = 0; i < frameCount; i++) {
        CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescriptorHandle{ heapRtv_->GetCPUDescriptorHandleForHeapStart(), heapRtvLastDescriptorOffset_, heapRtvIncrementalSize_ };
        device_->CreateRenderTargetView(resources->Get(i), viewDesc, cpuDescriptorHandle);
        descriptorsOffsets[i] = heapRtvLastDescriptorOffset_++;
    }

    GPUFrameResourceView rtvDescriptor{ frameCount, heapRtv_, heapRtvIncrementalSize_, descriptorsOffsets, state, semantics, resources };
    return rtvDescriptor;
}

GPUFrameResourceView GPUResourceViewAllocator::AllocDsvLinear(GPUFrameResource* resources,
                                                             D3D12_DEPTH_STENCIL_VIEW_DESC* viewDesc,
                                                             D3D12_RESOURCE_STATES state,
                                                             char const* semantics,
                                                             int frameCount)
{
    assert(frameCount <= 3 && "More frames for descriptors is not currently allowed.");

    int descriptorOffsets[3];
    for (int i = 0; i < frameCount; i++) {
        CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescriptorHandle{ heapDsv_->GetCPUDescriptorHandleForHeapStart(), heapDsvLastDescriptorOffset_, heapDsvIncrementalSize_ };
        device_->CreateDepthStencilView(resources->Get(i), viewDesc, cpuDescriptorHandle);
        descriptorOffsets[i] = heapDsvLastDescriptorOffset_++;
    }

    GPUFrameResourceView dsvDescriptor{ frameCount, heapDsv_, heapDsvIncrementalSize_, descriptorOffsets, state, semantics, resources };
    return dsvDescriptor;
}

GPUFrameResourceView GPUResourceViewAllocator::AllocCbvLinear(GPUFrameResource* resources,
                                                             D3D12_CONSTANT_BUFFER_VIEW_DESC* viewDesc,
                                                             D3D12_RESOURCE_STATES state,
                                                             char const* semantics,
                                                             int frameCount)
{
    assert(frameCount <= 3 && "More frames for descriptors is not currently allowed.");

    int descriptorOffsets[3];
    for (int i = 0; i < frameCount; i++) {
        CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescriptorHandle{ heapCbvSrvUav_->GetCPUDescriptorHandleForHeapStart(), heapCbvSrvUavLastDescriptorOffset_, heapCbvSrvUavIncrementalSize_ };
        device_->CreateConstantBufferView(viewDesc, cpuDescriptorHandle);
        descriptorOffsets[i] = heapCbvSrvUavLastDescriptorOffset_++;
    }

    GPUFrameResourceView cbvDescriptor{ frameCount, heapCbvSrvUav_, heapCbvSrvUavIncrementalSize_, descriptorOffsets, state, semantics, resources };
    return cbvDescriptor;
}

GPUFrameResourceView GPUResourceViewAllocator::AllocSrvLinear(GPUFrameResource* resources,
                                                             D3D12_SHADER_RESOURCE_VIEW_DESC* viewDesc,
                                                             D3D12_RESOURCE_STATES state,
                                                             char const* semantics,
                                                             int frameCount)
{
    assert(frameCount <= 3 && "More frames for descriptors is not currently allowed.");

    int descriptorOffsets[3];
    for (int i = 0; i < frameCount; i++) {
        CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescriptorHandle{ heapCbvSrvUav_->GetCPUDescriptorHandleForHeapStart(), heapCbvSrvUavLastDescriptorOffset_, heapCbvSrvUavIncrementalSize_ };
        device_->CreateShaderResourceView(resources->Get(i), viewDesc, cpuDescriptorHandle);
        descriptorOffsets[i] = heapCbvSrvUavLastDescriptorOffset_++;
    }

    GPUFrameResourceView srvDescriptor{ frameCount, heapCbvSrvUav_, heapCbvSrvUavIncrementalSize_, descriptorOffsets, state, semantics, resources };
    return srvDescriptor;
}

GPUFrameResourceView GPUResourceViewAllocator::AllocUavLinear(GPUFrameResource* resources,
                                                             D3D12_UNORDERED_ACCESS_VIEW_DESC* viewDesc,
                                                             D3D12_RESOURCE_STATES state,
                                                             char const* semantics,
                                                             int frameCount)
{
    assert(frameCount <= 3 && "More frames for descriptors in not currently allowed.");

    int descriptorOffsets[3];
    for (int i = 0; i < frameCount; i++) {
        CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescriptorHandle{ heapCbvSrvUav_->GetCPUDescriptorHandleForHeapStart(), heapCbvSrvUavLastDescriptorOffset_, heapCbvSrvUavIncrementalSize_ };
        device_->CreateUnorderedAccessView(resources->Get(i), nullptr, viewDesc, cpuDescriptorHandle);
        descriptorOffsets[i] = heapCbvSrvUavLastDescriptorOffset_++;
    }

    GPUFrameResourceView uavDescriptor{ frameCount, heapCbvSrvUav_, heapCbvSrvUavIncrementalSize_, descriptorOffsets, state, semantics, resources };
    return uavDescriptor;
}