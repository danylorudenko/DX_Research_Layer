#include <Rendering\GPUFoundation.hpp>
#include <Foundation\GPUCapabilities.hpp>

GPUFoundation::GPUFoundation() = default;

GPUFoundation::GPUFoundation(Application& application)
{    
    // Create device and dxgiFactory.
    InitializeD3D12();

    staticResourceAllocator_ = GPUStaticResourceAllocator{ *this };
    viewAllocator_ = GPUResourceViewAllocator{ *this };
    viewContextTable_ = GPUResourceViewContextTable{ swapChain_.BufferCount() };

    swapChain_ = GPUSwapChain{ *this, application.window().nativeHandle(), backBufferFormat_, WIDTH, HEIGHT, SWAP_CHAIN_BUFFER_COUNT };

    D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
    rtvDesc.Format = backBufferFormat_;
    rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
    rtvDesc.Texture2D.MipSlice = 0;
    rtvDesc.Texture2D.PlaneSlice = 0;
    swapChainRTV_ = AllocSwapChainRTV(swapChain_, rtvDesc);

    // Create main accessors to GPU.
    CreateGPUEngines();
}

GPUFoundation::GPUFoundation(GPUFoundation&& rhs) = default;

GPUFoundation& GPUFoundation::operator=(GPUFoundation&& rhs) = default;

void GPUFoundation::InitializeD3D12()
{
#if defined(DEBUG) || defined(_DEBUG)
    {
        auto const result = D3D12GetDebugInterface(IID_PPV_ARGS(&debugController_));
        ThrowIfFailed(result);
        debugController_->EnableDebugLayer();
        
        {
            Microsoft::WRL::ComPtr<ID3D12Debug1> debugController1{ nullptr };
            auto const result = debugController_->QueryInterface(IID_PPV_ARGS(debugController1.GetAddressOf()));
            ThrowIfFailed(result);
        }
    }
#endif

    HRESULT res = CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory_));
    assert(SUCCEEDED(res) && "Failed to create dxgiFactory\n");

    auto adaptersInfo = GPUCapabilities::ListAdapters(*this);
    OutputDebugStringW(adaptersInfo.c_str());

    device_ = GPUCapabilities::GenerateStandardDeviceQuery(*this);
}

void GPUFoundation::CreateGPUEngines()
{
    engines_[0] = GPUEngine{ device_.Get(), GPU_ENGINE_TYPE_DIRECT, SWAP_CHAIN_BUFFER_COUNT };
    engines_[1] = GPUEngine{ device_.Get(), GPU_ENGINE_TYPE_COPY, SWAP_CHAIN_BUFFER_COUNT };
    engines_[2] = GPUEngine{ device_.Get(), GPU_ENGINE_TYPE_COMPUTE, SWAP_CHAIN_BUFFER_COUNT };
}

void GPUFoundation::ResetAll()
{
    Engine<GPU_ENGINE_TYPE_DIRECT>().Reset();
    Engine<GPU_ENGINE_TYPE_COPY>().Reset();
    Engine<GPU_ENGINE_TYPE_COMPUTE>().Reset();
}

void GPUFoundation::CreateRootSignature(Microsoft::WRL::ComPtr<ID3DBlob> serializedRootSignature, Microsoft::WRL::ComPtr<ID3D12RootSignature>& dest)
{
    auto const result = device_->CreateRootSignature(0, serializedRootSignature->GetBufferPointer(), serializedRootSignature->GetBufferSize(), IID_PPV_ARGS(&dest));
    ThrowIfFailed(result);
}

void GPUFoundation::CompileShader(LPCWSTR fileName, Microsoft::WRL::ComPtr<ID3DBlob>& dest, LPCSTR entryPoint, LPCSTR type)
{
#if defined (_DEBUG) | (DEBUG)
    UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
    UINT compileFlags = 0;
#endif
    {
        auto const result = D3DCompileFromFile(fileName, nullptr, nullptr, entryPoint, type, compileFlags, 0, &dest, nullptr);
        ThrowIfFailed(result);
    }
}

void GPUFoundation::CreatePSO(Microsoft::WRL::ComPtr<ID3D12PipelineState>& dest, D3D12_GRAPHICS_PIPELINE_STATE_DESC* desc)
{
    auto const result = device_->CreateGraphicsPipelineState(desc, IID_PPV_ARGS(&dest));
    ThrowIfFailed(result);
}

GPUResourceHandle GPUFoundation::AllocDefaultResource(D3D12_RESOURCE_DESC const& desc, D3D12_RESOURCE_STATES initialState)
{
    return staticResourceAllocator_.AllocDefault(desc, initialState);
}

GPUResourceHandle GPUFoundation::AllocUploadResource(D3D12_RESOURCE_DESC const& desc, D3D12_RESOURCE_STATES initialState)
{
    return staticResourceAllocator_.AllocUpload(desc, initialState);
}

GPUResourceViewHandle GPUFoundation::SwapChainRTV()
{
    return swapChainRTV_;
}

GPUResourceViewHandle GPUFoundation::AllocSwapChainRTV(GPUSwapChain& swapChain, D3D12_RENDER_TARGET_VIEW_DESC const& desc)
{
    auto const framesCount = swapChain.BufferCount();
    std::vector<GPUResourceViewDirectHandle> directHandles{ framesCount };
    for (size_t i = 0; i < framesCount; i++) {
        directHandles[i] = viewAllocator_.AllocSwapChainRTV(swapChain_.AccessRenderBuffer(i), desc);
    }
    return viewContextTable_.InsertView(framesCount, directHandles.data(), viewAllocator_);
}

GPUResourceViewHandle GPUFoundation::AllocRTV(std::size_t frames, GPUResourceHandle const* resources, D3D12_RENDER_TARGET_VIEW_DESC const& desc)
{
    assert(frames == 1 || frames == swapChain_.BufferCount() && "Can't create views with invalid frameCount");
    std::vector<GPUResourceViewDirectHandle> directHandles{ frames };
    for (size_t i = 0; i < frames; i++) {
        directHandles[i] = viewAllocator_.AllocRTV(resources[i], desc);
    }
    return viewContextTable_.InsertView(frames, directHandles.data(), viewAllocator_);
}

GPUResourceViewHandle GPUFoundation::AllocDSV(std::size_t frames, GPUResourceHandle const* resource, D3D12_DEPTH_STENCIL_VIEW_DESC const& desc, D3D12_RESOURCE_STATES targetState)
{
    assert(frames == 1 || frames == swapChain_.BufferCount() && "Can't create views with invalid frameCount");
    std::vector<GPUResourceViewDirectHandle> directHandles{ frames };
    for (size_t i = 0; i < frames; i++) {
        directHandles[i] = viewAllocator_.AllocDSV(resource[i], desc, targetState);
    }
    return viewContextTable_.InsertView(frames, directHandles.data(), viewAllocator_);
}

GPUResourceViewHandle GPUFoundation::AllocCBV(std::size_t frames, GPUResourceHandle const* resource, D3D12_CONSTANT_BUFFER_VIEW_DESC const& desc, D3D12_RESOURCE_STATES targetState)
{
    assert(frames == 1 || frames == swapChain_.BufferCount() && "Can't create views with invalid frameCount");
    std::vector<GPUResourceViewDirectHandle> directHandles{ frames };
    for (size_t i = 0; i < frames; i++) {
        directHandles[i] = viewAllocator_.AllocCBV(resource[i], desc, targetState);
    }
    return viewContextTable_.InsertView(frames, directHandles.data(), viewAllocator_);
}

GPUResourceViewHandle GPUFoundation::AllocSRV(std::size_t frames, GPUResourceHandle const* resource, D3D12_SHADER_RESOURCE_VIEW_DESC const& desc, D3D12_RESOURCE_STATES targetState)
{
    assert(frames == 1 || frames == swapChain_.BufferCount() && "Can't create views with invalid frameCount");
    std::vector<GPUResourceViewDirectHandle> directHandles{ frames };
    for (size_t i = 0; i < frames; i++) {
        directHandles[i] = viewAllocator_.AllocSRV(resource[i], desc, targetState);
    }
    return viewContextTable_.InsertView(frames, directHandles.data(), viewAllocator_);
}

GPUResourceViewHandle GPUFoundation::AllocUAV(std::size_t frames, GPUResourceHandle const* resource, D3D12_UNORDERED_ACCESS_VIEW_DESC const& desc, D3D12_RESOURCE_STATES targetState)
{
    assert(frames == 1 || frames == swapChain_.BufferCount() && "Can't create views with invalid frameCount");
    std::vector<GPUResourceViewDirectHandle> directHandles{ frames };
    for (size_t i = 0; i < frames; i++) {
        directHandles[i] = viewAllocator_.AllocUAV(resource[i], desc, targetState);
    }
    return viewContextTable_.InsertView(frames, directHandles.data(), viewAllocator_);
}

GPUResourceViewTable GPUFoundation::BuildViewTable(std::size_t tableSize, GPUShaderVisibleResourceViewDesc const* descriptions)
{
    std::vector<GPUResourceViewHandle> handles{ tableSize };
    for (size_t i = 0; i < tableSize; i++) {
        GPUShaderVisibleResourceViewDesc const& currentDesc = descriptions[i];
        switch (currentDesc.type_) {
        case GPU_SHADER_VISIBLE_RESOURCE_VIEW_TYPE_CBV:
            handles[i] = AllocCBV(currentDesc.frames_, currentDesc.resources_, currentDesc.cbvDesc_, currentDesc.targetState_);
            break;
        case GPU_SHADER_VISIBLE_RESOURCE_VIEW_TYPE_SRV:
            handles[i] = AllocSRV(currentDesc.frames_, currentDesc.resources_, currentDesc.srvDesc_, currentDesc.targetState_);
            break;
        case GPU_SHADER_VISIBLE_RESOURCE_VIEW_TYPE_UAV:
            handles[i] = AllocUAV(currentDesc.frames_, currentDesc.resources_, currentDesc.uavDesc_, currentDesc.targetState_);
            break;
        }
    }

    return GPUResourceViewTable(tableSize, handles.data());
}

//
//void GPUFoundation::CreateFrameResources()
//{
//    assert(SWAP_CHAIN_BUFFER_COUNT <= 3 && "More than 3 framebuffers in not currently supported.");
//    
//    // Render targets fetch and view creation.
//    Microsoft::WRL::ComPtr<ID3D12Resource> renderBuffers[3];
//    for (int i = 0; i < SWAP_CHAIN_BUFFER_COUNT; i++) {
//        swapChain_->GetBuffer(i, IID_PPV_ARGS(renderBuffers[i].GetAddressOf()));
//    }
//
//    int const framesCount = static_cast<int>(SWAP_CHAIN_BUFFER_COUNT);
//
//    renderTargetBuffers_ = new GPUFrameResource{ framesCount, HEIGHT * WIDTH, renderBuffers, D3D12_RESOURCE_STATE_PRESENT };
//
//    D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
//    rtvDesc.Format = backBufferFormat_;
//    rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
//    rtvDesc.Texture2D.MipSlice = 0;
//    rtvDesc.Texture2D.PlaneSlice = 0;
//    auto rtv = descriptorHeap_->AllocRtvLinear(renderTargetBuffers_, rtvDesc, D3D12_RESOURCE_STATE_PRESENT, "renderBuffer", framesCount);
//    finalRenderTargetViews_ = new GPUFrameResourceDescriptor{ rtv };
//
//    // Creation of depth-stencil buffers, creation of views.
//    D3D12_RESOURCE_DESC depthStencilDesc{};
//    depthStencilDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
//    depthStencilDesc.Format = depthStencilBufferFormat;
//    depthStencilDesc.Width = WIDTH;
//    depthStencilDesc.Height = HEIGHT;
//    depthStencilDesc.MipLevels = 1;
//    depthStencilDesc.DepthOrArraySize = 1;
//    depthStencilDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
//    depthStencilDesc.SampleDesc.Count = 1;
//    depthStencilDesc.SampleDesc.Quality = 0;
//    depthStencilDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
//    depthStencilDesc.MipLevels = 1;
//
//    depthStencilBuffers_ = new GPUFrameResource{ framesCount, device_.Get(), WIDTH * HEIGHT, &depthStencilDesc, D3D12_RESOURCE_STATE_DEPTH_WRITE };
//
//    D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
//    dsvDesc.Format = depthStencilBufferFormat;
//    dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
//    dsvDesc.Texture2D.MipSlice = 0;
//    dsvDesc.Flags = D3D12_DSV_FLAG_NONE;
//    auto dsv = descriptorHeap_->AllocDsvLinear(depthStencilBuffers_, dsvDesc, D3D12_RESOURCE_STATE_DEPTH_WRITE, "depthStencil", framesCount);
//    finalDepthStencilViews_ = new GPUFrameResourceDescriptor{ dsv };
//}
//
//void GPUFoundation::CreateDefaultDataStorages()
//{
//    descriptorHeap_ = new GPUDescriptorHeap{ device_, RTV_HEAP_CAPACITY, DSV_HEAP_CAPACITY, CBV_SRV_UAV_CAPACITY };
//
//    ////////////////////
//
//    //resourceFactory_ = new GPUResourceFactory{ *this, SWAP_CHAIN_BUFFER_COUNT };
//}
//
//void GPUFoundation::CreateSwapChain(Application& application, IDXGIFactory* factory)
//{
//    swapChain_.Reset();
//
//    DXGI_SWAP_CHAIN_DESC sd;
//    sd.BufferDesc.Format = backBufferFormat_;
//    sd.BufferDesc.Width = WIDTH;
//    sd.BufferDesc.Height = HEIGHT;
//    sd.BufferDesc.RefreshRate.Numerator = 60;
//    sd.BufferDesc.RefreshRate.Denominator = 1;
//    sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
//    sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
//
//    sd.SampleDesc.Count = 1;
//    sd.SampleDesc.Quality = 0;
//
//    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
//    sd.BufferCount = SWAP_CHAIN_BUFFER_COUNT;
//    sd.OutputWindow = application.window().nativeHandle();
//    sd.Windowed = true;
//    sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
//    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
//
//    {
//        auto const result = factory->CreateSwapChain(Engine<GPU_ENGINE_TYPE_DIRECT>().CommandQueue(), &sd, &swapChain_);
//        ThrowIfFailed(result);
//    }
//}
