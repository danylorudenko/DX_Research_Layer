#include <Data\GPUFrameResource.hpp>

GPUFrameResource::GPUFrameResource() = default;

GPUFrameResource::GPUFrameResource(ID3D12Device* device,
    Microsoft::WRL::ComPtr<ID3D12Resource> renderTarget, UINT width, UINT height,
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtvSharedHeap, INT offsetInRtvHeap,
    DXGI_FORMAT depthStencilFormat,
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsvSharedHeap, INT offsetInDsvHeap) :

    sharedRtvDescriptorHeap_(rtvSharedHeap),
    offsetInRtvHeap_(offsetInRtvHeap),
    sharedDsvDescriptorHeap_(dsvSharedHeap),
    offsetInDsvHeap_(offsetInDsvHeap)
{
    // Render target resource and view.
    resource_ = renderTarget;
    device->CreateRenderTargetView(resource_.Get(), nullptr, CPURtvDescriptorHandle());

    // Depth/Stencil resource and view creation.
    D3D12_RESOURCE_DESC dsDesc {};
    dsDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    dsDesc.Width = width;
    dsDesc.Height = height;
    dsDesc.Format = depthStencilFormat;
    dsDesc.Alignment = 0;
    dsDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
    dsDesc.MipLevels = 1;
    dsDesc.DepthOrArraySize = 1;
    dsDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

    dsDesc.SampleDesc.Count = 1;
    dsDesc.SampleDesc.Quality = 0;
    
    {
        auto result = device->CreateCommittedResource(
            &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
            D3D12_HEAP_FLAG_NONE,
            &dsDesc,
            D3D12_RESOURCE_STATE_DEPTH_WRITE,
            nullptr,
            IID_PPV_ARGS(&depthStencilBuffer_));
        ThrowIfFailed(result);
    }

    D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc {};
    dsvDesc.Texture2D.MipSlice = 0;
    dsvDesc.Flags = D3D12_DSV_FLAG_NONE;
    dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
    dsvDesc.Format = depthStencilFormat;

    device->CreateDepthStencilView(depthStencilBuffer_.Get(), &dsvDesc, CPUDsvDescriptorHandle());
}

GPUFrameResource::GPUFrameResource(GPUFrameResource&&) = default;

GPUFrameResource& GPUFrameResource::operator=(GPUFrameResource&&) = default;

D3D12_CPU_DESCRIPTOR_HANDLE GPUFrameResource::CPURtvDescriptorHandle() const
{
    auto heapHandle = sharedRtvDescriptorHeap_->GetCPUDescriptorHandleForHeapStart();
    heapHandle.ptr += offsetInRtvHeap_;

    return heapHandle;
}

D3D12_GPU_DESCRIPTOR_HANDLE GPUFrameResource::GPURtvDescriptorHandle() const
{
    auto heapHandle = sharedRtvDescriptorHeap_->GetGPUDescriptorHandleForHeapStart();
    heapHandle.ptr += offsetInRtvHeap_;

    return heapHandle;
}

D3D12_CPU_DESCRIPTOR_HANDLE GPUFrameResource::CPUDsvDescriptorHandle() const
{
    auto heapHandle = sharedDsvDescriptorHeap_->GetCPUDescriptorHandleForHeapStart();
    heapHandle.ptr += offsetInDsvHeap_;

    return heapHandle;
}

D3D12_GPU_DESCRIPTOR_HANDLE GPUFrameResource::GPUDsvDescriptorHandle() const
{
    auto heapHandle = sharedDsvDescriptorHeap_->GetGPUDescriptorHandleForHeapStart();
    heapHandle.ptr += offsetInDsvHeap_;

    return heapHandle;
}

ID3D12Resource* GPUFrameResource::FrameBuffer() const
{
    return resource_.Get();
}
