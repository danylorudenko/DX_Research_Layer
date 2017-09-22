#include <pch.hpp>
#include <thread>
#include <chrono>

#include <Utility\DirectAppDelegate.hpp>

void DirectAppDelegate::start(Application& application)
{
    InitializeD3D12();
    CreateMainCommandQueue();

    Microsoft::WRL::ComPtr<ID3D12CommandAllocator> startupCommandAllocator;
    Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> startupCommandList;
    Microsoft::WRL::ComPtr<ID3D12Fence> startupFence;

    HRESULT result = {};
    result = Device().CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&startupCommandAllocator)); ThrowIfFailed(result);
    result = Device().CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, startupCommandAllocator.Get(), nullptr, IID_PPV_ARGS(&startupCommandList)); ThrowIfFailed(result);
    result = Device().CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&startupFence));

    GetDescriptorSizes();
    CreateDescriptorHeaps();
    CreateRootSignature();
    CreatePipelineState();
    CreateSwapChain(application);
    CreateFrameResources();
    CreateDepthStencilBuffer();
    CreateDepthStencilBufferView(startupCommandList.Get());
    SetViewportScissor();

    LoadTriangleVertices();
    LoadConstantBuffers();

    gameTimer_.Reset();

    startupCommandList->Close();
    ID3D12CommandList* list[] = { startupCommandList.Get() };

    commandQueue_->ExecuteCommandLists(1, list);
    commandQueue_->Signal(startupFence.Get(), 1);

    // Wait for initialization queue to be processed:
    HANDLE initEvent = CreateEvent(nullptr, false, false, nullptr);
    if (startupFence->GetCompletedValue() < 1) {
        startupFence->SetEventOnCompletion(1, initEvent);
        WaitForSingleObject(initEvent, INFINITE);
    }

    CreateMainCommandList();
}

void DirectAppDelegate::update(Application& application)
{
    //CustomAction();
    Draw();

    gameTimer_.Tick();
    DisplayFrameTime(application, Timer().DeltaTime());
}

void DirectAppDelegate::shutdown(Application& application)
{

}

PerformanceTimer& DirectAppDelegate::Timer()
{
    return gameTimer_;
}

void DirectAppDelegate::DisplayFrameTime(Application& application, float drawTime)
{
    std::wstring windowText = L"SPF: ";
    windowText += std::to_wstring(drawTime);

    windowText += L";    FPS: ";
    windowText += std::to_wstring(1 / drawTime);

    HWND windowHandle = application.window().nativeHandle();
    SetWindowText(windowHandle, windowText.c_str());
}

ID3D12Device& DirectAppDelegate::Device()
{
    return *device_.Get();
}

D3D12_CPU_DESCRIPTOR_HANDLE DirectAppDelegate::RenderTargetViewHandle() const
{
    return rtvHeap_->GetCPUDescriptorHandleForHeapStart();
}

D3D12_CPU_DESCRIPTOR_HANDLE DirectAppDelegate::DepthStencilViewHandle() const
{
    return dsvHeap_->GetCPUDescriptorHandleForHeapStart();
}

void DirectAppDelegate::InitializeD3D12()
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
    result = CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory1_));
    ThrowIfFailed(result);

    result = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device_));
    ThrowIfFailed(result);
}

void DirectAppDelegate::GetDescriptorSizes()
{
    rtvDescriptorSize_ = Device().GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    dsvDescriptorSize_ = Device().GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
    cvb_srv_uavDescriptorSize_ = Device().GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

void DirectAppDelegate::CreateMainCommandQueue()
{
    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
    queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;

    HRESULT result = {};
    result = Device().CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&commandQueue_)); ThrowIfFailed(result);
}

void DirectAppDelegate::CreateMainCommandList()
{
    HRESULT result = Device().CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, frameResources[currentRenderBuffer].CommandAllocator(), nullptr , IID_PPV_ARGS(&commandList_)); ThrowIfFailed(result);
    commandList_->Close();
}

void DirectAppDelegate::CreateFrameResources()
{
    for (int i = 0; i < SWAP_CHAIN_BUFFER_COUNT; i++) {
        Microsoft::WRL::ComPtr<ID3D12Resource> frameBuffer = nullptr;
        swapChain_->GetBuffer(i, IID_PPV_ARGS(&frameBuffer));
        frameResources[i] = FrameResource(Device(), rtvHeap_, i * rtvDescriptorSize_, frameBuffer);
    }
}

void DirectAppDelegate::CreateDepthStencilBuffer()
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

    HRESULT result = Device().CreateCommittedResource(
        &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
        D3D12_HEAP_FLAG_NONE,
        &resDesc,
        D3D12_RESOURCE_STATE_COMMON,
        &clearVal,
        IID_PPV_ARGS(&depthStencilBuffer_));
    ThrowIfFailed(result);
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
    sd.SampleDesc.Quality = 0;

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

    HRESULT result = Device().CreateDescriptorHeap(&rtvD, IID_PPV_ARGS(&rtvHeap_));
    ThrowIfFailed(result);

    D3D12_DESCRIPTOR_HEAP_DESC dsvD;
    dsvD.NumDescriptors = 1;
    dsvD.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
    dsvD.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    dsvD.NodeMask = 0;

    result = Device().CreateDescriptorHeap(&dsvD, IID_PPV_ARGS(&dsvHeap_));
    ThrowIfFailed(result);


    D3D12_DESCRIPTOR_HEAP_DESC cbvDescr = {};
    cbvDescr.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    cbvDescr.NumDescriptors = 1;
    cbvDescr.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    cbvDescr.NodeMask = 0;

    result = Device().CreateDescriptorHeap(&cbvDescr, IID_PPV_ARGS(&cbvHeap_));
    ThrowIfFailed(result);

}

void DirectAppDelegate::CreateDepthStencilBufferView(ID3D12GraphicsCommandList* startupCommandList)
{
    D3D12_DEPTH_STENCIL_VIEW_DESC depthStencDesc;
    depthStencDesc.Flags = D3D12_DSV_FLAG_NONE;
    depthStencDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
    depthStencDesc.Format = depthStencilBufferFormat;
    depthStencDesc.Texture2D.MipSlice = 0;
    
    Device().CreateDepthStencilView(depthStencilBuffer_.Get(), &depthStencDesc, DepthStencilViewHandle());

    startupCommandList->ResourceBarrier(
        1,
        &CD3DX12_RESOURCE_BARRIER::Transition(
            depthStencilBuffer_.Get(),
            D3D12_RESOURCE_STATE_COMMON,
            D3D12_RESOURCE_STATE_DEPTH_WRITE));
}

void DirectAppDelegate::SetViewportScissor()
{
    viewportRect_.TopLeftX = 0.0f;
    viewportRect_.TopLeftY = 0.0f;
    viewportRect_.MinDepth = 0.0f;
    viewportRect_.MaxDepth = 1.0f;
    viewportRect_.Width = static_cast<float>(WIDTH);
    viewportRect_.Height = static_cast<float>(HEIGHT);

    scissorRect_ = CD3DX12_RECT{ 0, 0, static_cast<LONG>(WIDTH), static_cast<LONG>(HEIGHT) };
}

void DirectAppDelegate::CreateRootSignature()
{
    using namespace Microsoft::WRL;

    CD3DX12_DESCRIPTOR_RANGE ranges[1];
    CD3DX12_ROOT_PARAMETER rootParameters[1];

    ranges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);
    rootParameters[0].InitAsDescriptorTable(1, ranges);

    D3D12_ROOT_SIGNATURE_FLAGS rootSignatureFlags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

    CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
    rootSignatureDesc.Init(1, rootParameters, 0, nullptr, rootSignatureFlags);

    ComPtr<ID3DBlob> signature;
    ComPtr<ID3DBlob> errors;

    HRESULT result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &signature, &errors);
    ThrowIfFailed(result);

    result = Device().CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&rootSignature_));
    ThrowIfFailed(result);
}

void DirectAppDelegate::CreatePipelineState()
{
    using namespace Microsoft::WRL;

    ComPtr<ID3DBlob> vertexShader;
    ComPtr<ID3DBlob> pixelShader;

    // Compile shaders to the Blob.
    {
#if defined (_DEBUG) | (DEBUG)
        UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
        UINT compileFlags = 0;
#endif

        HRESULT result = {};
        result = D3DCompileFromFile(L"Shaders/triangle_shader.hlsl", nullptr, nullptr, "VS", "vs_5_0", compileFlags, 0, &vertexShader, nullptr);
        ThrowIfFailed(result);
        result = D3DCompileFromFile(L"Shaders/triangle_shader.hlsl", nullptr, nullptr, "PS", "ps_5_0", compileFlags, 0, &pixelShader, nullptr);
        ThrowIfFailed(result);
    }

    // Define the vertex input layout.
    D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
    };

    // Setup pipeline state, which inludes setting shaders.
    D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
    psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
    psoDesc.pRootSignature = rootSignature_.Get();
    psoDesc.VS = CD3DX12_SHADER_BYTECODE(vertexShader.Get());
    psoDesc.PS = CD3DX12_SHADER_BYTECODE(pixelShader.Get());
    psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
    psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
    psoDesc.DepthStencilState.DepthEnable = FALSE;
    psoDesc.DepthStencilState.StencilEnable = FALSE;
    psoDesc.SampleMask = UINT_MAX;
    psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    psoDesc.NumRenderTargets = 1;
    psoDesc.RTVFormats[0] = backBufferFormat_;
    psoDesc.SampleDesc.Count = 1;

    HRESULT result = Device().CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&pipelineState_));
    ThrowIfFailed(result);
}

void DirectAppDelegate::LoadTriangleVertices()
{
    using namespace Microsoft::WRL;

    Vertex verticesData[] = {
        { { 0.0f, 0.25f, 0.0f },{ 1.0f, 0.0f, 0.0f, 1.0f } },
        { { 0.25f, -0.25f, 0.0f },{ 0.0f, 1.0f, 0.0f, 1.0f } },
        { { -0.25f, -0.25f, 0.0f },{ 0.0f, 0.0f, 1.0f, 1.0f } },
    };

    constexpr UINT vertexDataSize = sizeof(verticesData);

    Device().CreateCommittedResource(
        &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
        D3D12_HEAP_FLAG_NONE,
        &CD3DX12_RESOURCE_DESC::Buffer(vertexDataSize),
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&triangleVertices_));

    UINT8* vertexDataStart = nullptr;
    CD3DX12_RANGE range = { 0, 0 };

    HRESULT result = triangleVertices_->Map(0, &range, reinterpret_cast<void**>(&vertexDataStart));
    ThrowIfFailed(result);
    memcpy(vertexDataStart, verticesData, vertexDataSize);
    triangleVertices_->Unmap(0, nullptr);

    triangleVerticesView_.BufferLocation = triangleVertices_->GetGPUVirtualAddress();
    triangleVerticesView_.SizeInBytes = vertexDataSize;
    triangleVerticesView_.StrideInBytes = sizeof(Vertex);

    // Upload buffer logic. Not working.
    /*using namespace Microsoft::WRL;

    Vertex verticesData[] = {
    { { 0.0f, 0.25f, 0.0f },{ 1.0f, 0.0f, 0.0f, 1.0f } },
    { { 0.25f, -0.25f, 0.0f },{ 0.0f, 1.0f, 0.0f, 1.0f } },
    { { -0.25f, -0.25f, 0.0f },{ 0.0f, 0.0f, 1.0f, 1.0f } },
    };

    constexpr UINT vertexDataSize = sizeof(verticesData);

    ComPtr<ID3D12Resource> uploadHeap;
    Device().CreateCommittedResource(
    &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
    D3D12_HEAP_FLAG_NONE,
    &CD3DX12_RESOURCE_DESC::Buffer(vertexDataSize),
    D3D12_RESOURCE_STATE_GENERIC_READ,
    nullptr,
    IID_PPV_ARGS(&uploadHeap));

    UINT8* uploadHeapStart = nullptr;
    CD3DX12_RANGE range = { 0, 0 };
    HRESULT result = uploadHeap->Map(0, &range, reinterpret_cast<void**>(&uploadHeapStart));
    ThrowIfFailed(result);
    memcpy(uploadHeapStart, verticesData, vertexDataSize);
    uploadHeap->Unmap(0, nullptr);

    Device().CreateCommittedResource(
    &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
    D3D12_HEAP_FLAG_NONE,
    &CD3DX12_RESOURCE_DESC::Buffer(vertexDataSize),
    D3D12_RESOURCE_STATE_COPY_DEST,
    nullptr,
    IID_PPV_ARGS(&triangleVertices_));

    D3D12_SUBRESOURCE_DATA bufferData = {};
    bufferData.pData = verticesData;
    bufferData.RowPitch = vertexDataSize;
    bufferData.SlicePitch = 1;

    commandAllocator_.Reset();
    commandList_->Reset(commandAllocator_.Get(), pipelineState_.Get());
    UpdateSubresources(commandList_.Get(), triangleVertices_.Get(), uploadHeap.Get(), 0, 0, 1, &bufferData);
    commandList_->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(triangleVertices_.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_GENERIC_READ));

    triangleVerticesView_.BufferLocation = triangleVertices_->GetGPUVirtualAddress();
    triangleVerticesView_.SizeInBytes = vertexDataSize;
    triangleVerticesView_.StrideInBytes = sizeof(Vertex);

    FlushCommandQueue();*/
}

void DirectAppDelegate::LoadConstantBuffers()
{
    constexpr UINT cbSize = sizeof(SceneConstantBuffer) + 255 & ~255;

    HRESULT result = Device().CreateCommittedResource(
        &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
        D3D12_HEAP_FLAG_NONE,
        &CD3DX12_RESOURCE_DESC::Buffer(cbSize),
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&constantBuffer_)
    );

    D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
    cbvDesc.BufferLocation = constantBuffer_->GetGPUVirtualAddress();
    cbvDesc.SizeInBytes = cbSize;
    Device().CreateConstantBufferView(&cbvDesc, cbvHeap_->GetCPUDescriptorHandleForHeapStart());

    CD3DX12_RANGE readRange = { 0, 0 };
    result = constantBuffer_->Map(0, &readRange, reinterpret_cast<void**>(&constantBufferMappedData_));
    ThrowIfFailed(result);

    memcpy(constantBufferMappedData_, &constantBufferData_, sizeof(constantBufferData_));
}

void DirectAppDelegate::Present()
{
    swapChain_->Present(0, 0);
    currentRenderBuffer = (currentRenderBuffer + 1) % SWAP_CHAIN_BUFFER_COUNT;
}

void DirectAppDelegate::WaitForFence(FrameResource& frameResource)
{
    if (frameResource.CompletedFenceValue() < frameResource.TargetFenceValue()) {
        frameResource.Fence()->SetEventOnCompletion(frameResource.TargetFenceValue(), frameResource.FenceEvent());
        WaitForSingleObject(frameResource.FenceEvent(), INFINITE);
    }
}

void DirectAppDelegate::Draw()
{
    // Reset for command allocator and list.
    FrameResource& frameResource = frameResources[currentRenderBuffer];
    commandQueue_->Signal(frameResource.Fence(), frameResource.TargetFenceValue());
    WaitForFence(frameResource);
    frameResource.SetTargetFenceValue(frameResource.TargetFenceValue() + 1);

    frameResource.CommandAllocator()->Reset();
    commandList_->Reset(frameResource.CommandAllocator(), pipelineState_.Get());

    // Set signature of incoming data.
    commandList_->SetGraphicsRootSignature(rootSignature_.Get());

    commandList_->RSSetViewports(1, &viewportRect_);
    commandList_->RSSetScissorRects(1, &scissorRect_);

    // Set descriptor heaps which will the pipeline will use.
    ID3D12DescriptorHeap* ppHeaps[] = { cbvHeap_.Get() };
    commandList_->SetDescriptorHeaps(1, ppHeaps);
    
    // Set the handle for the 0th descriptor table.
    commandList_->SetGraphicsRootDescriptorTable(0, cbvHeap_->GetGPUDescriptorHandleForHeapStart());


    commandList_->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(frameResource.FrameBuffer(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));
    
    D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = frameResource.CPUDescriptorHandle();
    commandList_->OMSetRenderTargets(1, &rtvHandle, FALSE, &DepthStencilViewHandle());

    // Drawing commands.
    static FLOAT clearColor[4] = { 0.6f, 0.2f, 0.2f, 1.0f };
    commandList_->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
    commandList_->ClearDepthStencilView(DepthStencilViewHandle(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);
    commandList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    commandList_->IASetVertexBuffers(0, 1, &triangleVerticesView_);
    commandList_->DrawInstanced(3, 1, 0, 0);

    commandList_->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(frameResource.FrameBuffer(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

    commandList_->Close();

    ID3D12CommandList* list[] = { commandList_.Get() };
    commandQueue_->ExecuteCommandLists(1, list);
    Present();
}

void DirectAppDelegate::CustomAction()
{
    constexpr float offset = 0.005f;
    constexpr float offsetBounds = 1.25f;

    constantBufferData_.offset.x += offset;
    if (constantBufferData_.offset.x > offsetBounds) {
        constantBufferData_.offset.x -= 2 * offsetBounds;
    }
    memcpy(constantBufferMappedData_, &constantBufferData_, sizeof(constantBufferData_));
}