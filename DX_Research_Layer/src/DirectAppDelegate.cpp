#include <pch.hpp>
#include <thread>
#include <chrono>

#include <Foundation\DirectAppDelegate.hpp>
#include <Rendering\Data\FrameGraph\GPUGraphicsGraphNode.hpp>
#include <Rendering\Data\FrameGraph\GPUPresentGraphNode.hpp>
#include <Rendering\Data\GPURenderItem.hpp>

struct VertHeader
{
    std::uint32_t vertexCount_ = 0;
    std::uint32_t vertexSize_ = 0;
    std::uint32_t indexCount_ = 0;
    std::uint32_t indexSize_ = 0;
};

struct Pos
{
    float x, y, z, w;
};

void DirectAppDelegate::start(Application& application)
{
    //Pos testVertexData[] = {
    //    { 0.0f, 0.25f, 0.0f, 1.0f },
    //    { 0.25f, -0.25f, 0.0f, 1.0f },
    //    { -0.25f, -0.25f, 0.0f, 1.0f },
    //};
    //
    //std::uint32_t testIndexData[] = { 0, 1, 2 };
    
    auto constexpr framesCount = DXRL::GPUFoundation::SWAP_CHAIN_BUFFER_COUNT;


    gpuFoundation_ = std::make_unique<DXRL::GPUFoundation>(application);
    gameTimer_.Reset();

    auto& initializationEngine = gpuFoundation_->Engine<DXRL::GPU_ENGINE_TYPE_DIRECT>();


    ////////////////////////////////////////////////////////////////////////////
    std::ifstream ifstream("test.vert", std::ios_base::binary);
    if (!ifstream.is_open()) {
        assert(false);
    }
    ifstream.seekg(std::ios_base::beg);
    VertHeader header;
    //header.vertexCount_ = 3;
    //header.vertexSize_ = sizeof(Pos);
    //header.indexCount_ = 3;
    //header.indexSize_ = sizeof(std::uint32_t);
    ifstream.read(reinterpret_cast<char*>(&header), sizeof(VertHeader));

    std::size_t const vertexSize = header.vertexSize_;
    std::size_t const indexSize = header.indexSize_;

    std::size_t const vertexBytes = vertexSize * header.vertexCount_;
    std::size_t const indexBytes = indexSize * header.indexCount_;

    BYTE* vertexData = new BYTE[vertexBytes];
    BYTE* indexData = new BYTE[indexBytes];
    //std::memcpy(vertexData, testVertexData, vertexBytes);
    //std::memcpy(indexData, testIndexData, indexBytes);

    ifstream.read(reinterpret_cast<char*>(vertexData), vertexBytes);
    ifstream.read(reinterpret_cast<char*>(indexData), indexBytes);

    ifstream.close();

    auto uploadVertexBuffer = gpuFoundation_->AllocUploadResource(CD3DX12_RESOURCE_DESC::Buffer(vertexBytes), D3D12_RESOURCE_STATE_GENERIC_READ);
    auto vertexBuffer = gpuFoundation_->AllocDefaultResource(CD3DX12_RESOURCE_DESC::Buffer(vertexBytes), D3D12_RESOURCE_STATE_COPY_DEST);
    auto uploadIndexBuffer = gpuFoundation_->AllocUploadResource(CD3DX12_RESOURCE_DESC::Buffer(indexBytes), D3D12_RESOURCE_STATE_GENERIC_READ);
    auto indexBuffer = gpuFoundation_->AllocDefaultResource(CD3DX12_RESOURCE_DESC::Buffer(indexBytes), D3D12_RESOURCE_STATE_COPY_DEST);


    BYTE* mappedVertexData = nullptr;
    uploadVertexBuffer.Resource().Get()->Map(0, nullptr, reinterpret_cast<void**>(&mappedVertexData));
    std::memcpy(mappedVertexData, vertexData, vertexBytes);
    D3D12_RANGE writtenVertexRange{ 0, vertexBytes };
    uploadVertexBuffer.Resource().Get()->Unmap(0, &writtenVertexRange);
    mappedVertexData = nullptr;

    BYTE* mappedIndexData = nullptr;
    uploadIndexBuffer.Resource().Get()->Map(0, nullptr, reinterpret_cast<void**>(&mappedIndexData));
    std::memcpy(mappedIndexData, indexData, indexBytes);
    D3D12_RANGE writtenIndexRange{ 0, indexBytes };
    uploadIndexBuffer.Resource().Get()->Unmap(0, &writtenIndexRange);
    mappedIndexData = nullptr;

    initializationEngine.Commit().CopyBufferRegion(vertexBuffer.Resource().GetPtr(), 0, uploadVertexBuffer.Resource().GetPtr(), 0, vertexBytes);
    initializationEngine.Commit().CopyBufferRegion(indexBuffer.Resource().GetPtr(), 0, uploadIndexBuffer.Resource().GetPtr(), 0, indexBytes);


    vertexBuffer.Resource().Transition(initializationEngine, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);
    indexBuffer.Resource().Transition(initializationEngine, D3D12_RESOURCE_STATE_INDEX_BUFFER);

    delete[] vertexData; vertexData = nullptr;
    delete[] indexData; indexData = nullptr;
    initializationEngine.FlushReset();

    D3D12_VERTEX_BUFFER_VIEW vbView{};
    vbView.BufferLocation = vertexBuffer.Resource().Get()->GetGPUVirtualAddress();
    vbView.SizeInBytes = static_cast<UINT>(vertexBytes);
    vbView.StrideInBytes = sizeof(Pos);

    D3D12_INDEX_BUFFER_VIEW ibView{};
    ibView.BufferLocation = indexBuffer.Resource().Get()->GetGPUVirtualAddress();
    ibView.Format = DXGI_FORMAT_R32_UINT;
    ibView.SizeInBytes = static_cast<UINT>(indexBytes);


    /////////////////////////////////////////////////////////////////////////////


    auto rootSignature = CreateRootSignature();
    DXRL::GPURootSignature triangleRootSignature{ rootSignature };

    auto constexpr sceneConstBufferSize = (sizeof(sceneBufferData_) + 255) & ~255;
    DXRL::GPUResourceHandle constBufferHandles[framesCount];
    for (std::size_t i = 0; i < framesCount; i++) {
        constBufferHandles[i] = gpuFoundation_->AllocUploadResource(CD3DX12_RESOURCE_DESC::Buffer(sceneConstBufferSize), D3D12_RESOURCE_STATE_GENERIC_READ);
    }

    D3D12_CONSTANT_BUFFER_VIEW_DESC sceneCbvDesc[framesCount];
    for (size_t i = 0; i < framesCount; i++) {
        sceneCbvDesc[i].BufferLocation = constBufferHandles[i].Resource().Get()->GetGPUVirtualAddress();
        sceneCbvDesc[i].SizeInBytes = sceneConstBufferSize;
    }
    
    sceneBuffer_ = gpuFoundation_->AllocCBV(framesCount, constBufferHandles, sceneCbvDesc, D3D12_RESOURCE_STATE_GENERIC_READ);
    triangleRootSignature.PushRootArgument(0, DXRL::GPUResourceViewTable{ 1, &sceneBuffer_ });

    auto constexpr transformBufferSize = (sizeof(DirectX::XMFLOAT4X4A) + 255) & ~255;
    DXRL::GPUResourceHandle transformBufferHandles[framesCount];
    for (std::size_t i = 0; i < framesCount; i++) {
        transformBufferHandles[i] = gpuFoundation_->AllocUploadResource(CD3DX12_RESOURCE_DESC::Buffer(transformBufferSize), D3D12_RESOURCE_STATE_GENERIC_READ);
    }

    D3D12_CONSTANT_BUFFER_VIEW_DESC transformCbvDesc[framesCount];
    for (std::size_t i = 0; i < framesCount; i++) {
        transformCbvDesc[i].BufferLocation = transformBufferHandles[i].Resource().Get()->GetGPUVirtualAddress();
        transformCbvDesc[i].SizeInBytes = transformBufferSize;
    }

    auto transformBuffer = gpuFoundation_->AllocCBV(framesCount, transformBufferHandles, transformCbvDesc, D3D12_RESOURCE_STATE_GENERIC_READ);

    DXRL::GPURenderItem triangleRenderItem{};
    triangleRenderItem.transform_.Position(DirectX::XMFLOAT3A{ 0.0f, 0.0f, 100.0f });
    triangleRenderItem.transform_.RotationRollPitchYaw(DirectX::XMFLOAT3A{ 0.0f, 0.0f, 0.0f });
    triangleRenderItem.transform_.Scale(DirectX::XMFLOAT3A(0.2f, 0.5f, 0.5f));
    triangleRenderItem.vertexBuffer_ = vertexBuffer;
    triangleRenderItem.vertexBufferDescriptor_ = vbView;
    triangleRenderItem.vertexCount_ = header.vertexCount_;
    triangleRenderItem.primitiveTopology_ = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
    triangleRenderItem.indexBuffer_ = indexBuffer;
    triangleRenderItem.indexBufferDescriptor_ = ibView;
    triangleRenderItem.indexCount_ = header.indexCount_;
    triangleRenderItem.dynamicArg_ = DXRL::GPURenderItem::GPUDynamicRootArgument{ 1, DXRL::GPUResourceViewTable{1, &transformBuffer} };

    auto pipelineState = CreatePipelineState(rootSignature);
    DXRL::GPUPipelineState trianglePipelineState_{ pipelineState };
    auto& triangleGraphNode = gpuFoundation_->FrameGraph().AddGraphicsNode(nullptr, gpuFoundation_->Engine<DXRL::GPU_ENGINE_TYPE_DIRECT>(), std::move(trianglePipelineState_), std::move(triangleRootSignature));
    triangleGraphNode.ImportRenderItem(triangleRenderItem);

    auto swapChainRTV = gpuFoundation_->SwapChainRTV();
    triangleGraphNode.ImportRenderTargets(1, &swapChainRTV);
    DXRL::Color clearColor{ 0.5f, 0.2f, 0.3f, 1.0f };
    triangleGraphNode.ImportClearColors(&clearColor, 1);

    D3D12_RESOURCE_DESC depthStencilDesc;
    depthStencilDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilDesc.Height = DXRL::GPUFoundation::HEIGHT;
    depthStencilDesc.Width = DXRL::GPUFoundation::WIDTH;
    depthStencilDesc.DepthOrArraySize = 1;
    depthStencilDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
    depthStencilDesc.Alignment = 0;
    depthStencilDesc.MipLevels = 1;
    depthStencilDesc.SampleDesc.Quality = 0;
    depthStencilDesc.SampleDesc.Count = 1;
    depthStencilDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
    DXRL::GPUResourceHandle depthStencilBuffers_[framesCount];
    for (size_t i = 0; i < framesCount; i++) {
        depthStencilBuffers_[i] = gpuFoundation_->AllocDefaultResource(depthStencilDesc, D3D12_RESOURCE_STATE_DEPTH_WRITE);
    }

    D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc;
    dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
    dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    dsvDesc.Flags = D3D12_DSV_FLAG_NONE;
    dsvDesc.Texture2D.MipSlice = 0;
    auto depthStencilView = gpuFoundation_->AllocDSV(framesCount, depthStencilBuffers_, dsvDesc, D3D12_RESOURCE_STATE_DEPTH_WRITE);

    DXRL::GPUDepthStencilSettings dsSettings{};
    dsSettings.ActivateDepth();
    dsSettings.depthStencilClearFlags_ = D3D12_CLEAR_FLAG_DEPTH;

    triangleGraphNode.ImportDepthStencilTarget(depthStencilView);
    triangleGraphNode.ImportDepthStencilSettings(dsSettings);
    

    D3D12_VIEWPORT viewport{};
    viewport.TopLeftX = 0.0f;
    viewport.TopLeftY = 0.0f;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    viewport.Width = static_cast<float>(DXRL::GPUFoundation::WIDTH);
    viewport.Height = static_cast<float>(DXRL::GPUFoundation::HEIGHT);
    CD3DX12_RECT scissorRect{ 0, 0, static_cast<LONG>(DXRL::GPUFoundation::WIDTH), static_cast<LONG>(DXRL::GPUFoundation::HEIGHT) };
    triangleGraphNode.ImportViewportScissor(viewport, scissorRect);


    auto& presentNode_ = gpuFoundation_->FrameGraph().AddPresentNode(triangleGraphNode, gpuFoundation_->SwapChain(), gpuFoundation_->Engine<DXRL::GPU_ENGINE_TYPE_DIRECT>());
    presentNode_.ImportRenderTarget(swapChainRTV);
    

    gpuFoundation_->FrameGraph().ParseGraphToQueue();

    initializationEngine.FlushReset();

    camera_.NearPlane(0.1f);
    camera_.FarPlane(1000.0f);
    camera_.Fow(60.0f);
    camera_.AspectRatio(static_cast<float>(DXRL::GPUFoundation::WIDTH) / static_cast<float>(DXRL::GPUFoundation::HEIGHT));
}

void DirectAppDelegate::update(Application& application)
{
    std::size_t const normalizedFrameIndex = frameIndex_ % DXRL::GPUFoundation::SWAP_CHAIN_BUFFER_COUNT;
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
    CD3DX12_ROOT_PARAMETER rootParameters[2];
    CD3DX12_DESCRIPTOR_RANGE ranges[2];

    ranges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);
    rootParameters[0].InitAsDescriptorTable(1, ranges);

    ranges[1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 1);
    rootParameters[1].InitAsDescriptorTable(1, ranges + 1);

    D3D12_ROOT_SIGNATURE_FLAGS rootSignatureFlags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

    CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
    rootSignatureDesc.Init(2, rootParameters, 0, nullptr, rootSignatureFlags);

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
        { "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
    };

    // Setup pipeline state, which inludes setting shaders.
    D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
    psoDesc.InputLayout = { inputElementDescs, 1 };
    psoDesc.pRootSignature = rootSignature.Get();
    psoDesc.VS = CD3DX12_SHADER_BYTECODE(vertexShader.Get());
    psoDesc.PS = CD3DX12_SHADER_BYTECODE(pixelShader.Get());
    psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
    psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
    psoDesc.DepthStencilState.DepthEnable = TRUE;
    psoDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
    psoDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
    psoDesc.DepthStencilState.StencilEnable = FALSE;
    psoDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
    psoDesc.SampleMask = UINT_MAX;
    psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    psoDesc.NumRenderTargets = 1;
    psoDesc.RTVFormats[0] = DXRL::GPUFoundation::backBufferFormat_;
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
    
    auto& directEngine = gpuFoundation_->Engine<DXRL::GPU_ENGINE_TYPE_DIRECT>();

    gpuFoundation_->SetDefaultDescriptorHeaps();

    while (graphIterator != graphEnd) {
        (*graphIterator)->Process(frameIndex);
        ++graphIterator;
    }
}

void DirectAppDelegate::CustomAction(std::size_t frameIndex)
{
    sceneBufferData_.perspectiveMatrix_ = camera_.PerspectiveMatrix();
    auto cameraPos = DirectX::XMFLOAT3A{ 0, 0, 0 };
    sceneBufferData_.viewMatrix_ = camera_.ViewMatrix(cameraPos, DirectX::XMFLOAT3A{ 0, 0, 1 }, DirectX::XMFLOAT3A{ 0, 1, 0 });
    sceneBufferData_.cameraPosition_ = cameraPos;

    char* mappedCameraData = nullptr;
    auto& cameraBuffer = sceneBuffer_.View(frameIndex).Resource();

    cameraBuffer.Get()->Map(0, nullptr, (void**)&mappedCameraData);
    std::memcpy(mappedCameraData, &sceneBufferData_, sizeof(sceneBufferData_));

    D3D12_RANGE writtenRange{ 0, sizeof(sceneBufferData_) };
    cameraBuffer.Get()->Unmap(0, &writtenRange);
}