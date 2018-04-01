#pragma once

#include <pch.hpp>

#include <Rendering\Resource\ResourceView\GPUResourceView.hpp>
#include <Rendering\Resource\ResourceView\GPUResourceViewHeap.hpp>
#include <Rendering\Resource\GPUResourceHandle.hpp>
#include <Rendering\Resource\ResourceView\GPUResourceViewTable.hpp>

namespace DXRL
{

class GPUFoundation;

class GPUResourceViewAllocator
{
public:
    GPUResourceViewAllocator();
    GPUResourceViewAllocator(GPUFoundation& foundation);
    GPUResourceViewAllocator(GPUResourceViewAllocator const&) = delete;
    GPUResourceViewAllocator(GPUResourceViewAllocator&& rhs);

    GPUResourceViewAllocator& operator=(GPUResourceViewAllocator const&) = delete;
    GPUResourceViewAllocator& operator=(GPUResourceViewAllocator&& rhs);

    GPUResourceViewDirectHandle AllocCBV(GPUResourceHandle const& resourceHandle, D3D12_CONSTANT_BUFFER_VIEW_DESC const& desc, D3D12_RESOURCE_STATES targetState);
    GPUResourceViewDirectHandle AllocSRV(GPUResourceHandle const& resourceHandle, D3D12_SHADER_RESOURCE_VIEW_DESC const& desc, D3D12_RESOURCE_STATES targetState);
    GPUResourceViewDirectHandle AllocUAV(GPUResourceHandle const& resourceHandle, D3D12_UNORDERED_ACCESS_VIEW_DESC const& desc, D3D12_RESOURCE_STATES targetState);
    GPUResourceViewDirectHandle AllocRTV(GPUResourceHandle const& resourceHandle, D3D12_RENDER_TARGET_VIEW_DESC const& desc);
    GPUResourceViewDirectHandle AllocSwapChainRTV(GPUResource& resource, D3D12_RENDER_TARGET_VIEW_DESC const& desc);
    GPUResourceViewDirectHandle AllocDSV(GPUResourceHandle const& resourceHandle, D3D12_DEPTH_STENCIL_VIEW_DESC const& desc, D3D12_RESOURCE_STATES targetState);

    void SetDefaultHeaps(GPUEngine& executionEngine);

    GPUResourceView& AccessView(GPUResourceViewDirectHandle handle);

private:

    template<typename TView>
    GPUResourceViewDirectHandle RegisterResourceView(TView&& view)
    {
        resourceViewsList_.emplace_back(std::make_unique<TView>(std::move(view)));
        return GPUResourceViewDirectHandle{ resourceViewsList_.size() - 1 };
    }


    static constexpr std::size_t RTV_HEAP_CAPACITY = 6;
    static constexpr std::size_t DSV_HEAP_CAPACITY = 6;
    static constexpr std::size_t CBV_SRV_UAV_CAPACITY = 30;

    GPUFoundation* foundation_ = nullptr;

    GPUResourceViewHeap rtvHeap_;
    GPUResourceViewHeap dsvHeap_;
    GPUResourceViewHeap cbvSrvUavHeap_;

    std::vector<std::unique_ptr<GPUResourceView>> resourceViewsList_;
};

}