#include <Rendering\GPUAccess.hpp>

GPUAccess::GPUAccess(Application& application)
{
    InitializeD3D12();
    CreateSwapChain(application, dxgiFactory_.Get());
    CreateDepthStencilBuffer();
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
        HRESULT result = D3D12GetDebugInterface(IID_PPV_ARGS(&debugController));
        ThrowIfFailed(result);

        debugController->EnableDebugLayer();
    }
#endif
    HRESULT result;
    result = CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory_));
    ThrowIfFailed(result);

    result = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device_));
    ThrowIfFailed(result);
}

void GPUAccess::CreateFrameResources()
{
    UINT rtvDescriptorSize = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    
    for (int i = 0; i < SWAP_CHAIN_BUFFER_COUNT; i++) {
        Microsoft::WRL::ComPtr<ID3D12Resource> frameBuffer = nullptr;
        swapChain_->GetBuffer(i, IID_PPV_ARGS(&frameBuffer));
        frameResources[i] = FrameResource(*device_.Get(), rtvHeap_, i * rtvDescriptorSize, frameBuffer);
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
}

void GPUAccess::CreateDescriptorHeaps()
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
}

void GPUAccess::CreateDepthStencilBufferView(ID3D12GraphicsCommandList* startupCommandList)
{
    D3D12_DEPTH_STENCIL_VIEW_DESC depthStencDesc;
    depthStencDesc.Flags = D3D12_DSV_FLAG_NONE;
    depthStencDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
    depthStencDesc.Format = depthStencilBufferFormat;
    depthStencDesc.Texture2D.MipSlice = 0;

    device_->CreateDepthStencilView(depthStencilBuffer_.Get(), &depthStencDesc, dsvHeap_->GetCPUDescriptorHandleForHeapStart());

    startupCommandList->ResourceBarrier(
        1,
        &CD3DX12_RESOURCE_BARRIER::Transition(
            depthStencilBuffer_.Get(),
            D3D12_RESOURCE_STATE_COMMON,
            D3D12_RESOURCE_STATE_DEPTH_WRITE));
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

    HRESULT result = factory->CreateSwapChain(RenderingQueue(), &sd, swapChain_.GetAddressOf());
    ThrowIfFailed(result);
}

void GPUAccess::Begin()
{
    Worker<GPU_WORKER_TYPE_DIRECT>().Reset();
    Worker<GPU_WORKER_TYPE_COPY>().Reset();
    Worker<GPU_WORKER_TYPE_COMPUTE>().Reset();
}

void GPUAccess::End()
{
    Worker<GPU_WORKER_TYPE_DIRECT>().Finalize();
    Worker<GPU_WORKER_TYPE_COPY>().Finalize();
    Worker<GPU_WORKER_TYPE_COMPUTE>().Finalize();

    swapChain_->Present(0, 0);
    currentFrame_ = (currentFrame_ + 1) % SWAP_CHAIN_BUFFER_COUNT;
}

void GPUAccess::CreateGPUBuffer(GPUResource** dest, std::size_t size)
{
    *dest = new GPUResource{ device_.Get(), static_cast<UINT64>(size) };
}

void GPUAccess::CreateGPUUploadHeap(GPUUploadHeap** dest, void const* data, std::size_t elementSize, std::size_t elementsCount, bool isConstBuffer)
{
    *dest = new GPUUploadHeap{ device_.Get(), data, elementSize, elementsCount, isConstBuffer };
}

void GPUAccess::ScheduleRender(UINT vertexCount)
{
    Worker<GPU_WORKER_TYPE_DIRECT>().Commit().DrawInstanced(vertexCount, 1, 0, 0);
}

void GPUAccess::SheduleIndexedRender(UINT indexCount)
{
    Worker<GPU_WORKER_TYPE_DIRECT>().Commit().DrawIndexedInstanced(indexCount, 0, 0, 0, 0);
}

void GPUAccess::CompileShader(LPWSTR fileName, ID3DBlob * dest, LPCSTR entryPoint, LPCSTR type)
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

D3D12_RESOURCE_STATES GPUAccess::TransitionGPUResource(GPUResource& resource, D3D12_RESOURCE_STATES state, bool immediate)
{
    auto prevState = resource.SetTargetState(state);
    Worker<GPU_WORKER_TYPE_COPY>().Commit().ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(resource.Get(), prevState, resource.TargetState()));

    if (immediate) {
        Worker<GPU_WORKER_TYPE_COPY>().Finalize();
        Worker<GPU_WORKER_TYPE_COPY>().Reset();
    }

    return prevState;
}

void GPUAccess::UpdateGPUResource(GPUResource& dest, std::size_t offset, const void* data, std::size_t size)
{
    assert(size + offset <= dest.Size());

    const D3D12_RANGE mapRange = { offset, offset + size };
    void* bufferPtr = nullptr;
    dest.Map(&bufferPtr, mapRange);

    memcpy(bufferPtr, data, size);
    dest.Unmap(mapRange);
}

void GPUAccess::SetVertexBuffer(D3D12_VERTEX_BUFFER_VIEW const* bufferView)
{
    Worker<GPU_WORKER_TYPE_DIRECT>().Commit().IASetVertexBuffers(0, 1, bufferView);
}

void GPUAccess::SetIndexBuffer(D3D12_INDEX_BUFFER_VIEW const* bufferView)
{
    Worker<GPU_WORKER_TYPE_DIRECT>().Commit().IASetIndexBuffer(bufferView);
}

