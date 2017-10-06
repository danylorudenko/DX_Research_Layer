#include <pch.hpp>
#include <thread>
#include <chrono>

#include <Utility\DirectAppDelegate.hpp>
#include <Data\GPUUploadHeap.hpp>

void DirectAppDelegate::start(Application& application)
{
    gpuAccess_ = GPUAccess{ application };

    CreateRootSignature();
    CreatePipelineState();
    
    LoadTriangleVertices();
    CreateConstantBufferDescriptorHeap();
    LoadConstantBuffers();

    gameTimer_.Reset();
}

void DirectAppDelegate::update(Application& application)
{
    CustomAction();
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
    windowText_.resize(0);
    windowText_ = L"SPF: ";
    windowText_ += std::to_wstring(drawTime);

    windowText_ += L";    FPS: ";
    windowText_ += std::to_wstring(1 / drawTime);

    HWND windowHandle = application.window().nativeHandle();
    SetWindowText(windowHandle, windowText_.c_str());
}

void DirectAppDelegate::CreateConstantBufferDescriptorHeap()
{
    D3D12_DESCRIPTOR_HEAP_DESC cbvDescr = {};
    cbvDescr.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    cbvDescr.NumDescriptors = 1;
    cbvDescr.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    cbvDescr.NodeMask = 0;
    
    gpuAccess_.CreateDescriptorHeap(&cbvDescr, cbvHeap_);
}

void DirectAppDelegate::CreateRootSignature()
{
    CD3DX12_DESCRIPTOR_RANGE ranges[1];
    CD3DX12_ROOT_PARAMETER rootParameters[1];

    ranges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);
    rootParameters[0].InitAsDescriptorTable(1, ranges);

    D3D12_ROOT_SIGNATURE_FLAGS rootSignatureFlags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

    CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
    rootSignatureDesc.Init(1, rootParameters, 0, nullptr, rootSignatureFlags);

    Microsoft::WRL::ComPtr<ID3DBlob> signature;
    Microsoft::WRL::ComPtr<ID3DBlob> errors;

    {
        auto const result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &signature, &errors);
        ThrowIfFailed(result);
    }
    
    gpuAccess_.CreateRootSignature(signature, rootSignature_);
}

void DirectAppDelegate::CreatePipelineState()
{
    Microsoft::WRL::ComPtr<ID3DBlob> vertexShader;
    Microsoft::WRL::ComPtr<ID3DBlob> pixelShader;

    gpuAccess_.CompileShader(L"Shaders\\triangle_shader.hlsl", vertexShader, "VS", "vs_5_0");
    gpuAccess_.CompileShader(L"Shaders\\triangle_shader.hlsl", pixelShader, "PS", "ps_5_0");

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

    gpuAccess_.CreatePSO(pipelineState_, &psoDesc);
}

void DirectAppDelegate::LoadTriangleVertices()
{
    Vertex verticesData[] = {
        { { 0.0f, 0.25f, 0.0f },{ 1.0f, 0.0f, 0.0f, 1.0f } },
        { { 0.25f, -0.25f, 0.0f },{ 0.0f, 1.0f, 0.0f, 1.0f } },
        { { -0.25f, -0.25f, 0.0f },{ 0.0f, 0.0f, 1.0f, 1.0f } },
    };
    constexpr UINT vertexDataSize = sizeof(verticesData);

    GPUUploadHeap uploadHeap{ gpuAccess_.Device(), verticesData, vertexDataSize };
    
    auto& gpuEngine = gpuAccess_.Engine<GPU_ENGINE_TYPE_DIRECT>();
    triangleVertices_.CreateResource(gpuAccess_.Device(), vertexDataSize, D3D12_RESOURCE_STATE_COPY_DEST);
    triangleVertices_.UpdateData(gpuEngine.CommandList(), 0, uploadHeap, 0, vertexDataSize);
    triangleVertices_.Transition(gpuEngine.CommandList(), D3D12_RESOURCE_STATE_COMMON);
    gpuEngine.FlushReset();
    
    triangleVerticesView_.BufferLocation = triangleVertices_.GPUVirtualAddress();
    triangleVerticesView_.SizeInBytes = vertexDataSize;
    triangleVerticesView_.StrideInBytes = sizeof(Vertex);
}

void DirectAppDelegate::LoadConstantBuffers()
{
    constexpr UINT cbSize = sizeof(SceneConstantBuffer) + 255 & ~255;

    constantBuffer_ = GPUUploadHeap{ gpuAccess_.Device(), nullptr, cbSize, true };
    constantBuffer_.Map(reinterpret_cast<void**>(&constantBufferMappedData_), nullptr);

    D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
    cbvDesc.BufferLocation = constantBuffer_.GPUVirtualAddress();
    cbvDesc.SizeInBytes = cbSize;
    gpuAccess_.CreateConstantBufferView(&cbvDesc, cbvHeap_->GetCPUDescriptorHandleForHeapStart());
}

void DirectAppDelegate::Draw()
{
    GPUEngine& graphicsEngine = gpuAccess_.Engine<GPU_ENGINE_TYPE_DIRECT>();

    // Set general pipeline state.
    graphicsEngine.Commit().SetPipelineState(pipelineState_.Get());

    // Set signature of incoming data.
    graphicsEngine.Commit().SetGraphicsRootSignature(rootSignature_.Get());

    gpuAccess_.CommitDefaultViewportScissorRects();

    // Set descriptor heaps which will the pipeline will use.
    ID3D12DescriptorHeap* ppHeaps[] = { cbvHeap_.Get() };
    graphicsEngine.Commit().SetDescriptorHeaps(1, ppHeaps);
    
    // Set the handle for the 0th descriptor table.
    graphicsEngine.Commit().SetGraphicsRootDescriptorTable(0, cbvHeap_->GetGPUDescriptorHandleForHeapStart());

    graphicsEngine.Commit().ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(gpuAccess_.CurrentFramebuffer(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));
    
    D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = gpuAccess_.CurrentRtvHandle();
    D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = gpuAccess_.DepthStencilHandle();
    graphicsEngine.Commit().OMSetRenderTargets(1, &rtvHandle, FALSE, &dsvHandle);

    // Drawing commands.
    static FLOAT clearColor[4] = { 0.6f, 0.2f, 0.2f, 1.0f };
    graphicsEngine.Commit().ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
    graphicsEngine.Commit().IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    graphicsEngine.Commit().IASetVertexBuffers(0, 1, &triangleVerticesView_);
    graphicsEngine.Commit().DrawInstanced(3, 1, 0, 0);
                           
    graphicsEngine.Commit().ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(gpuAccess_.CurrentFramebuffer(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

    graphicsEngine.FlushReset();
    gpuAccess_.Present();
}

void DirectAppDelegate::CustomAction()
{
    constexpr float offset = 0.0005f;
    constexpr float offsetBounds = 1.25f;

    constantBufferData_.offset.x += offset;
    if (constantBufferData_.offset.x > offsetBounds) {
        constantBufferData_.offset.x -= 2 * offsetBounds;
    }

    // Update constant buffer uploadheap.
    memcpy(constantBufferMappedData_, &constantBufferData_, sizeof(constantBufferData_));
}