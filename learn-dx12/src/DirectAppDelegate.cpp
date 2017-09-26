#include <pch.hpp>
#include <thread>
#include <chrono>

#include <Utility\DirectAppDelegate.hpp>

void DirectAppDelegate::start(Application& application)
{
    gpuAccess_ = std::make_unique<GPUAccess>(application);

    Microsoft::WRL::ComPtr<ID3D12CommandAllocator> startupCommandAllocator;
    Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> startupCommandList;
    Microsoft::WRL::ComPtr<ID3D12Fence> startupFence;

    HRESULT result = {};
    //result = Device().CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&startupCommandAllocator)); ThrowIfFailed(result);
    //result = Device().CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, startupCommandAllocator.Get(), nullptr, IID_PPV_ARGS(&startupCommandList)); ThrowIfFailed(result);
    //result = Device().CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&startupFence));
    //
    //CreateRootSignature();
    //CreatePipelineState();
    //
    //LoadTriangleVertices(startupCommandList.Get());
    //LoadConstantBuffers();
    //
    //gameTimer_.Reset();
    //
    //startupCommandList->Close();
    //ID3D12CommandList* list[] = { startupCommandList.Get() };
    //
    //gpuAccess_->RenderingQueue()->ExecuteCommandLists(1, list);
    //gpuAccess_->RenderingQueue()->Signal(startupFence.Get(), 1);
    //
    //// Wait for initialization queue to be processed:
    //HANDLE initEvent = CreateEvent(nullptr, false, false, nullptr);
    //if (startupFence->GetCompletedValue() < 1) {
    //    startupFence->SetEventOnCompletion(1, initEvent);
    //    WaitForSingleObject(initEvent, INFINITE);
    //}
    //
}

void DirectAppDelegate::update(Application& application)
{
    //CustomAction();
    //Draw();

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
//
//ID3D12Device& DirectAppDelegate::Device()
//{
//    return *device_.Get();
//}

void DirectAppDelegate::CreateConstantBufferDescriptorHeap()
{
    //D3D12_DESCRIPTOR_HEAP_DESC cbvDescr = {};
    //cbvDescr.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    //cbvDescr.NumDescriptors = 1;
    //cbvDescr.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    //cbvDescr.NodeMask = 0;
    //
    //HRESULT result = Device().CreateDescriptorHeap(&cbvDescr, IID_PPV_ARGS(&cbvHeap_));
    //ThrowIfFailed(result);
    //
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

    //result = Device().CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&rootSignature_));
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
    psoDesc.RTVFormats[0] = GPUAccess::backBufferFormat_;
    psoDesc.SampleDesc.Count = 1;

    //HRESULT result = Device().CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&pipelineState_));
    //ThrowIfFailed(result);
}

void DirectAppDelegate::LoadTriangleVertices(ID3D12GraphicsCommandList* startupCommandList)
{
    using namespace Microsoft::WRL;

    Vertex verticesData[] = {
        { { 0.0f, 0.25f, 0.0f },{ 1.0f, 0.0f, 0.0f, 1.0f } },
        { { 0.25f, -0.25f, 0.0f },{ 0.0f, 1.0f, 0.0f, 1.0f } },
        { { -0.25f, -0.25f, 0.0f },{ 0.0f, 0.0f, 1.0f, 1.0f } },
    };

    constexpr UINT vertexDataSize = sizeof(verticesData);

    //GPUUploadHeap uploadHeap{ Device(), verticesData, vertexDataSize };
    //
    //triangleVertices_ = uploadHeap.GenerateDefault(startupCommandList);

    //Device().CreateCommittedResource(
    //    &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
    //    D3D12_HEAP_FLAG_NONE,
    //    &CD3DX12_RESOURCE_DESC::Buffer(vertexDataSize),
    //    D3D12_RESOURCE_STATE_GENERIC_READ,
    //    nullptr,
    //    IID_PPV_ARGS(&triangleVertices_));

    UINT8* vertexDataStart = nullptr;
    CD3DX12_RANGE range = { 0, 0 };

    HRESULT result = triangleVertices_->Map(0, &range, reinterpret_cast<void**>(&vertexDataStart));
    ThrowIfFailed(result);
    memcpy(vertexDataStart, verticesData, vertexDataSize);
    triangleVertices_->Unmap(0, nullptr);

    triangleVerticesView_.BufferLocation = triangleVertices_->GetGPUVirtualAddress();
    triangleVerticesView_.SizeInBytes = vertexDataSize;
    triangleVerticesView_.StrideInBytes = sizeof(Vertex);

}

void DirectAppDelegate::LoadConstantBuffers()
{
    constexpr UINT cbSize = sizeof(SceneConstantBuffer) + 255 & ~255;

    //HRESULT result = Device().CreateCommittedResource(
    //    &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
    //    D3D12_HEAP_FLAG_NONE,
    //    &CD3DX12_RESOURCE_DESC::Buffer(cbSize),
    //    D3D12_RESOURCE_STATE_GENERIC_READ,
    //    nullptr,
    //    IID_PPV_ARGS(&constantBuffer_)
    //);
    //
    //D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
    //cbvDesc.BufferLocation = constantBuffer_->GetGPUVirtualAddress();
    //cbvDesc.SizeInBytes = cbSize;
    //Device().CreateConstantBufferView(&cbvDesc, cbvHeap_->GetCPUDescriptorHandleForHeapStart());
    //
    //CD3DX12_RANGE readRange = { 0, 0 };
    //result = constantBuffer_->Map(0, &readRange, reinterpret_cast<void**>(&constantBufferMappedData_));
    //ThrowIfFailed(result);
    //
    //memcpy(constantBufferMappedData_, &constantBufferData_, sizeof(constantBufferData_));
}

//void DirectAppDelegate::Draw()
//{
//    gpuAccess_->Begin();
//    
//    // Set signature of incoming data.
//    gpuAccess_->Commit().SetGraphicsRootSignature(rootSignature_.Get());
//
//    gpuAccess_->Commit().RSSetViewports(1, &viewportRect_);
//    gpuAccess_->Commit().RSSetScissorRects(1, &scissorRect_);
//
//    // Set descriptor heaps which will the pipeline will use.
//    ID3D12DescriptorHeap* ppHeaps[] = { cbvHeap_.Get() };
//    gpuAccess_->Commit().SetDescriptorHeaps(1, ppHeaps);
//    
//    // Set the handle for the 0th descriptor table.
//    gpuAccess_->Commit().SetGraphicsRootDescriptorTable(0, cbvHeap_->GetGPUDescriptorHandleForHeapStart());
//
//
//    gpuAccess_->Commit().ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(frameResource.FrameBuffer(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));
//    
//    D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = frameResource.CPUDescriptorHandle();
//    gpuAccess_->Commit().OMSetRenderTargets(1, &rtvHandle, FALSE, &DepthStencilViewHandle());
//
//    // Drawing commands.
//    static FLOAT clearColor[4] = { 0.6f, 0.2f, 0.2f, 1.0f };
//    gpuAccess_->Commit().ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
//    gpuAccess_->Commit().ClearDepthStencilView(DepthStencilViewHandle(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);
//    gpuAccess_->Commit().IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//    gpuAccess_->Commit().IASetVertexBuffers(0, 1, &triangleVerticesView_);
//    gpuAccess_->Commit().DrawInstanced(3, 1, 0, 0);
//
//    gpuAccess_->Commit().ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(frameResource.FrameBuffer(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));
//
//    gpuAccess_->End();
//    Present();
//}

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