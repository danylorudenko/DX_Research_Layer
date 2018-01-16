#pragma once

#include <pch.hpp>

#include <Rendering\GPUFoundation.hpp>

#include <Rendering\Data\Resource\ResourceView\GPUResourceViewHeap.hpp>
#include <Rendering\Data\Resource\ResourceView\GPUResourceView.hpp>
#include <Rendering\Data\Resource\GPUResourceID.hpp>

class GPUResourceViewAllocator
{
public:
    GPUResourceViewAllocator();
    GPUResourceViewAllocator(GPUFoundation& foundation);
    GPUResourceViewAllocator(GPUResourceViewAllocator const&) = delete;
    GPUResourceViewAllocator(GPUResourceViewAllocator&& rhs);

    GPUResourceViewAllocator& operator=(GPUResourceViewAllocator const&) = delete;
    GPUResourceViewAllocator& operator=(GPUResourceViewAllocator&& rhs);

    GPUResourceViewDirectID AllocCBV(GPUResourceDirectID resourceID, D3D12_CONSTANT_BUFFER_VIEW_DESC const& desc, D3D12_RESOURCE_STATES targetState);
    GPUResourceViewDirectID AllocSRV(GPUResourceDirectID resourceID, D3D12_SHADER_RESOURCE_VIEW_DESC const& desc, D3D12_RESOURCE_STATES targetState);
    GPUResourceViewDirectID AllocUAV(GPUResourceDirectID resourceID, D3D12_UNORDERED_ACCESS_VIEW_DESC const& desc, D3D12_RESOURCE_STATES targetState);
    GPUResourceViewDirectID AllocSwapChainRTV(GPUResource& resource, D3D12_RENDER_TARGET_VIEW_DESC const& desc);
    GPUResourceViewDirectID AllocRTV(GPUResourceDirectID resourceID, D3D12_RENDER_TARGET_VIEW_DESC const& desc);
    GPUResourceViewDirectID AllocDSV(GPUResourceDirectID resourceID, D3D12_DEPTH_STENCIL_VIEW_DESC const& desc, D3D12_RESOURCE_STATES targetState);

private:
    
    template<typename TView>
    GPUResourceViewDirectID RegisterResourceView(TView&& view)
    {
        resourceViewsList_.emplace_back(std::make_unique<TView>(std::move(view)));
        return GPUResourceViewDirectID{ resourceViewsList_.size() - 1 };
    }


    static constexpr std::size_t RTV_HEAP_CAPACITY = 3;
    static constexpr std::size_t DSV_HEAP_CAPACITY = 3;
    static constexpr std::size_t CBV_SRV_UAV_CAPACITY = 1;

    GPUFoundation* foundation_ = nullptr;

    GPUResourceViewHeap rtvHeap_;
    GPUResourceViewHeap dsvHeap_;
    GPUResourceViewHeap cbvSrvUavHeap_;

    std::vector<std::unique_ptr<GPUResourceView>> resourceViewsList_;
};