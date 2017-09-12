#include <cassert>

#include <Utility\DirectAppDelegate.hpp>

void DirectAppDelegate::start(Application& application)
{
    InitializeD3D12();
    CreateFence();
    GetDescriptorSizes();
    CheckMXAA4xQuality();
    CreateCommandObjects();
    CreateSwapChain(application);
    CreateDescriptorHeaps();
    CreateRenderTargetView();
    CreateDepthStencilBufferView();
}

void DirectAppDelegate::update(Application& application)
{

}

void DirectAppDelegate::shutdown(Application& application)
{

}


Microsoft::WRL::ComPtr<ID3D12Device> DirectAppDelegate::Device()
{
    return device_;
}

void DirectAppDelegate::InitializeD3D12()
{
    using namespace Microsoft::WRL;
#if defined(DEBUG) || defined(_DEBUG)
    {
        ComPtr<ID3D12Debug> debugController;
        HRESULT result = D3D12GetDebugInterface(IID_PPV_ARGS(&debugController));
        ThrowIfFailed(result);
    }
#endif
    HRESULT result;
    result = CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory1_));
    ThrowIfFailed(result);

    result = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device_));
    ThrowIfFailed(result);
}

void DirectAppDelegate::CreateFence()
{
    HRESULT result = Device()->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence_));
    ThrowIfFailed(result);
}

void DirectAppDelegate::GetDescriptorSizes()
{
    rtvDescriptorSize_ = Device()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    dsvDescriptorSize_ = Device()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
    cvb_srv_uavDescriptorSize_ = Device()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

void DirectAppDelegate::CheckMXAA4xQuality()
{
    D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS qualityLevels;

    qualityLevels.Format = backBufferFormat_;
    qualityLevels.SampleCount = 4;
    qualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
    qualityLevels.NumQualityLevels = 0;

    HRESULT result = Device()->CheckFeatureSupport(D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS, &qualityLevels, sizeof(qualityLevels));
    ThrowIfFailed(result);

    MSAA4xQuality_ = qualityLevels.NumQualityLevels;

    assert(MSAA4xQuality_ > 0 && "Unexpected MSAA quality level.");
}

void DirectAppDelegate::CreateCommandObjects()
{
    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
    queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;

    HRESULT result = Device()->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&commandQueue_));
    ThrowIfFailed(result);

    result = Device()->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator_));
    ThrowIfFailed(result);

    result = Device()->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator_.Get(), nullptr, IID_PPV_ARGS(&commandList_));
    ThrowIfFailed(result);

    commandList_->Close();
}

void DirectAppDelegate::CreateSwapChain(Application& application)
{
    swapChain_.Reset();

    DXGI_SWAP_CHAIN_DESC sd;
    sd.BufferDesc.Format = backBufferFormat_;
    sd.BufferDesc.Width = WIDTH;
    sd.BufferDesc.Height = HEIGHT;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = MSAA4xQuality_ - 1;

    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.BufferCount = SWAP_CHAIN_BUFFER_COUNT;
    sd.OutputWindow = application.window().nativeHandle();
    sd.Windowed = true;
    sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    HRESULT result = dxgiFactory1_->CreateSwapChain(commandQueue_.Get(), &sd, swapChain_.GetAddressOf());
    ThrowIfFailed(result);
}

void DirectAppDelegate::CreateDescriptorHeaps()
{
    D3D12_DESCRIPTOR_HEAP_DESC rtvD;
    rtvD.NumDescriptors = SWAP_CHAIN_BUFFER_COUNT;
    rtvD.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    rtvD.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    rtvD.NodeMask = 0;

    HRESULT result = Device()->CreateDescriptorHeap(&rtvD, IID_PPV_ARGS(&rtvHeap_));
    ThrowIfFailed(result);

    D3D12_DESCRIPTOR_HEAP_DESC dsvD;
    dsvD.NumDescriptors = 1;
    dsvD.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
    dsvD.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    dsvD.NodeMask = 0;

    result = Device()->CreateDescriptorHeap(&dsvD, IID_PPV_ARGS(&dsvHeap_));
    ThrowIfFailed(result);
}

void DirectAppDelegate::CreateRenderTargetView()
{
    using namespace Microsoft::WRL;
    ComPtr<ID3D12Resource> swapChainBuffer[SWAP_CHAIN_BUFFER_COUNT];

    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle(rtvHeap_->GetCPUDescriptorHandleForHeapStart());

    for (UINT i = 0; i < SWAP_CHAIN_BUFFER_COUNT; i++) {
        // Get ith buffer in swap chain.
        HRESULT result = swapChain_->GetBuffer(i, IID_PPV_ARGS(&swapChainBuffer[i]));
        ThrowIfFailed(result);

        Device()->CreateRenderTargetView(swapChainBuffer[i].Get(), nullptr, rtvHeapHandle);
        rtvHeapHandle.Offset(1, rtvDescriptorSize_);
    }
}

void DirectAppDelegate::CreateDepthStencilBufferView()
{
    D3D12_CLEAR_VALUE clearVal;
    clearVal.Format = depthStencilBufferFormat;
    clearVal.DepthStencil.Depth = 1.0f;
    clearVal.DepthStencil.Stencil = 0;

    D3D12_RESOURCE_DESC resDesc;
    resDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    resDesc.Alignment = 0;
    resDesc.Width = WIDTH;
    resDesc.Height = HEIGHT;
    resDesc.DepthOrArraySize = 1;
    resDesc.MipLevels = 1;
    resDesc.Format = depthStencilBufferFormat;

    resDesc.SampleDesc.Count = 1;
    resDesc.SampleDesc.Quality = MSAA4xQuality_ - 1;

    resDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
    resDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

    HRESULT result = Device()->CreateCommittedResource(
        &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
        D3D12_HEAP_FLAG_NONE,
        &resDesc,
        D3D12_RESOURCE_STATE_COMMON,
        &clearVal,
        IID_PPV_ARGS(&depthStencilBuffer_));
    ThrowIfFailed(result);

    //=================

    D3D12_DEPTH_STENCIL_VIEW_DESC depthStencDesc;
    depthStencDesc.Flags = D3D12_DSV_FLAG_NONE;
    depthStencDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
    depthStencDesc.Format = depthStencilBufferFormat;
    depthStencDesc.Texture2D.MipSlice = 0;

    Device()->CreateDepthStencilView(depthStencilBuffer_.Get(), &depthStencDesc, dsvHeap_->GetCPUDescriptorHandleForHeapStart());

    commandList_->ResourceBarrier(
        1,
        &CD3DX12_RESOURCE_BARRIER::Transition(
            depthStencilBuffer_.Get(),
            D3D12_RESOURCE_STATE_COMMON,
            D3D12_RESOURCE_STATE_DEPTH_WRITE));

    commandList_->Close();
    ID3D12CommandList* cmdsLists[] = { commandList_.Get() };
    commandQueue_->ExecuteCommandLists(1, cmdsLists);

    aweyxahf
}