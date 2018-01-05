#include <pch.hpp>
#include <thread>
#include <chrono>

#include <Foundation\DirectAppDelegate.hpp>
#include <Rendering\Data\FrameGraph\GPUGraphicsGraphNode.hpp>
#include <Rendering\Data\FrameGraph\GPUPresentGraphNode.hpp>
#include <Rendering\Data\GPUUploadHeap.hpp>
#include <Rendering\Data\GPURenderItem.hpp>

void DirectAppDelegate::start(Application& application)
{
    /*{
        auto const result = DXGIGetDebugInterface1(0, IID_PPV_ARGS(&graphicsAnalysis_));
        if (SUCCEEDED(result)) {
            graphicsAnalysis_->BeginCapture();
        }
        else {
            graphicsAnalysis_ = nullptr;
        }
        
    }*/

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
    
    
    gpuAccess_ = GPUFoundation{ application };
    gameTimer_.Reset();

    auto& initializationEngine = gpuAccess_.Engine<GPU_ENGINE_TYPE_DIRECT>();

    auto rootSignature = CreateRootSignature();
    auto pipelineState = CreatePipelineState(rootSignature);

    triangleRootSignature_ = GPURootSignature{ rootSignature };
    trianglePipelineState_ = GPUPipelineState{ pipelineState };



    auto constexpr framesCount = static_cast<int>(GPUFoundation::SWAP_CHAIN_BUFFER_COUNT);


    auto constexpr constBufferSize = (sizeof(constantBufferData_) + 255) & ~255;
    constantBuffer_ = GPUUploadHeap{ 
        framesCount, gpuAccess_.Device().Get(), 
        &constantBufferData_, constBufferSize, true };

    D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc{};
    cbvDesc.BufferLocation = constantBuffer_.GPUVirtualAddress(0);
    cbvDesc.SizeInBytes = constBufferSize;
    constantBufferView_ = gpuAccess_.DescriptorHeap().AllocCbvLinear(&constantBuffer_, cbvDesc, D3D12_RESOURCE_STATE_GENERIC_READ, "constBuffer", framesCount);




    std::vector<GPUFrameResourceDescriptor> tableStartViews{ 1, constantBufferView_ };
    std::vector<GPUFrameRootTablesMap::StateAndResource> tableResources{ 1, std::make_pair(D3D12_RESOURCE_STATE_GENERIC_READ, &constantBuffer_) };
    GPUFrameRootTablesMap rootTableMap{ gpuAccess_.DescriptorHeap().HeapCbvSrvUav(), tableStartViews, tableResources };

    triangleRootSignature_.ImportPassFrameRootDescriptorTable(rootTableMap);


    


    GPUUploadHeap triangleMeshUploadHeap{ 1, gpuAccess_.Device().Get(), &verticesData, verticesDataSize };
    triangleMesh_ = GPUFrameResource{ 1, gpuAccess_.Device().Get(), verticesDataSize, &CD3DX12_RESOURCE_DESC::Buffer(verticesDataSize), D3D12_RESOURCE_STATE_COPY_DEST };
    triangleMesh_.UpdateData(0, initializationEngine.CommandList(), 0, triangleMeshUploadHeap, 0, 0, verticesDataSize);
    triangleMesh_.Transition(0, initializationEngine.CommandList(), D3D12_RESOURCE_STATE_COMMON);
    initializationEngine.FlushReset();


    D3D12_VERTEX_BUFFER_VIEW triangleView{};
    triangleView.BufferLocation = triangleMesh_.GPUVirtualAddress(0);
    triangleView.SizeInBytes = verticesDataSize;
    triangleView.StrideInBytes = sizeof(Vertex);



    
    GPURenderItem triangleRenderItem{};
    triangleRenderItem.vertexBufferDescriptor_ = triangleView;
    triangleRenderItem.vertexCount_ = 3;
    triangleRenderItem.primitiveTopology_ = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
    triangleRenderItem.hasIndexBuffer_ = false;




    triangleGraphNode_ = GPUGraphicsGraphNode{ &gpuAccess_.Engine<GPU_ENGINE_TYPE_DIRECT>(), &triangleRootSignature_, &trianglePipelineState_, framesCount };
    triangleGraphNode_.ImportRenderItem(triangleRenderItem);
    triangleGraphNode_.ImportRenderTarget(gpuAccess_.FinalRenderTargetViews());
    triangleGraphNode_.ImportDepthStencilTarget(gpuAccess_.FinalDepthSteniclViews());

    Color clearColor{ 0.5f, 0.2f, 0.3f, 1.0f };
    triangleGraphNode_.ImportClearColors(&clearColor, 1);

    D3D12_VIEWPORT viewport{};
    viewport.TopLeftX = 0.0f;
    viewport.TopLeftY = 0.0f;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    viewport.Width = static_cast<float>(GPUFoundation::WIDTH);
    viewport.Height = static_cast<float>(GPUFoundation::HEIGHT);
    CD3DX12_RECT scissorRect{ 0, 0, static_cast<LONG>(GPUFoundation::WIDTH), static_cast<LONG>(GPUFoundation::HEIGHT) };
    triangleGraphNode_.ImportViewportScissor(viewport, scissorRect);




    presentNode_ = GPUPresentGraphNode{ gpuAccess_.SwapChain(), &gpuAccess_.Engine<GPU_ENGINE_TYPE_DIRECT>() };
    presentNode_.ImportRenderTarget(gpuAccess_.FinalRenderTargetViews().DescribedResource());
    



    triangleGraphNode_.ImportChildNode(&presentNode_);

    gpuAccess_.FrameGraph().AddParentNode(&triangleGraphNode_);
    gpuAccess_.FrameGraph().ParseGraphToQueue();

    initializationEngine.FlushReset();

    /*if (graphicsAnalysis_ != nullptr) {
        graphicsAnalysis_->EndCapture();
    }*/
}

void DirectAppDelegate::update(Application& application)
{
    if (frameIndex_ < 3 && frameIndex_ > 0 && graphicsAnalysis_ != nullptr) {
        graphicsAnalysis_->BeginCapture();
    }
    
    int const normalizedFrameIndex = frameIndex_ % GPUFoundation::SWAP_CHAIN_BUFFER_COUNT;
    CustomAction(normalizedFrameIndex);
    Draw(normalizedFrameIndex);

    gameTimer_.Tick();
    DisplayFrameTime(application, Timer().DeltaTime());

    if (frameIndex_ < 3 && graphicsAnalysis_ != nullptr) {
        graphicsAnalysis_->EndCapture();
    }

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
    //SetWindowText(windowHandle, windowText_.c_str());
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
    gpuAccess_.CreateRootSignature(signature, rootSignature);
    return rootSignature;
}

Microsoft::WRL::ComPtr<ID3D12PipelineState> DirectAppDelegate::CreatePipelineState(Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature)
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
    gpuAccess_.CreatePSO(pipelineState, &psoDesc);
    return pipelineState;
}

void DirectAppDelegate::Draw(int frameIndex)
{
    auto& graph = gpuAccess_.FrameGraph();
    auto& graphIterator = graph.GraphQueueStart();
    auto& graphEnd = graph.GraphQueueEnd();
    
    auto& directEngine = gpuAccess_.Engine<GPU_ENGINE_TYPE_DIRECT>();

    while (graphIterator != graphEnd) {
        (*graphIterator)->Process(frameIndex);
        ++graphIterator;
    }

    
}

void DirectAppDelegate::CustomAction(int frameIndex)
{
    constexpr float offset = 0.0005f;
    constexpr float offsetBounds = 1.25f;

    constantBufferData_.offset.x += offset;
    if (constantBufferData_.offset.x > offsetBounds) {
        constantBufferData_.offset.x -= 2 * offsetBounds;
    }

    //static D3D12_RANGE readRange{ 0, 0 };


    void* mappedData = nullptr;
    constantBuffer_.Map(frameIndex, &mappedData, nullptr);

    std::memcpy(mappedData, &constantBufferData_, sizeof(constantBufferData_));

    static D3D12_RANGE writeRange{ 0, sizeof(constantBufferData_) };
    constantBuffer_.Unmap(frameIndex, &writeRange);
}