#include <Rendering\GPUSwapChain.hpp>

GPUSwapChain::GPUSwapChain() = default;

GPUSwapChain::GPUSwapChain(GPUFoundation& foundation, HWND windowHandle, DXGI_FORMAT backBufferFormat, std::size_t width, std::size_t height, std::size_t bufferCount)
{
    DXGI_SWAP_CHAIN_DESC swapChainDesc;
    swapChainDesc.BufferDesc.Format = backBufferFormat;
    swapChainDesc.BufferDesc.Width = width;
    swapChainDesc.BufferDesc.Height = height;
    swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
    swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
    swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;

    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.BufferCount = bufferCount;
    swapChainDesc.OutputWindow = windowHandle;
    swapChainDesc.Windowed = true;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    {
        auto& factory = foundation.DXGIFactory();
        auto queuePtr = foundation.Engine<GPU_ENGINE_TYPE_DIRECT>().CommandQueue();

        auto const result = factory->CreateSwapChain(queuePtr, &swapChainDesc, swapChainHandle_.GetAddressOf());
        ThrowIfFailed(result);
    }

    bufferCount_ = bufferCount;

    ////////////////////////////////////////////////////////////

    for (size_t i = 0; i < bufferCount_; i++) {
        Microsoft::WRL::ComPtr<ID3D12Resource> temp{ nullptr };
        auto const result = swapChainHandle_->GetBuffer(static_cast<UINT>(i), IID_PPV_ARGS(temp.GetAddressOf()));
        ThrowIfFailed(result);
        renderBuffers_.push_back(GPUResource{ std::move(temp), D3D12_RESOURCE_STATE_COMMON });
    }
}

GPUSwapChain::GPUSwapChain(GPUSwapChain&& rhs) = default;

GPUSwapChain& GPUSwapChain::operator=(GPUSwapChain&& rhs) = default;