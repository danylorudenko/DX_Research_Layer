#pragma once

#include <pch.hpp>

#include <Rendering\GPUEngine.hpp>

#include <Rendering\Data\Resource\GPUResource.hpp>
#include <Rendering\Data\Resource\GPUResourceID.hpp>
#include <Rendering\Data\Resource\ResourceView\GPUResourceViewHeap.hpp>

class GPUResourceView
{
public:
    GPUResourceView();
    GPUResourceView(std::size_t offsetInHeap, GPUResourceViewHeap const& parentHeap);
    GPUResourceView(GPUResourceView const& rhs) = delete;
    GPUResourceView(GPUResourceView&& rhs);

    GPUResourceView& operator=(GPUResourceView const& rhs) = delete;
    GPUResourceView& operator=(GPUResourceView&& rhs);

    virtual D3D12_RESOURCE_STATES TargetState() const = 0;
    D3D12_CPU_DESCRIPTOR_HANDLE CPUHandle() const;

protected:
    std::size_t offsetInHeap_ = 0;
    GPUResourceViewHeap const* parentHeap_ = nullptr;
};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

class GPUShaderVisibleResourceView : public GPUResourceView
{
public:
    GPUShaderVisibleResourceView();
    GPUShaderVisibleResourceView(std::size_t offsetInHeap, GPUResourceViewHeap const& parentHeap, GPUResourceDirectID resourceID, D3D12_RESOURCE_STATES targetState);
    GPUShaderVisibleResourceView(GPUShaderVisibleResourceView const& rhs) = delete;
    GPUShaderVisibleResourceView(GPUShaderVisibleResourceView&& rhs);

    GPUShaderVisibleResourceView& operator=(GPUShaderVisibleResourceView const& rhs) = delete;
    GPUShaderVisibleResourceView& operator=(GPUShaderVisibleResourceView&& rhs);

    virtual D3D12_RESOURCE_STATES TargetState() const override;
    D3D12_GPU_DESCRIPTOR_HANDLE GPUHandle() const;

protected:
    GPUResourceDirectID resourceID_;
    D3D12_RESOURCE_STATES targetState_ = (D3D12_RESOURCE_STATES)0;
};

/////////////////////////////////////////////////////////////////////////////////

class GPUConstantBufferView : public GPUShaderVisibleResourceView
{
public:
    GPUConstantBufferView();
    GPUConstantBufferView(std::size_t offsetInHeap, GPUResourceViewHeap const& parentHeap, GPUResourceDirectID resourceID, D3D12_RESOURCE_STATES targetState);
    GPUConstantBufferView(GPUConstantBufferView const& rhs) = delete;
    GPUConstantBufferView(GPUConstantBufferView&& rhs);

    GPUConstantBufferView& operator=(GPUConstantBufferView const& rhs) = delete;
    GPUConstantBufferView& operator=(GPUConstantBufferView&& rhs);
};

/////////////////////////////////////////////////////////////////////////////////

class GPUShaderResourceView : public GPUShaderVisibleResourceView
{
public:
    GPUShaderResourceView();
    GPUShaderResourceView(std::size_t offsetInHeap, GPUResourceViewHeap const& parentHeap, GPUResourceDirectID resourceID, D3D12_RESOURCE_STATES targetState);
    GPUShaderResourceView(GPUShaderResourceView const& rhs) = delete;
    GPUShaderResourceView(GPUShaderResourceView&& rhs);

    GPUShaderResourceView& operator=(GPUShaderResourceView const& rhs) = delete;
    GPUShaderResourceView& operator=(GPUShaderResourceView&& rhs);
};

/////////////////////////////////////////////////////////////////////////////////

class GPUUnorderedAccessView : public GPUShaderVisibleResourceView
{
public:
    GPUUnorderedAccessView();
    GPUUnorderedAccessView(std::size_t offsetInHeap, GPUResourceViewHeap const& parentHeap, GPUResourceDirectID resourceID, D3D12_RESOURCE_STATES targetState);
    GPUUnorderedAccessView(GPUUnorderedAccessView const& rhs) = delete;
    GPUUnorderedAccessView(GPUUnorderedAccessView&& rhs);

    GPUUnorderedAccessView& operator=(GPUUnorderedAccessView const& rhs) = delete;
    GPUUnorderedAccessView& operator=(GPUUnorderedAccessView&& rhs);
};

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

class GPUDepthStencilView : public GPUResourceView
{
public:
    GPUDepthStencilView();
    GPUDepthStencilView(std::size_t offsetInHeap, GPUResourceViewHeap const& parentHeap, GPUResourceDirectID resourceID, D3D12_RESOURCE_STATES targetState);
    GPUDepthStencilView(GPUDepthStencilView const& rhs) = delete;
    GPUDepthStencilView(GPUDepthStencilView&& rhs);

    GPUDepthStencilView& operator=(GPUDepthStencilView const& rhs) = delete;
    GPUDepthStencilView& operator=(GPUDepthStencilView&& rhs);

    virtual D3D12_RESOURCE_STATES TargetState() const override;

protected:
    GPUResourceDirectID resourceID_;
    D3D12_RESOURCE_STATES targetState_;
};

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

class GPURenderTargetView : public GPUResourceView
{
public:
    GPURenderTargetView();
    GPURenderTargetView(std::size_t offsetInHeap, GPUResourceViewHeap const& parentHeap);
    GPURenderTargetView(GPURenderTargetView const& rhs) = delete;
    GPURenderTargetView(GPURenderTargetView&& rhs);

    GPURenderTargetView& operator=(GPURenderTargetView const& rhs) = delete;
    GPURenderTargetView& operator=(GPURenderTargetView&& rhs);

    virtual D3D12_RESOURCE_STATES TargetState() const override;
};

/////////////////////////////////////////////////////////////////////////////////

class GPUSwapChainRenderTargetView : public GPURenderTargetView
{
public:
    GPUSwapChainRenderTargetView();
    GPUSwapChainRenderTargetView(std::size_t offsetInHeap, GPUResourceViewHeap const& parentHeap, GPUResource& renderBuffer);
    GPUSwapChainRenderTargetView(GPUSwapChainRenderTargetView const& rhs) = delete;
    GPUSwapChainRenderTargetView(GPUSwapChainRenderTargetView&& rhs);

    GPUSwapChainRenderTargetView& operator=(GPUSwapChainRenderTargetView const& rhs) = delete;
    GPUSwapChainRenderTargetView& operator=(GPUSwapChainRenderTargetView&& rhs);

protected:
    GPUResource* renderBuffer_;
};

/////////////////////////////////////////////////////////////////////////////////

class GPUGenericRenderTargetView : public GPURenderTargetView
{
public:
    GPUGenericRenderTargetView();
    GPUGenericRenderTargetView(std::size_t offsetInHeap, GPUResourceViewHeap const& parentHeap, GPUResourceDirectID resourceID);
    GPUGenericRenderTargetView(GPUGenericRenderTargetView const& rhs) = delete;
    GPUGenericRenderTargetView(GPUGenericRenderTargetView&& rhs);

    GPUGenericRenderTargetView& operator=(GPUGenericRenderTargetView const& rhs) = delete;
    GPUGenericRenderTargetView& operator=(GPUGenericRenderTargetView&& rhs);

protected:
    GPUResourceDirectID resourceID_;
};