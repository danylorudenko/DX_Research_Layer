#include <pch.hpp>

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
    float x, y, z;
};

struct Normal
{
    float x, y, z;
};

struct Vertex
{
    Pos position_;
    Normal normal_;;
};

void DirectAppDelegate::start(Application& application)
{
    winProcDelegate_ = DirectWinProcDelegate{ this };
    application.window().winProcHandler(&winProcDelegate_);
    
    auto constexpr framesCount = DXRL::GPUFoundation::SWAP_CHAIN_BUFFER_COUNT;


    gpuFoundation_ = std::make_unique<DXRL::GPUFoundation>(application);
    gameTimer_.Reset();

    auto& initializationEngine = gpuFoundation_->Engine<DXRL::GPU_ENGINE_TYPE_DIRECT>();


    ////////////////////////////////////////////////////////////////////////////
	float constexpr planeVertexData[] = {
		 1.0f, 0.0f,  1.0f,   0.0f, 1.0f, 0.0f,
		-1.0f, 0.0f,  1.0f,   0.0f, 1.0f, 0.0f,
		 1.0f, 0.0f, -1.0f,   0.0f, 1.0f, 0.0f,
		-1.0f, 0.0f, -1.0f,   0.0f, 1.0f, 0.0f,
	};

	std::int32_t constexpr planeIndexData[] = {
		2, 1, 0,
		3, 1, 2
	};

    std::ifstream ifstream("skull.model", std::ios_base::binary);
    if (!ifstream.is_open()) {
        assert(false);
    }
    ifstream.seekg(std::ios_base::beg);
    VertHeader header;
    ifstream.read(reinterpret_cast<char*>(&header), sizeof(VertHeader));

    std::size_t const vertexSize = header.vertexSize_;
    std::size_t const indexSize = header.indexSize_;

    std::size_t const vertexBytes = vertexSize * header.vertexCount_;
    std::size_t const indexBytes = indexSize * header.indexCount_;

    BYTE* vertexData = new BYTE[vertexBytes + sizeof(planeVertexData)];
    BYTE* indexData = new BYTE[indexBytes + sizeof(planeIndexData)];

    ifstream.read(reinterpret_cast<char*>(vertexData), vertexBytes);
    ifstream.read(reinterpret_cast<char*>(indexData), indexBytes);

    ifstream.close();

	std::memcpy(vertexData + vertexBytes, planeVertexData, sizeof(planeVertexData));
	std::memcpy(indexData + indexBytes, planeIndexData, sizeof(planeIndexData));


    auto uploadVertexBuffer = gpuFoundation_->AllocUploadResource(CD3DX12_RESOURCE_DESC::Buffer(vertexBytes + sizeof(planeVertexData)), D3D12_RESOURCE_STATE_GENERIC_READ);
    auto vertexBuffer = gpuFoundation_->AllocDefaultResource(CD3DX12_RESOURCE_DESC::Buffer(vertexBytes + sizeof(planeVertexData)), D3D12_RESOURCE_STATE_COPY_DEST);
    auto uploadIndexBuffer = gpuFoundation_->AllocUploadResource(CD3DX12_RESOURCE_DESC::Buffer(indexBytes + sizeof(planeIndexData)), D3D12_RESOURCE_STATE_GENERIC_READ);
    auto indexBuffer = gpuFoundation_->AllocDefaultResource(CD3DX12_RESOURCE_DESC::Buffer(indexBytes + sizeof(planeIndexData)), D3D12_RESOURCE_STATE_COPY_DEST);


    BYTE* mappedVertexData = nullptr;
    uploadVertexBuffer.Resource().Get()->Map(0, nullptr, reinterpret_cast<void**>(&mappedVertexData));
    std::memcpy(mappedVertexData, vertexData, vertexBytes);
	std::memcpy(mappedVertexData + vertexBytes, planeVertexData, sizeof(planeVertexData));
    D3D12_RANGE writtenVertexRange{ 0, vertexBytes + sizeof(planeVertexData) };
    uploadVertexBuffer.Resource().Get()->Unmap(0, &writtenVertexRange);
    mappedVertexData = nullptr;

    BYTE* mappedIndexData = nullptr;
    uploadIndexBuffer.Resource().Get()->Map(0, nullptr, reinterpret_cast<void**>(&mappedIndexData));
    std::memcpy(mappedIndexData, indexData, indexBytes);
	std::memcpy(mappedIndexData + indexBytes, planeIndexData, sizeof(planeIndexData));
    D3D12_RANGE writtenIndexRange{ 0, indexBytes + sizeof(planeIndexData) };
    uploadIndexBuffer.Resource().Get()->Unmap(0, &writtenIndexRange);
    mappedIndexData = nullptr;

    initializationEngine.Commit().CopyBufferRegion(vertexBuffer.Resource().GetPtr(), 0, uploadVertexBuffer.Resource().GetPtr(), 0, vertexBytes + sizeof(planeVertexData));
    initializationEngine.Commit().CopyBufferRegion(indexBuffer.Resource().GetPtr(), 0, uploadIndexBuffer.Resource().GetPtr(), 0, indexBytes + sizeof(planeIndexData));


    vertexBuffer.Resource().Transition(initializationEngine, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);
    indexBuffer.Resource().Transition(initializationEngine, D3D12_RESOURCE_STATE_INDEX_BUFFER);

    delete[] vertexData; vertexData = nullptr;
    delete[] indexData; indexData = nullptr;
    initializationEngine.FlushReset();

    D3D12_VERTEX_BUFFER_VIEW vbView{};
    vbView.BufferLocation = vertexBuffer.Resource().Get()->GetGPUVirtualAddress();
    vbView.SizeInBytes = static_cast<UINT>(vertexBytes);
    vbView.StrideInBytes = sizeof(Vertex);

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


    auto pipelineState = CreatePipelineState(rootSignature);
    DXRL::GPUPipelineState trianglePipelineState_{ pipelineState };
    auto& graphicsGraphNode = gpuFoundation_->FrameGraph().AddGraphicsNode(nullptr, gpuFoundation_->Engine<DXRL::GPU_ENGINE_TYPE_DIRECT>(), std::move(trianglePipelineState_), std::move(triangleRootSignature));

	std::size_t constexpr renderItemsCount = 3;
	DXRL::GPURenderItem renderItem[renderItemsCount];
	for (std::size_t i = 0; i < renderItemsCount; i++) {
		renderItem[i].transform_.Position(DirectX::XMFLOAT3A{ static_cast<float>(i * 1.7f) - 1.7f , 0.0f, 0.0f });
		renderItem[i].transform_.RotationRollPitchYaw(DirectX::XMFLOAT3A{ 90.0f, (i * -45.0f) + 45.0f, 0.0f });
		renderItem[i].transform_.Scale(0.8f);
		renderItem[i].vertexBuffer_ = vertexBuffer;
		renderItem[i].vertexBufferDescriptor_ = vbView;
		renderItem[i].vertexCount_ = header.vertexCount_;
		renderItem[i].primitiveTopology_ = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		renderItem[i].indexBuffer_ = indexBuffer;
		renderItem[i].indexBufferDescriptor_ = ibView;
		renderItem[i].indexCount_ = header.indexCount_;
		renderItem[i].CreateTransformBuffer(framesCount, 1, *gpuFoundation_);

		graphicsGraphNode.ImportRenderItem(std::move(renderItem[i]));
	}

	D3D12_VERTEX_BUFFER_VIEW planeVbView{};
	planeVbView.BufferLocation = vertexBuffer.Resource().Get()->GetGPUVirtualAddress() + vertexBytes;
	planeVbView.SizeInBytes = static_cast<UINT>(sizeof(planeVertexData));
	planeVbView.StrideInBytes = sizeof(Vertex);

	D3D12_INDEX_BUFFER_VIEW planeIbView{};
	planeIbView.BufferLocation = indexBuffer.Resource().Get()->GetGPUVirtualAddress() + indexBytes;
	planeIbView.Format = DXGI_FORMAT_R32_UINT;
	planeIbView.SizeInBytes = static_cast<UINT>(sizeof(planeIndexData));

	DXRL::GPURenderItem planeRenderItem{};
	planeRenderItem.transform_.Position(DirectX::XMFLOAT3A{ 0.0f, -2.0f, 0.0f });
	planeRenderItem.transform_.RotationRollPitchYaw(DirectX::XMFLOAT3A{ 0.0f, 0.0f, 0.0f });
	planeRenderItem.transform_.Scale(6.0f);
	planeRenderItem.vertexBuffer_ = vertexBuffer;
	planeRenderItem.vertexBufferDescriptor_ = planeVbView;
	planeRenderItem.vertexCount_ = 4;
	planeRenderItem.primitiveTopology_ = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	planeRenderItem.indexBuffer_ = indexBuffer;
	planeRenderItem.indexBufferDescriptor_ = planeIbView;
	planeRenderItem.indexCount_ = 6;
	planeRenderItem.CreateTransformBuffer(framesCount, 1, *gpuFoundation_);

	//graphicsGraphNode.ImportRenderItem(std::move(planeRenderItem));

	/////////////////////////////////////////////////////////////

    auto swapChainRTV = gpuFoundation_->SwapChainRTV();
    graphicsGraphNode.ImportRenderTargets(1, &swapChainRTV);
    DXRL::Color clearColor{ 0.1f, 0.11f, 0.12f, 1.0f };
    graphicsGraphNode.ImportClearColors(&clearColor, 1);

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

    graphicsGraphNode.ImportDepthStencilTarget(depthStencilView);
    graphicsGraphNode.ImportDepthStencilSettings(dsSettings);
    

    D3D12_VIEWPORT viewport{};
    viewport.TopLeftX = 0.0f;
    viewport.TopLeftY = 0.0f;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    viewport.Width = static_cast<float>(DXRL::GPUFoundation::WIDTH);
    viewport.Height = static_cast<float>(DXRL::GPUFoundation::HEIGHT);
    CD3DX12_RECT scissorRect{ 0, 0, static_cast<LONG>(DXRL::GPUFoundation::WIDTH), static_cast<LONG>(DXRL::GPUFoundation::HEIGHT) };
    graphicsGraphNode.ImportViewportScissor(viewport, scissorRect);


    auto& presentNode_ = gpuFoundation_->FrameGraph().AddPresentNode(graphicsGraphNode, gpuFoundation_->SwapChain(), gpuFoundation_->Engine<DXRL::GPU_ENGINE_TYPE_DIRECT>());
    presentNode_.ImportRenderTarget(swapChainRTV);
    

    gpuFoundation_->FrameGraph().ParseGraphToQueue();

    initializationEngine.FlushReset();

	camera_.Transform().Position(DirectX::XMFLOAT3A{ 0.0f, 0.0f, -4.0f });
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
        { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
    };

    // Setup pipeline state, which inludes setting shaders.
    D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
    psoDesc.InputLayout = { inputElementDescs, 2 };
    psoDesc.pRootSignature = rootSignature.Get();
    psoDesc.VS = CD3DX12_SHADER_BYTECODE(vertexShader.Get());
    psoDesc.PS = CD3DX12_SHADER_BYTECODE(pixelShader.Get());
    psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	//psoDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
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
	float constexpr PIXEL_TO_ANGLE = 0.3f;

	cameraTargetPitch_ += winProcDelegate_.mouseYDelta_ * PIXEL_TO_ANGLE * DEGREE_TO_RAD;
	cameraTargetYaw_ += winProcDelegate_.mouseXDelta_ * PIXEL_TO_ANGLE * DEGREE_TO_RAD;

	camera_.Transform().Rotation(DirectX::XMQuaternionRotationRollPitchYaw(cameraTargetPitch_, cameraTargetYaw_, 0.0f));
	
	/////////////////////////////////

	float constexpr SPEED_PER_FRAME = 0.02f;

	auto pos = camera_.Transform().PositionSIMD();
	auto forward = camera_.Transform().ForwardSIMD();
	auto right = camera_.Transform().RightSIMD();

	if (winProcDelegate_.WPressed()) {
		pos = DirectX::XMVectorAdd(pos, DirectX::XMVectorMultiply(forward, DirectX::XMVectorReplicate(SPEED_PER_FRAME)));
	}
	if (winProcDelegate_.SPressed()) {
		pos = DirectX::XMVectorSubtract(pos, DirectX::XMVectorMultiply(forward, DirectX::XMVectorReplicate(SPEED_PER_FRAME)));
	}
	if (winProcDelegate_.APressed()) {
		pos = DirectX::XMVectorAdd(pos, DirectX::XMVectorMultiply(right, DirectX::XMVectorReplicate(SPEED_PER_FRAME)));
	}
	if (winProcDelegate_.DPressed()) {
		pos = DirectX::XMVectorSubtract(pos, DirectX::XMVectorMultiply(right, DirectX::XMVectorReplicate(SPEED_PER_FRAME)));
	}

	camera_.Transform().Position(pos);

	/////////////////////////////////

	sceneBufferData_.cameraPosition_ = camera_.Transform().Position();
	sceneBufferData_.perspectiveMatrix_ = camera_.PerspectiveMatrix();
    sceneBufferData_.viewMatrix_ = camera_.ViewMatrix();
	
    char* mappedCameraData = nullptr;
    auto& cameraBuffer = sceneBuffer_.View(frameIndex).Resource();

    cameraBuffer.Get()->Map(0, nullptr, (void**)&mappedCameraData);
    std::memcpy(mappedCameraData, &sceneBufferData_, sizeof(sceneBufferData_));

    D3D12_RANGE writtenRange{ 0, sizeof(sceneBufferData_) };
    cameraBuffer.Get()->Unmap(0, &writtenRange);
}

LRESULT DirectWinProcDelegate::operator()(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    DirectAppDelegate* directAppDelegate = reinterpret_cast<DirectAppDelegate*>(appDelegate_);

	mouseXDelta_ = mouseYDelta_ = 0.0f;

    switch (msg) {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
	case WM_LBUTTONDOWN:
		rotationOn = !rotationOn;
		if(rotationOn) {
			//ShowCursor(false);
		}
		else {
			//ShowCursor(true);
		}
		return 0;
    case WM_MOUSEMOVE: {
		int xMouse = static_cast<int>(LOWORD(lParam));
		int yMouse = static_cast<int>(HIWORD(lParam));

        if (rotationOn) {
			mouseXDelta_ = static_cast<float>(xMouse - prevMouseX_);
			mouseYDelta_ = static_cast<float>(yMouse - prevMouseY_);

			//RECT winRect;
			//GetWindowRect(hwnd, &winRect);
			//ClipCursor(&winRect);
        }

		prevMouseX_ = xMouse;
		prevMouseY_ = yMouse;
        return 0;
    }
	case WM_KEYDOWN:
		if (wParam == 0x57) { // W
			keyMap_ |= (1 << 0);
		}
		if (wParam == 0x41) { // A
			keyMap_ |= (1 << 1);
		}
		if (wParam == 0x53) { // S
			keyMap_ |= (1 << 2);
		}
		if (wParam == 0x44) { // D
			keyMap_ |= (1 << 3);
		}
		return 0;
	case WM_KEYUP:
		if (wParam == 0x57) { // W
			keyMap_ &= ~(1 << 0);
		}
		if (wParam == 0x41) { // A
			keyMap_ &= ~(1 << 1);
		}
		if (wParam == 0x53) { // S
			keyMap_ &= ~(1 << 2);
		}
		if (wParam == 0x44) { // D
			keyMap_ &= ~(1 << 3);
		}
		return 0;
    default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
    }
}

bool DirectWinProcDelegate::WPressed() const
{
	return keyMap_ & (1 << 0);
}

bool DirectWinProcDelegate::APressed() const
{
	return keyMap_ & (1 << 1);
}

bool DirectWinProcDelegate::SPressed() const
{
	return keyMap_ & (1 << 2);
}

bool DirectWinProcDelegate::DPressed() const
{
	return keyMap_ & (1 << 3);
}