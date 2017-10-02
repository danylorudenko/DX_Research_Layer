#include <Rendering\GPUAccess.hpp>

GPUAccess::GPUAccess(Application& application)
{
    // Create device and dxgiFactory.
    InitializeD3D12();

    // Create main accessors to GPU.
    CreateGPUWorkers();

    // Create swap chain and supporting resources.
    CreateSwapChain(application, dxgiFactory_.Get());

    CreateDefaultDescriptorHeaps();
    CreateFrameResources();
    CreateDepthStencilBuffer();
    CreateDepthStencilBufferView();

    // Set default viewport and scissor values.
    SetViewportScissor();
}

GPUAccess::GPUAccess(GPUAccess&& rhs)
{
    
}

GPUAccess& GPUAccess::operator=(GPUAccess&& rhs)
{
    return *this;
}

void GPUAccess::InitializeD3D12()
{
    using namespace Microsoft::WRL;
#if defined(DEBUG) || defined(_DEBUG)
    {
        ComPtr<ID3D12Debug> debugController;
        ThrowIfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))
        debugController->EnableDebugLayer();
    }
#endif

    ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory_)));
    ThrowIfFailed(D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device_)));
}

void GPUAccess::CreateFrameResources()
{
    UINT rtvDescriptorSize = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    
    for (int i = 0; i < SWAP_CHAIN_BUFFER_COUNT; i++) {
        Microsoft::WRL::ComPtr<ID3D12Resource> frameBuffer = nullptr;
        swapChain_->GetBuffer(i, IID_PPV_ARGS(&frameBuffer));
        frameResources_[i] = FrameResource(*device_.Get(), rtvHeap_, i * rtvDescriptorSize, frameBuffer);
    }
}

void GPUAccess::CreateDepthStencilBuffer()
{
    D3D12_RESOURCE_DESC resDesc;
    resDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    resDesc.Alignment = 0;
    resDesc.Width = WIDTH;
    resDesc.Height = HEIGHT;
    resDesc.DepthOrArraySize = 1;
    resDesc.MipLevels = 1;
    resDesc.Format = depthStencilBufferFormat;

    resDesc.SampleDesc.Count = 1;
    resDesc.SampleDesc.Quality = 0;

    resDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
    resDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

    //=================

    D3D12_CLEAR_VALUE clearVal;
    clearVal.Format = depthStencilBufferFormat;
    clearVal.DepthStencil.Depth = 1.0f;
    clearVal.DepthStencil.Stencil = 0;

    HRESULT result = device_->CreateCommittedResource(
        &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
        D3D12_HEAP_FLAG_NONE,
        &resDesc,
        D3D12_RESOURCE_STATE_COMMON,
        &clearVal,
        IID_PPV_ARGS(&depthStencilBuffer_));
    ThrowIfFailed(result);

    Worker<GPU_WORKER_TYPE_DIRECT>().Reset();
    Worker<GPU_WORKER_TYPE_DIRECT>().Commit().ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(depthStencilBuffer_.Get(), D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_DEPTH_WRITE));
    Worker<GPU_WORKER_TYPE_DIRECT>().Finalize();
}

void GPUAccess::CreateDefaultDescriptorHeaps()
{
    D3D12_DESCRIPTOR_HEAP_DESC rtvD;
    rtvD.NumDescriptors = SWAP_CHAIN_BUFFER_COUNT;
    rtvD.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    rtvD.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    rtvD.NodeMask = 0;

    HRESULT result = device_->CreateDescriptorHeap(&rtvD, IID_PPV_ARGS(&rtvHeap_));
    ThrowIfFailed(result);

    D3D12_DESCRIPTOR_HEAP_DESC dsvD;
    dsvD.NumDescriptors = 1;
    dsvD.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
    dsvD.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    dsvD.NodeMask = 0;

    result = device_->CreateDescriptorHeap(&dsvD, IID_PPV_ARGS(&dsvHeap_));
    ThrowIfFailed(result);

    rtvDescriptorSize_ = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    dsvDescriptorSize_ = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
    cbv_srv_uavDescriptorSize_ = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

void GPUAccess::CreateDepthStencilBufferView()
{
    D3D12_DEPTH_STENCIL_VIEW_DESC depthStencDesc;
    depthStencDesc.Flags = D3D12_DSV_FLAG_NONE;
    depthStencDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
    depthStencDesc.Format = depthStencilBufferFormat;
    depthStencDesc.Texture2D.MipSlice = 0;

    device_->CreateDepthStencilView(depthStencilBuffer_.Get(), &depthStencDesc, dsvHeap_->GetCPUDescriptorHandleForHeapStart());
}

void GPUAccess::SetViewportScissor()
{
    viewportRect_.TopLeftX = 0.0f;
    viewportRect_.TopLeftY = 0.0f;
    viewportRect_.MinDepth = 0.0f;
    viewportRect_.MaxDepth = 1.0f;
    viewportRect_.Width = static_cast<float>(WIDTH);
    viewportRect_.Height = static_cast<float>(HEIGHT);

    scissorRect_ = CD3DX12_RECT{ 0, 0, static_cast<LONG>(WIDTH), static_cast<LONG>(HEIGHT) };
}

void GPUAccess::CreateSwapChain(Application& application, IDXGIFactory* factory)
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

    ThrowIfFailed(factory->CreateSwapChain(Worker<GPU_WORKER_TYPE_DIRECT>().CommandQueue(), &sd, swapChain_.GetAddressOf()));
}

void GPUAccess::CreateGPUWorkers()
{
    // Workers are being reset automatically on creation.

    workers_[0] = new GPUEngine(device_.Get(), GPU_WORKER_TYPE_DIRECT);
    //workers_[0]->Reset();

    workers_[1] = new GPUEngine(device_.Get(), GPU_WORKER_TYPE_COPY);
    //workers_[1]->Reset();

    workers_[2] = new GPUEngine(device_.Get(), GPU_WORKER_TYPE_COMPUTE);
    //workers_[2]->Reset();
}

void GPUAccess::ResetAll()
{
    Worker<GPU_WORKER_TYPE_DIRECT>().Reset();
    Worker<GPU_WORKER_TYPE_COPY>().Reset();
    Worker<GPU_WORKER_TYPE_COMPUTE>().Reset();
}

ID3D12Resource* GPUAccess::DepthStencilBuffer() const
{
    return depthStencilBuffer_.Get();
}

ID3D12Resource* GPUAccess::CurrentFramebuffer() const
{
    return frameResources_[currentFrame_].FrameBuffer();
}

D3D12_GPU_VIRTUAL_ADDRESS GPUAccess::CurrentFramebufferGPUVirtualAddress() const
{
    return frameResources_[currentFrame_].FrameBuffer()->GetGPUVirtualAddress();
}

D3D12_GPU_VIRTUAL_ADDRESS GPUAccess::DepthStencilBufferGPUVirtualAddress() const
{
    return depthStencilBuffer_->GetGPUVirtualAddress();
}

D3D12_CPU_DESCRIPTOR_HANDLE GPUAccess::CurrentRtvHandle() const
{
    return frameResources_[currentFrame_].CPUDescriptorHandle();
}

D3D12_CPU_DESCRIPTOR_HANDLE GPUAccess::DepthStencilHandle() const
{
    return dsvHeap_->GetCPUDescriptorHandleForHeapStart();
}

void GPUAccess::FinalizeAll()
{
    Worker<GPU_WORKER_TYPE_DIRECT>().Finalize();
    Worker<GPU_WORKER_TYPE_COPY>().Finalize();
    Worker<GPU_WORKER_TYPE_COMPUTE>().Finalize();
}

void GPUAccess::CommitDefaultViewportScissorRects()
{
    Worker<GPU_WORKER_TYPE_DIRECT>().Commit().RSSetViewports(1, &viewportRect_);
    Worker<GPU_WORKER_TYPE_DIRECT>().Commit().RSSetScissorRects(1, &scissorRect_);
}

void GPUAccess::Present()
{
    swapChain_->Present(0, 0);
    currentFrame_ = (currentFrame_ + 1) % SWAP_CHAIN_BUFFER_COUNT;
}

void GPUAccess::CreateGPUBuffer(GPUResource** dest, std::size_t size)
{
    *dest = new GPUResource{ device_.Get(), static_cast<UINT64>(size) };
}

void GPUAccess::CreateRootSignature(Microsoft::WRL::ComPtr<ID3DBlob> serializedRootSignature, Microsoft::WRL::ComPtr<ID3D12RootSignature>& dest)
{
    ThrowIfFailed(device_->CreateRootSignature(0, serializedRootSignature->GetBufferPointer(), serializedRootSignature->GetBufferSize(), IID_PPV_ARGS(&dest)));
}

void GPUAccess::CreateConstantBufferView(D3D12_CONSTANT_BUFFER_VIEW_DESC* desc, D3D12_CPU_DESCRIPTOR_HANDLE heapHandle)
{
    device_->CreateConstantBufferView(desc, heapHandle);
}

void GPUAccess::CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_DESC* desc, Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& dest)
{
    ThrowIfFailed(device_->CreateDescriptorHeap(desc, IID_PPV_ARGS(&dest)));
}

void GPUAccess::CreateGPUUploadHeap(GPUUploadHeap** dest, void const* data, std::size_t elementSize, bool isConstBuffer)
{
    *dest = new GPUUploadHeap{ device_.Get(), data, elementSize, isConstBuffer };
}

void GPUAccess::CompileShader(LPCWSTR fileName, Microsoft::WRL::ComPtr<ID3DBlob>& dest, LPCSTR entryPoint, LPCSTR type)
{
#if defined (_DEBUG) | (DEBUG)
    UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
    UINT compileFlags = 0;
#endif
    ThrowIfFailed(D3DCompileFromFile(fileName, nullptr, nullptr, entryPoint, type, compileFlags, 0, &dest, nullptr));
}

void GPUAccess::CreatePSO(Microsoft::WRL::ComPtr<ID3D12PipelineState>& dest, D3D12_GRAPHICS_PIPELINE_STATE_DESC* desc)
{
    ThrowIfFailed(device_->CreateGraphicsPipelineState(desc, IID_PPV_ARGS(&dest)));
}

void GPUAccess::UpdateGPUResource(GPUResource& dest, std::size_t offset, const void* data, std::size_t size)
{
    assert(size + offset <= dest.Size());

    GPUUploadHeap uploadHeap{ device_.Get(), data, size };

    D3D12_RESOURCE_STATES prevState = dest.State();
    dest.Transition(&Worker<GPU_WORKER_TYPE_COPY>().Commit(), D3D12_RESOURCE_STATE_COPY_DEST);

    Worker<GPU_WORKER_TYPE_COPY>().Commit().CopyBufferRegion(
        dest.Get(),
        offset,
        uploadHeap.Get(),
        0,
        size);
    
    dest.Transition(&Worker<GPU_WORKER_TYPE_COPY>().Commit(), prevState);
}
