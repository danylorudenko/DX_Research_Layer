#include <pch.hpp>
#include <thread>
#include <chrono>

#include <Utility\DirectAppDelegate.hpp>
#include <Rendering\Data\FrameGraph\GPUGraphicsGraphNode.hpp>
#include <Rendering\Data\FrameGraph\GPUPresentGraphNode.hpp>
#include <Rendering\Data\GPUUploadHeap.hpp>
#include <Rendering\Data\GPURenderItem.hpp>

void DirectAppDelegate::start(Application& application)
{
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
    
    
    gpuAccess_ = GPUAccess{ application };
    gameTimer_.Reset();

    auto& initializationEngine = gpuAccess_.Engine<GPU_ENGINE_TYPE_DIRECT>();

    auto rootSignature = CreateRootSignature();
    auto pipelineState = CreatePipelineState(rootSignature);

    triangleRootSignature_ = GPURootSignature{ rootSignature };
    trianglePipelineState_ = GPUPipelineState{ pipelineState };

    auto const framesCount = static_cast<int>(GPUAccess::SWAP_CHAIN_BUFFER_COUNT);
    constantBuffer_ = GPUUploadHeap{ 
        framesCount, gpuAccess_.Device().Get(), 
        &constantBufferData_, sizeof(constantBufferData_), &CD3DX12_RESOURCE_DESC::Buffer(sizeof(constantBufferData_)), true 
    };
    constantBufferView_ = gpuAccess_.DescriptorHeap().AllocCbvLinear(&constantBuffer_, nullptr, D3D12_RESOURCE_STATE_GENERIC_READ, "constBuffer", framesCount);

    std::vector<GPUFrameResourceDescriptor> describedResourcesViews{ 1, constantBufferView_ };
    std::vector<GPUFrameRootTablesMap::StateAndResource> describedResources{ 1, std::make_pair(D3D12_RESOURCE_STATE_GENERIC_READ, &constantBuffer_) };
    GPUFrameRootTablesMap rootTableMap{ framesCount, describedResourcesViews, describedResources };

    triangleRootSignature_.ImportPassFrameRootDescriptorTable(rootTableMap);

    //////////////////////////////////////////////////////////////////////////////////////////////


    GPUUploadHeap triangleMeshUploadHeap{ 1, gpuAccess_.Device().Get(), &verticesData, verticesDataSize, &CD3DX12_RESOURCE_DESC::Buffer(verticesDataSize) };
    triangleMesh_.Transition(0, initializationEngine.CommandList(), D3D12_RESOURCE_STATE_COPY_DEST);
    triangleMesh_.UpdateData(0, initializationEngine.CommandList(), 0, triangleMeshUploadHeap, 0, 0, verticesDataSize);
    triangleMesh_.Transition(0, initializationEngine.CommandList(), D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);

    D3D12_VERTEX_BUFFER_VIEW triangleView{};
    triangleView.BufferLocation = triangleMesh_.GPUVirtualAddress(0);
    triangleView.SizeInBytes = static_cast<UINT>(triangleMesh_.Size());
    triangleView.StrideInBytes = sizeof(float) * 3;
    
    GPURenderItem triangleRenderItem{};
    triangleRenderItem.vertexBufferDescriptor_ = triangleView;
    triangleRenderItem.vertexCount_ = 3;

    triangleGraphNode_ = GPUGraphicsGraphNode{ &initializationEngine, &triangleRootSignature_, &trianglePipelineState_ };
    triangleGraphNode_.AddRenderItem(triangleRenderItem);

    presentNode_ = GPUPresentGraphNode{ gpuAccess_.SwapChain() };
    
    triangleGraphNode_.ImportChildNode(&presentNode_);
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
    psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
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
    psoDesc.RTVFormats[0] = GPUAccess::backBufferFormat_;
    psoDesc.SampleDesc.Count = 1;

    Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState = nullptr;
    gpuAccess_.CreatePSO(pipelineState, &psoDesc);
    return pipelineState;
}

void DirectAppDelegate::Draw()
{
    //GPUEngine& graphicsEngine = gpuAccess_.Engine<GPU_ENGINE_TYPE_DIRECT>();
    //GPUFrameResource& currentFrameResource = gpuAccess_.CurrentFrameResource();
    //
    //// Set general pipeline state.
    //graphicsEngine.Commit().SetPipelineState(pipelineState_.Get());
    //
    //// Set signature of incoming data.
    //graphicsEngine.Commit().SetGraphicsRootSignature(rootSignature_.Get());
    //
    //gpuAccess_.CommitDefaultViewportScissorRects();
    //
    //// Set descriptor heaps which will the pipeline will use.
    //ID3D12DescriptorHeap* ppHeaps[] = { cbvHeap_.Get() };
    //graphicsEngine.Commit().SetDescriptorHeaps(1, ppHeaps);
    //
    //// Set the handle for the 0th descriptor table.
    //graphicsEngine.Commit().SetGraphicsRootDescriptorTable(0, cbvHeap_->GetGPUDescriptorHandleForHeapStart());
    //
    //graphicsEngine.Commit().ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(currentFrameResource.FrameBuffer(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));
    //
    //D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = currentFrameResource.CPURtvDescriptorHandle();
    //D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = currentFrameResource.CPUDsvDescriptorHandle();
    //graphicsEngine.Commit().OMSetRenderTargets(1, &rtvHandle, FALSE, &dsvHandle);
    //
    //// Drawing commands.
    //static FLOAT clearColor[4] = { 0.6f, 0.2f, 0.2f, 1.0f };
    //graphicsEngine.Commit().ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
    //graphicsEngine.Commit().IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    //graphicsEngine.Commit().IASetVertexBuffers(0, 1, &triangleMesh_.verticesView);
    //graphicsEngine.Commit().DrawInstanced(3, 1, 0, 0);
    //                       
    //graphicsEngine.Commit().ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(currentFrameResource.FrameBuffer(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));
    //
    //graphicsEngine.FlushReset();
    //gpuAccess_.Present();
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
    //memcpy(constantBufferMappedData_, &constantBufferData_, sizeof(constantBufferData_));
}