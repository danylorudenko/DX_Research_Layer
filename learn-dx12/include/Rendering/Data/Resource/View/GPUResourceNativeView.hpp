#pragma once

#include <pch.hpp>

#include <Rendering\Data\Resource\View\GPUResourceView.hpp>

class GPUResourceNativeView : public GPUResourceView
{
public:
    GPUResourceNativeView();
    GPUResourceNativeView(Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& heap, std::int32_t descriptorOffset, std::uint32_t descriptorSize, D3D12_RESOURCE_STATES targetState, GPUResource* describedResource);
    GPUResourceNativeView(GPUResourceNativeView const& rhs);
    GPUResourceNativeView(GPUResourceNativeView&& rhs);

    GPUResourceNativeView& operator=(GPUResourceNativeView const& rhs);
    GPUResourceNativeView& operator=(GPUResourceNativeView&& rhs);

    D3D12_GPU_DESCRIPTOR_HANDLE GPUHandle() const;
    D3D12_CPU_DESCRIPTOR_HANDLE CPUHandle() const;

    virtual bool IsNativeView() const override;

public:
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap_;
    std::int32_t descriptorOffset_;
    std::int64_t descriptorSize_;
};