#include <cassert>

#include <Utility\DirectAppDelegate.hpp>

void DirectAppDelegate::start(Application& application)
{
    InitializeD3D12();
    CreateFence();
    GetDescriptorSizes();
}

void DirectAppDelegate::update(Application& application)
{

}

void DirectAppDelegate::shutdown(Application& application)
{

}

Microsoft::WRL::ComPtr<ID3D12Device> DirectAppDelegate::Device()
{
    return device_;
}

void DirectAppDelegate::InitializeD3D12()
{
    using namespace Microsoft::WRL;
#if defined(DEBUG) || defined(_DEBUG)
    {
        
        ComPtr<ID3D12Debug> debugController;
        HRESULT result = D3D12GetDebugInterface(IID_PPV_ARGS(&debugController));
        ThrowIfFailed(result);
    }
#endif

    HRESULT result = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device_));
    ThrowIfFailed(result);
}

void DirectAppDelegate::CreateFence()
{
    HRESULT result = Device()->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence_));
    ThrowIfFailed(result);
}

void DirectAppDelegate::GetDescriptorSizes()
{
    rtvDescriptorSize_ = Device()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    dsvDescriptorSize_ = Device()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
    cvb_srv_uavDescriptorSize_ = Device()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

void DirectAppDelegate::CheckMXAA4xQuality()
{
    D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS qualityLevels;

    qualityLevels.Format = backBufferFormat_;
    qualityLevels.SampleCount = 4;
    qualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
    qualityLevels.NumQualityLevels = 0;

    HRESULT result = Device()->CheckFeatureSupport(D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS, &qualityLevels, sizeof(qualityLevels));
    ThrowIfFailed(result);

    MSAA4xQuality_ = qualityLevels.NumQualityLevels;

    assert(MSAA4xQuality_ > 0 && "Unexpected MSAA quality level.");
}

void DirectAppDelegate::CreateCommandObjects()
{
    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
    queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    
    HRESULT result = Device()->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&commandQueue_));
    ThrowIfFailed(result);

    result = Device()->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator_));
    ThrowIfFailed(result);

    result = Device()->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator_.Get(), nullptr, IID_PPV_ARGS(&commandList_));
    ThrowIfFailed(result);

    commandList_->Close();
}

void DirectAppDelegate::CreateSwapChain()
{

}