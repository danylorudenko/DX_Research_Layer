#include <Rendering\GPUFoundation.hpp>
#include <Foundation\GPUCapabilities.hpp>

GPUFoundation::GPUFoundation() = default;

GPUFoundation::GPUFoundation(Application& application)
{    
    // Create device and dxgiFactory.
    InitializeD3D12();

    // Create main accessors to GPU.
    CreateGPUEngines();

    // Create swap chain and supporting resources.
    CreateSwapChain(application, dxgiFactory_.Get());

    CreateDefaultDataStorages();
    CreateFrameResources();

    frameGraph_ = new GPUFrameGraph{};
}

GPUFoundation::GPUFoundation(GPUFoundation&& rhs)
{
    device_ = std::move(rhs.device_);
    dxgiFactory_ = std::move(rhs.dxgiFactory_);

    for (int i = 0; i < 3; i++) {
        engines_[i] = std::move(rhs.engines_[i]);
    }

    swapChain_ = std::move(rhs.swapChain_);
    currentFrame_ = std::move(rhs.currentFrame_);

    frameGraph_ = rhs.frameGraph_; rhs.frameGraph_ = nullptr;

    renderTargetBuffers_ = rhs.renderTargetBuffers_; rhs.renderTargetBuffers_ = nullptr;
    depthStencilBuffers_ = rhs.depthStencilBuffers_; rhs.depthStencilBuffers_ = nullptr;
    finalRenderTargetViews_ = rhs.finalRenderTargetViews_; rhs.finalRenderTargetViews_ = nullptr;
    finalDepthStencilViews_ = rhs.finalDepthStencilViews_; rhs.finalDepthStencilViews_ = nullptr;

    //resourceFactory_ = rhs.resourceFactory_; rhs.resourceFactory_ = nullptr;
    descriptorHeap_ = rhs.descriptorHeap_; rhs.descriptorHeap_ = nullptr;

}

GPUFoundation& GPUFoundation::operator=(GPUFoundation&& rhs)
{
    device_ = std::move(rhs.device_);
    dxgiFactory_ = std::move(rhs.dxgiFactory_);

    for (int i = 0; i < 3; i++) {
        engines_[i] = std::move(rhs.engines_[i]);
    }

    swapChain_ = std::move(rhs.swapChain_);
    currentFrame_ = std::move(rhs.currentFrame_);

    frameGraph_ = rhs.frameGraph_; rhs.frameGraph_ = nullptr;

    renderTargetBuffers_ = rhs.renderTargetBuffers_; rhs.renderTargetBuffers_ = nullptr;
    depthStencilBuffers_ = rhs.depthStencilBuffers_; rhs.depthStencilBuffers_ = nullptr;
    finalRenderTargetViews_ = rhs.finalRenderTargetViews_; rhs.finalRenderTargetViews_ = nullptr;
    finalDepthStencilViews_ = rhs.finalDepthStencilViews_; rhs.finalDepthStencilViews_ = nullptr;

    //resourceFactory_ = rhs.resourceFactory_; rhs.resourceFactory_ = nullptr;
    descriptorHeap_ = rhs.descriptorHeap_; rhs.descriptorHeap_ = nullptr;

    return *this;
}

GPUFoundation::~GPUFoundation()
{
    delete renderTargetBuffers_;
    delete depthStencilBuffers_;

    delete finalRenderTargetViews_;
    delete finalDepthStencilViews_;

    delete frameGraph_;

    //delete resourceFactory_;
    delete descriptorHeap_;
}

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

void GPUFoundation::CreateFrameResources()
{
    assert(SWAP_CHAIN_BUFFER_COUNT <= 3 && "More than 3 framebuffers in not currently supported.");
    
    // Render targets fetch and view creation.
    Microsoft::WRL::ComPtr<ID3D12Resource> renderBuffers[3];
    for (int i = 0; i < SWAP_CHAIN_BUFFER_COUNT; i++) {
        swapChain_->GetBuffer(i, IID_PPV_ARGS(renderBuffers[i].GetAddressOf()));
    }

    int const framesCount = static_cast<int>(SWAP_CHAIN_BUFFER_COUNT);

    renderTargetBuffers_ = new GPUFrameResource{ framesCount, HEIGHT * WIDTH, renderBuffers, D3D12_RESOURCE_STATE_PRESENT };

    D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
    rtvDesc.Format = backBufferFormat_;
    rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
    rtvDesc.Texture2D.MipSlice = 0;
    rtvDesc.Texture2D.PlaneSlice = 0;
    auto rtv = descriptorHeap_->AllocRtvLinear(renderTargetBuffers_, rtvDesc, D3D12_RESOURCE_STATE_PRESENT, "renderBuffer", framesCount);
    finalRenderTargetViews_ = new GPUFrameResourceDescriptor{ rtv };

    // Creation of depth-stencil buffers, creation of views.
    D3D12_RESOURCE_DESC depthStencilDesc{};
    depthStencilDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    depthStencilDesc.Format = depthStencilBufferFormat;
    depthStencilDesc.Width = WIDTH;
    depthStencilDesc.Height = HEIGHT;
    depthStencilDesc.MipLevels = 1;
    depthStencilDesc.DepthOrArraySize = 1;
    depthStencilDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
    depthStencilDesc.SampleDesc.Count = 1;
    depthStencilDesc.SampleDesc.Quality = 0;
    depthStencilDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
    depthStencilDesc.MipLevels = 1;

    depthStencilBuffers_ = new GPUFrameResource{ framesCount, device_.Get(), WIDTH * HEIGHT, &depthStencilDesc, D3D12_RESOURCE_STATE_DEPTH_WRITE };

    D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
    dsvDesc.Format = depthStencilBufferFormat;
    dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
    dsvDesc.Texture2D.MipSlice = 0;
    dsvDesc.Flags = D3D12_DSV_FLAG_NONE;
    auto dsv = descriptorHeap_->AllocDsvLinear(depthStencilBuffers_, dsvDesc, D3D12_RESOURCE_STATE_DEPTH_WRITE, "depthStencil", framesCount);
    finalDepthStencilViews_ = new GPUFrameResourceDescriptor{ dsv };
}

void GPUFoundation::CreateDefaultDataStorages()
{
    descriptorHeap_ = new GPUDescriptorHeap{ device_, RTV_HEAP_CAPACITY, DSV_HEAP_CAPACITY, CBV_SRV_UAV_CAPACITY };

    ////////////////////

    //resourceFactory_ = new GPUResourceFactory{ *this, SWAP_CHAIN_BUFFER_COUNT };
}

void GPUFoundation::CreateSwapChain(Application& application, IDXGIFactory* factory)
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
    sd.SampleDesc.Quality = 0;

    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.BufferCount = SWAP_CHAIN_BUFFER_COUNT;
    sd.OutputWindow = application.window().nativeHandle();
    sd.Windowed = true;
    sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    {
        auto const result = factory->CreateSwapChain(Engine<GPU_ENGINE_TYPE_DIRECT>().CommandQueue(), &sd, &swapChain_);
        ThrowIfFailed(result);
    }
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
