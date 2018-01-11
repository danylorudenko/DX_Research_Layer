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
    GPUResourceView(GPUResourceViewDirectID viewID, GPUResourceViewHeap const& parentHeap);
    GPUResourceView(GPUResourceView const& rhs);
    GPUResourceView(GPUResourceView&& rhs);

    GPUResourceView& operator=(GPUResourceView const& rhs);
    GPUResourceView& operator=(GPUResourceView&& rhs);

    virtual D3D12_RESOURCE_STATES TargetState() const = 0;
    D3D12_CPU_DESCRIPTOR_HANDLE CPUHandle();

protected:
    GPUResourceViewDirectID viewID_;
    GPUResourceViewHeap const* parentHeap_ = nullptr;
};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

class GPUShaderVisibleResourceView : public GPUResourceView
{
public:
    GPUShaderVisibleResourceView();
    GPUShaderVisibleResourceView(GPUResourceViewDirectID viewID, GPUResourceViewHeap const& parentHeap, GPUResourceDirectID resourceID, D3D12_RESOURCE_STATES targetState);
    GPUShaderVisibleResourceView(GPUShaderVisibleResourceView const& rhs);
    GPUShaderVisibleResourceView(GPUShaderVisibleResourceView&& rhs);

    GPUShaderVisibleResourceView& operator=(GPUShaderVisibleResourceView const& rhs);
    GPUShaderVisibleResourceView& operator=(GPUShaderVisibleResourceView&& rhs);

    virtual D3D12_RESOURCE_STATES TargetState() const override;
    D3D12_GPU_DESCRIPTOR_HANDLE GPUHandle() const;

    virtual void Bind(GPUEngine& executionEngine) const = 0;

protected:
    GPUResourceDirectID resourceID_;
    D3D12_RESOURCE_STATES targetState_ = (D3D12_RESOURCE_STATES)0;
};

/////////////////////////////////////////////////////////////////////////////////

class GPUConstantBufferView : public GPUShaderVisibleResourceView
{
public:
    GPUConstantBufferView();
    GPUConstantBufferView(GPUResourceViewDirectID viewID, GPUResourceViewHeap const& parentHeap, GPUResourceDirectID resourceID, D3D12_RESOURCE_STATES targetState);
    GPUConstantBufferView(GPUConstantBufferView const& rhs);
    GPUConstantBufferView(GPUConstantBufferView&& rhs);

    GPUConstantBufferView& operator=(GPUConstantBufferView const& rhs);
    GPUConstantBufferView& operator=(GPUConstantBufferView&& rhs);
    
    virtual D3D12_RESOURCE_STATES TargetState() const override;
    virtual void Bind(GPUEngine& executionEngine) const override;
};

/////////////////////////////////////////////////////////////////////////////////

class GPShaderResourceView : public GPUShaderVisibleResourceView
{
    GPShaderResourceView();
    GPShaderResourceView(GPUResourceViewDirectID viewID, GPUResourceViewHeap const& parentHeap, GPUResourceDirectID resourceID, D3D12_RESOURCE_STATES targetState);
    GPShaderResourceView(GPShaderResourceView const& rhs);
    GPShaderResourceView(GPShaderResourceView&& rhs);

    GPShaderResourceView& operator=(GPShaderResourceView const& rhs);
    GPShaderResourceView& operator=(GPShaderResourceView&& rhs);

    virtual D3D12_RESOURCE_STATES TargetState() const override;
    virtual void Bind(GPUEngine& executionEngine) const override;
};

/////////////////////////////////////////////////////////////////////////////////

class GPUUnorderedAccessView : public GPUShaderVisibleResourceView
{
    GPUUnorderedAccessView();
    GPUUnorderedAccessView(GPUUnorderedAccessView const& rhs);
    GPUUnorderedAccessView(GPUUnorderedAccessView&& rhs);

    GPUUnorderedAccessView& operator=(GPUUnorderedAccessView const& rhs);
    GPUUnorderedAccessView& operator=(GPUUnorderedAccessView&& rhs);

    virtual D3D12_RESOURCE_STATES TargetState() const override;
    virtual void Bind(GPUEngine& executionEngine) const override;
};

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

class GPUDepthStencilView : public GPUResourceView
{
    GPUDepthStencilView();
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

};