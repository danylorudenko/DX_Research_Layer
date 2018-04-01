#include <Rendering\GPUFoundation.hpp>
#include <Rendering\Misc\GPUCapabilities.hpp>

namespace DXRL
{

GPUFoundation::GPUFoundation() = default;

GPUFoundation::GPUFoundation(Application& application)
{
    // Create device and dxgiFactory.
    InitializeD3D12();

    CreateGPUEngines();
    swapChain_ = GPUSwapChain{ *this, application.window().nativeHandle(), backBufferFormat_, WIDTH, HEIGHT, SWAP_CHAIN_BUFFER_COUNT };


    staticResourceAllocator_ = GPUStaticResourceAllocator{ *this };
    viewAllocator_ = GPUResourceViewAllocator{ *this };
    viewContextTable_ = GPUResourceViewContextTable{ swapChain_.BufferCount() };


    D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
    rtvDesc.Format = backBufferFormat_;
    rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
    rtvDesc.Texture2D.MipSlice = 0;
    rtvDesc.Texture2D.PlaneSlice = 0;
    swapChainRTV_ = AllocSwapChainRTV(swapChain_, rtvDesc);
}

GPUFoundation::GPUFoundation(GPUFoundation&& rhs) = default;

GPUFoundation& GPUFoundation::operator=(GPUFoundation&& rhs) = default;

void GPUFoundation::InitializeD3D12()
{
#if defined(DEBUG) || defined(_DEBUG)
    {
        auto const result = D3D12GetDebugInterface(IID_PPV_ARGS(&debugController_));
        DXRL_THROW_IF_FAILED(result);
        debugController_->EnableDebugLayer();

        {
            Microsoft::WRL::ComPtr<ID3D12Debug1> debugController1{ nullptr };
            auto const result = debugController_->QueryInterface(IID_PPV_ARGS(debugController1.GetAddressOf()));
            DXRL_THROW_IF_FAILED(result);
        }
    }
#endif

    HRESULT res = CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory_));
    assert(SUCCEEDED(res) && "Failed to create dxgiFactory\n");

    auto adaptersInfo = GPUCapabilities::ListAdapters(*this);
    OutputDebugStringW(adaptersInfo.c_str());

    device_ = GPUCapabilities::GenerateStandardDeviceQuery(*this);
}

void GPUFoundation::CreateGPUEngines()
{
    engines_[0] = GPUEngine{ device_.Get(), GPU_ENGINE_TYPE_DIRECT, SWAP_CHAIN_BUFFER_COUNT };
    engines_[1] = GPUEngine{ device_.Get(), GPU_ENGINE_TYPE_COPY, SWAP_CHAIN_BUFFER_COUNT };
    engines_[2] = GPUEngine{ device_.Get(), GPU_ENGINE_TYPE_COMPUTE, SWAP_CHAIN_BUFFER_COUNT };
}

void GPUFoundation::ResetAll()
{
    Engine<GPU_ENGINE_TYPE_DIRECT>().Reset();
    Engine<GPU_ENGINE_TYPE_COPY>().Reset();
    Engine<GPU_ENGINE_TYPE_COMPUTE>().Reset();
}

void GPUFoundation::CreateRootSignature(Microsoft::WRL::ComPtr<ID3DBlob> serializedRootSignature, Microsoft::WRL::ComPtr<ID3D12RootSignature>& dest)
{
    auto const result = device_->CreateRootSignature(0, serializedRootSignature->GetBufferPointer(), serializedRootSignature->GetBufferSize(), IID_PPV_ARGS(&dest));
    DXRL_THROW_IF_FAILED(result);
}

void GPUFoundation::CompileShader(LPCWSTR fileName, Microsoft::WRL::ComPtr<ID3DBlob>& dest, LPCSTR entryPoint, LPCSTR type)
{
#if defined (_DEBUG) | (DEBUG)
    UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
    UINT compileFlags = 0;
#endif
    {
        auto const result = D3DCompileFromFile(fileName, nullptr, nullptr, entryPoint, type, compileFlags, 0, &dest, nullptr);
        DXRL_THROW_IF_FAILED(result);
    }
}

void GPUFoundation::CreatePSO(Microsoft::WRL::ComPtr<ID3D12PipelineState>& dest, D3D12_GRAPHICS_PIPELINE_STATE_DESC* desc)
{
    auto const result = device_->CreateGraphicsPipelineState(desc, IID_PPV_ARGS(&dest));
    DXRL_THROW_IF_FAILED(result);
}

GPUResourceHandle GPUFoundation::AllocDefaultResource(D3D12_RESOURCE_DESC const& desc, D3D12_RESOURCE_STATES initialState)
{
    return staticResourceAllocator_.AllocDefault(desc, initialState);
}

GPUResourceHandle GPUFoundation::AllocUploadResource(D3D12_RESOURCE_DESC const& desc, D3D12_RESOURCE_STATES initialState)
{
    return staticResourceAllocator_.AllocUpload(desc, initialState);
}

void GPUFoundation::SetDefaultDescriptorHeaps()
{
    viewAllocator_.SetDefaultHeaps(Engine<GPU_ENGINE_TYPE_DIRECT>());
}

GPUResourceViewHandle GPUFoundation::SwapChainRTV()
{
    return swapChainRTV_;
}

GPUResourceViewHandle GPUFoundation::AllocSwapChainRTV(GPUSwapChain& swapChain, D3D12_RENDER_TARGET_VIEW_DESC const& desc)
{
    auto const framesCount = swapChain.BufferCount();
    std::vector<GPUResourceViewDirectHandle> directHandles;
    directHandles.resize(framesCount);
    for (size_t i = 0; i < framesCount; i++) {
        directHandles[i] = viewAllocator_.AllocSwapChainRTV(swapChain_.AccessRenderBuffer(i), desc);
    }
    return viewContextTable_.InsertView(framesCount, directHandles.data(), viewAllocator_);
}

GPUResourceViewHandle GPUFoundation::AllocRTV(std::size_t frames, GPUResourceHandle const* resources, D3D12_RENDER_TARGET_VIEW_DESC const* desc)
{
    assert(frames == 1 || frames == swapChain_.BufferCount() && "Can't create views with invalid frameCount");
    std::vector<GPUResourceViewDirectHandle> directHandles;
    directHandles.resize(frames);
    for (size_t i = 0; i < frames; i++) {
        directHandles[i] = viewAllocator_.AllocRTV(resources[i], desc[i]);
    }
    return viewContextTable_.InsertView(frames, directHandles.data(), viewAllocator_);
}

GPUResourceViewHandle GPUFoundation::AllocDSV(std::size_t frames, GPUResourceHandle const* resources, D3D12_DEPTH_STENCIL_VIEW_DESC const& desc, D3D12_RESOURCE_STATES targetState)
{
    assert(frames == 1 || frames == swapChain_.BufferCount() && "Can't create views with invalid frameCount");
    std::vector<GPUResourceViewDirectHandle> directHandles;
    directHandles.resize(frames);
    for (size_t i = 0; i < frames; i++) {
        directHandles[i] = viewAllocator_.AllocDSV(resources[i], desc, targetState);
    }
    return viewContextTable_.InsertView(frames, directHandles.data(), viewAllocator_);
}

GPUResourceViewHandle GPUFoundation::AllocCBV(std::size_t frames, GPUResourceHandle const* resources, D3D12_CONSTANT_BUFFER_VIEW_DESC const* desc, D3D12_RESOURCE_STATES targetState)
{
    assert(frames == 1 || frames == swapChain_.BufferCount() && "Can't create views with invalid frameCount");
    std::vector<GPUResourceViewDirectHandle> directHandles;
    directHandles.resize(frames);
    for (size_t i = 0; i < frames; i++) {
        directHandles[i] = viewAllocator_.AllocCBV(resources[i], desc[i], targetState);
    }
    return viewContextTable_.InsertView(frames, directHandles.data(), viewAllocator_);
}

GPUResourceViewHandle GPUFoundation::AllocSRV(std::size_t frames, GPUResourceHandle const* resources, D3D12_SHADER_RESOURCE_VIEW_DESC const* desc, D3D12_RESOURCE_STATES targetState)
{
    assert(frames == 1 || frames == swapChain_.BufferCount() && "Can't create views with invalid frameCount");
    std::vector<GPUResourceViewDirectHandle> directHandles;
    directHandles.resize(frames);
    for (size_t i = 0; i < frames; i++) {
        directHandles[i] = viewAllocator_.AllocSRV(resources[i], desc[i], targetState);
    }
    return viewContextTable_.InsertView(frames, directHandles.data(), viewAllocator_);
}

GPUResourceViewHandle GPUFoundation::AllocUAV(std::size_t frames, GPUResourceHandle const* resources, D3D12_UNORDERED_ACCESS_VIEW_DESC const* desc, D3D12_RESOURCE_STATES targetState)
{
    assert(frames == 1 || frames == swapChain_.BufferCount() && "Can't create views with invalid frameCount");
    std::vector<GPUResourceViewDirectHandle> directHandles;
    directHandles.resize(frames);
    for (size_t i = 0; i < frames; i++) {
        directHandles[i] = viewAllocator_.AllocUAV(resources[i], desc[i], targetState);
    }
    return viewContextTable_.InsertView(frames, directHandles.data(), viewAllocator_);
}

GPUResourceViewTable GPUFoundation::BuildViewTable(std::size_t tableSize, GPUShaderVisibleResourceViewDesc const* descriptions)
{
    std::vector<GPUResourceViewHandle> handles;
    handles.resize(tableSize);
    for (size_t i = 0; i < tableSize; i++) {
        GPUShaderVisibleResourceViewDesc const& currentDesc = descriptions[i];
        switch (currentDesc.type_) {
        case GPU_SHADER_VISIBLE_RESOURCE_VIEW_TYPE_CBV:
            handles[i] = AllocCBV(currentDesc.frames_, currentDesc.resources_, currentDesc.cbvDescs_, currentDesc.targetState_);
            break;
        case GPU_SHADER_VISIBLE_RESOURCE_VIEW_TYPE_SRV:
            handles[i] = AllocSRV(currentDesc.frames_, currentDesc.resources_, currentDesc.srvDescs_, currentDesc.targetState_);
            break;
        case GPU_SHADER_VISIBLE_RESOURCE_VIEW_TYPE_UAV:
            handles[i] = AllocUAV(currentDesc.frames_, currentDesc.resources_, currentDesc.uavDescs_, currentDesc.targetState_);
            break;
        }
    }

    return GPUResourceViewTable(tableSize, handles.data());
}

}