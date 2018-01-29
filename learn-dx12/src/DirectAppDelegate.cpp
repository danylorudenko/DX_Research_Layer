#include <pch.hpp>
#include <thread>
#include <chrono>

#include <Foundation\DirectAppDelegate.hpp>
#include <Rendering\Data\FrameGraph\GPUGraphicsGraphNode.hpp>
#include <Rendering\Data\FrameGraph\GPUPresentGraphNode.hpp>
#include <Rendering\Data\GPURenderItem.hpp>

void DirectAppDelegate::start(Application& application)
{
    auto constexpr framesCount = GPUFoundation::SWAP_CHAIN_BUFFER_COUNT;

    /////////////////////////////////////////////////////////////////////////////
    // TESTING CONSTANT VERTEX DATA
    /////////////////////////////////////////////////////////////////////////////

    Vertex verticesData[] = {
        { { 0.0f, 0.25f, 0.0f },{ 1.0f, 0.0f, 0.0f, 1.0f } },
        { { 0.25f, -0.25f, 0.0f },{ 0.0f, 1.0f, 0.0f, 1.0f } },
        { { -0.25f, -0.25f, 0.0f },{ 0.0f, 0.0f, 1.0f, 1.0f } },
    };

    auto const verticesDataSize = sizeof(verticesData);

    /////////////////////////////////////////////////////////////////////////////
    
    
    gpuFoundation_ = std::make_unique<GPUFoundation>(application);
    gameTimer_.Reset();

    auto& initializationEngine = gpuFoundation_->Engine<GPU_ENGINE_TYPE_DIRECT>();


    auto rootSignature = CreateRootSignature();
    GPURootSignature triangleRootSignature_{ rootSignature };

    auto constexpr constBufferSize = (sizeof(constantBufferData_) + 255) & ~255;
    GPUResourceHandle constBufferHandles[framesCount];
    for (size_t i = 0; i < framesCount; i++) {
        constBufferHandles[i] = gpuFoundation_->AllocUploadResource(CD3DX12_RESOURCE_DESC::Buffer(constBufferSize), D3D12_RESOURCE_STATE_GENERIC_READ);
    }
    
    D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc[framesCount];
    for (size_t i = 0; i < framesCount; i++) {
        cbvDesc[i].BufferLocation = constBufferHandles[i].Resource().Get()->GetGPUVirtualAddress();
        cbvDesc[i].SizeInBytes = constBufferSize;
    }
    
    constBuffer_ = gpuFoundation_->AllocCBV(framesCount, constBufferHandles, cbvDesc, D3D12_RESOURCE_STATE_GENERIC_READ);
    triangleRootSignature_.PushRootArgument(0, GPUResourceViewTable{ 1, &constBuffer_ });


    auto uploadBuffer = gpuFoundation_->AllocUploadResource(CD3DX12_RESOURCE_DESC::Buffer(verticesDataSize), D3D12_RESOURCE_STATE_GENERIC_READ);
    auto triangleMesh = gpuFoundation_->AllocDefaultResource(CD3DX12_RESOURCE_DESC::Buffer(verticesDataSize), D3D12_RESOURCE_STATE_COPY_DEST);
    
    void* uploadBufferPtr = nullptr;
    uploadBuffer.Resource().Get()->Map(0, nullptr, &uploadBufferPtr);
    std::memcpy(uploadBufferPtr, verticesData, verticesDataSize);
    uploadBuffer.Resource().Get()->Unmap(0, nullptr);

    triangleMesh.Resource().UpdateData(initializationEngine, uploadBuffer.Resource(), 0, verticesDataSize);
    triangleMesh.Resource().Transition(initializationEngine, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);

    D3D12_VERTEX_BUFFER_VIEW triangleView{};
    triangleView.BufferLocation = triangleMesh.Resource().Get()->GetGPUVirtualAddress();
    triangleView.SizeInBytes = verticesDataSize;
    triangleView.StrideInBytes = sizeof(Vertex);

    GPURenderItem triangleRenderItem{};
    triangleRenderItem.InsertVertexView(3, triangleMesh, triangleView, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


    auto pipelineState = CreatePipelineState(rootSignature);
    GPUPipelineState trianglePipelineState_{ pipelineState };
    auto& triangleGraphNode = gpuFoundation_->FrameGraph().AddGraphicsNode(nullptr, gpuFoundation_->Engine<GPU_ENGINE_TYPE_DIRECT>(), std::move(trianglePipelineState_), std::move(triangleRootSignature_));
    triangleGraphNode.ImportRenderItem(triangleRenderItem);
    auto swapChainRTV = gpuFoundation_->SwapChainRTV();
    triangleGraphNode.ImportRenderTargets(1, &swapChainRTV);

    Color clearColor{ 0.5f, 0.2f, 0.3f, 1.0f };
    triangleGraphNode.ImportClearColors(&clearColor, 1);

    D3D12_VIEWPORT viewport{};
    viewport.TopLeftX = 0.0f;
    viewport.TopLeftY = 0.0f;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    viewport.Width = static_cast<float>(GPUFoundation::WIDTH);
    viewport.Height = static_cast<float>(GPUFoundation::HEIGHT);
    CD3DX12_RECT scissorRect{ 0, 0, static_cast<LONG>(GPUFoundation::WIDTH), static_cast<LONG>(GPUFoundation::HEIGHT) };
    triangleGraphNode.ImportViewportScissor(viewport, scissorRect);


    auto& presentNode_ = gpuFoundation_->FrameGraph().AddPresentNode(triangleGraphNode, gpuFoundation_->SwapChain(), gpuFoundation_->Engine<GPU_ENGINE_TYPE_DIRECT>());
    presentNode_.ImportRenderTarget(swapChainRTV);
    

    gpuFoundation_->FrameGraph().ParseGraphToQueue();

    initializationEngine.FlushReset();
}

void DirectAppDelegate::update(Application& application)
{
    std::size_t const normalizedFrameIndex = frameIndex_ % GPUFoundation::SWAP_CHAIN_BUFFER_COUNT;
    CustomAction(normalizedFrameIndex);
    Draw(normalizedFrameIndex);

    gameTimer_.Tick();
    DisplayFrameTime(application, Timer().DeltaTime());

    ++frameIndex_;
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

Microsoft::WRL::ComPtr<ID3D12RootSignature> DirectAppDelegate::CreateRootSignature()
{
    CD3DX12_ROOT_PARAMETER rootParameters[1];
    CD3DX12_DESCRIPTOR_RANGE ranges[1];

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
    
    Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature = nullptr;
    gpuFoundation_->CreateRootSignature(signature, rootSignature);
    return rootSignature;
}

Microsoft::WRL::ComPtr<ID3D12PipelineState> DirectAppDelegate::CreatePipelineState(Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature)
{
    Microsoft::WRL::ComPtr<ID3DBlob> vertexShader;
    Microsoft::WRL::ComPtr<ID3DBlob> pixelShader;

    gpuFoundation_->CompileShader(L"Shaders\\triangle_shader.hlsl", vertexShader, "VS", "vs_5_0");
    gpuFoundation_->CompileShader(L"Shaders\\triangle_shader.hlsl", pixelShader, "PS", "ps_5_0");

    // Define the vertex input layout.
    D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
    };

    // Setup pipeline state, which inludes setting shaders.
    D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
    psoDesc.InputLayout = { inputElementDescs, 2 };
    psoDesc.pRootSignature = rootSignature.Get();
    psoDesc.VS = CD3DX12_SHADER_BYTECODE(vertexShader.Get());
    psoDesc.PS = CD3DX12_SHADER_BYTECODE(pixelShader.Get());
    psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
    psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
    psoDesc.DepthStencilState.DepthEnable = FALSE;
    psoDesc.DepthStencilState.StencilEnable = FALSE;
    psoDesc.SampleMask = UINT_MAX;
    psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    psoDesc.NumRenderTargets = 1;
    psoDesc.RTVFormats[0] = GPUFoundation::backBufferFormat_;
    psoDesc.SampleDesc.Count = 1;
    psoDesc.SampleDesc.Quality = 0;

    Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState = nullptr;
    gpuFoundation_->CreatePSO(pipelineState, &psoDesc);
    return pipelineState;
}

void DirectAppDelegate::Draw(std::size_t frameIndex)
{
    auto& graph = gpuFoundation_->FrameGraph();
    auto& graphIterator = graph.GraphQueueStart();
    auto& graphEnd = graph.GraphQueueEnd();
    
    auto& directEngine = gpuFoundation_->Engine<GPU_ENGINE_TYPE_DIRECT>();

    gpuFoundation_->SetDefaultDescriptorHeaps();

    while (graphIterator != graphEnd) {
        (*graphIterator)->Process(frameIndex);
        ++graphIterator;
    }
}

void DirectAppDelegate::CustomAction(std::size_t frameIndex)
{
    constexpr float offset = 0.0005f;
    constexpr float offsetBounds = 1.25f;

    constantBufferData_.offset.x += offset;
    if (constantBufferData_.offset.x > offsetBounds) {
        constantBufferData_.offset.x -= 2 * offsetBounds;
    }

    static D3D12_RANGE readRange{ 0, 0 };


    void* mappedData = nullptr;
    constBuffer_.View(frameIndex).Resource().Get()->Map(0, nullptr, &mappedData);

    std::memcpy(mappedData, &constantBufferData_, sizeof(constantBufferData_));

    static D3D12_RANGE writeRange{ 0, sizeof(constantBufferData_) };
    constBuffer_.View(frameIndex).Resource().Get()->Unmap(0, &writeRange);
}