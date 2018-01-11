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
    GPUResourceView(GPUResourceView const& rhs);
    GPUResourceView(GPUResourceView&& rhs);

    GPUResourceView& operator=(GPUResourceView const& rhs);
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
    GPUShaderVisibleResourceView(GPUShaderVisibleResourceView const& rhs);
    GPUShaderVisibleResourceView(GPUShaderVisibleResourceView&& rhs);

    GPUShaderVisibleResourceView& operator=(GPUShaderVisibleResourceView const& rhs);
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
    GPUConstantBufferView(GPUConstantBufferView const& rhs);
    GPUConstantBufferView(GPUConstantBufferView&& rhs);

    GPUConstantBufferView& operator=(GPUConstantBufferView const& rhs);
    GPUConstantBufferView& operator=(GPUConstantBufferView&& rhs);
};

/////////////////////////////////////////////////////////////////////////////////

class GPUShaderResourceView : public GPUShaderVisibleResourceView
{
    GPUShaderResourceView();
    GPUShaderResourceView(std::size_t offsetInHeap, GPUResourceViewHeap const& parentHeap, GPUResourceDirectID resourceID, D3D12_RESOURCE_STATES targetState);
    GPUShaderResourceView(GPUShaderResourceView const& rhs);
    GPUShaderResourceView(GPUShaderResourceView&& rhs);

    GPUShaderResourceView& operator=(GPUShaderResourceView const& rhs);
    GPUShaderResourceView& operator=(GPUShaderResourceView&& rhs);
};

/////////////////////////////////////////////////////////////////////////////////

class GPUUnorderedAccessView : public GPUShaderVisibleResourceView
{
    GPUUnorderedAccessView();
    GPUUnorderedAccessView(std::size_t offsetInHeap, GPUResourceViewHeap const& parentHeap, GPUResourceDirectID resourceID, D3D12_RESOURCE_STATES targetState);
    GPUUnorderedAccessView(GPUUnorderedAccessView const& rhs);
    GPUUnorderedAccessView(GPUUnorderedAccessView&& rhs);

    GPUUnorderedAccessView& operator=(GPUUnorderedAccessView const& rhs);
    GPUUnorderedAccessView& operator=(GPUUnorderedAccessView&& rhs);
};

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

class GPUDepthStencilView : public GPUResourceView
{
    GPUDepthStencilView();
    GPUDepthStencilView(std::size_t offsetInHeap, GPUResourceViewHeap const& parentHeap, GPUResourceDirectID resourceID, D3D12_RESOURCE_STATES targetState);
    GPUDepthStencilView(GPUDepthStencilView const& rhs);
    GPUDepthStencilView(GPUDepthStencilView&& rhs);

    GPUDepthStencilView& operator=(GPUDepthStencilView const& rhs);
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
    GPURenderTargetView();
    GPURenderTargetView(GPUResourceViewDirectID viewID, GPUResourceViewHeap const& parentHeap);
    GPURenderTargetView(GPURenderTargetView const& rhs);
    GPURenderTargetView(GPURenderTargetView&& rhs);

    GPURenderTargetView& operator=(GPURenderTargetView const& rhs);
    GPURenderTargetView& operator=(GPURenderTargetView&& rhs);

    virtual D3D12_RESOURCE_STATES TargetState() const override;
};

/////////////////////////////////////////////////////////////////////////////////

class GPUSwapChainRenderTargetView : public GPURenderTargetView
{
    GPUSwapChainRenderTargetView();
    GPUSwapChainRenderTargetView(GPUSwapChainRenderTargetView const& rhs);
    GPUSwapChainRenderTargetView(GPUSwapChainRenderTargetView&& rhs);

    GPUSwapChainRenderTargetView& operator=(GPUSwapChainRenderTargetView const& rhs);
    GPUSwapChainRenderTargetView& operator=(GPUSwapChainRenderTargetView&& rhs);

    virtual D3D12_RESOURCE_STATES TargetState() const override;

protected:
    GPUResource renderBuffer_;
};

/////////////////////////////////////////////////////////////////////////////////

class GPUGenericRenderTargetView : public GPURenderTargetView
{
    GPUGenericRenderTargetView();
    GPUGenericRenderTargetView(std::size_t offsetInHeap, GPUResourceViewHeap const& parentHeap, GPUResourceDirectID resourceID);
    GPUGenericRenderTargetView(GPUGenericRenderTargetView const& rhs);
    GPUGenericRenderTargetView(GPUGenericRenderTargetView&& rhs);

    GPUGenericRenderTargetView& operator=(GPUGenericRenderTargetView const& rhs);
    GPUGenericRenderTargetView& operator=(GPUGenericRenderTargetView&& rhs);

    virtual D3D12_RESOURCE_STATES TargetState() const override;
protected:
    GPUResourceDirectID resourceID_;
};