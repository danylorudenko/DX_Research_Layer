#pragma once

#include <pch.hpp>

#include <Rendering\Resource\GPUResource.hpp>

namespace DXRL
{

class GPUDelegate;

class GPUSwapChain
{
public:
    GPUSwapChain();
    GPUSwapChain(GPUDelegate& foundation, HWND windowHandle, DXGI_FORMAT backBufferFormat, std::size_t width, std::size_t height, std::size_t bufferCount);
    GPUSwapChain(GPUSwapChain const&) = delete;
    GPUSwapChain(GPUSwapChain&& rhs);

    GPUSwapChain& operator=(GPUSwapChain const&) = delete;
    GPUSwapChain& operator=(GPUSwapChain&& rhs);

    std::size_t BufferCount() const;
    GPUResource& AccessRenderBuffer(std::size_t frameIndex);

    void Present(std::size_t syncInterval);

private:
    std::size_t bufferCount_ = 0;
    Microsoft::WRL::ComPtr<IDXGISwapChain> swapChainHandle_ = nullptr;

    std::vector<GPUResource> renderBuffers_;
};

}