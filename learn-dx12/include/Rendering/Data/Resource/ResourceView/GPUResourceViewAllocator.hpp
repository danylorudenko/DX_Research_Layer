#pragma once

#include <pch.hpp>

#include <Rendering\GPUFoundation.hpp>

#include <Rendering\Data\Resource\ResourceView\GPUResourceViewHeap.hpp>
#include <Rendering\Data\Resource\ResourceView\GPUResourceView.hpp>
#include <Rendering\Data\Resource\GPUResourceID.hpp>

class GPUResourveViewAllocator
{
public:
    GPUResourveViewAllocator();
    GPUResourveViewAllocator(GPUFoundation const& foundation);
    GPUResourveViewAllocator(GPUResourveViewAllocator const&) = delete;
    GPUResourveViewAllocator(GPUResourveViewAllocator&& rhs);

    GPUResourveViewAllocator& operator=(GPUResourveViewAllocator const&) = delete;
    GPUResourveViewAllocator& operator=(GPUResourveViewAllocator&& rhs);

    GPUResourceViewDirectID AllocCBV();
    GPUResourceViewDirectID AllocSRV();
    GPUResourceViewDirectID AllocUAV();
    GPUResourceViewDirectID AllocRTV();
    GPUResourceViewDirectID AllocDSV();

private:
    void PushViewToList(GPUResourceView&& view);

    static constexpr std::size_t RTV_HEAP_CAPACITY = 3;
    static constexpr std::size_t DSV_HEAP_CAPACITY = 3;
    static constexpr std::size_t CBV_SRV_UAV_CAPACITY = 1;

    GPUFoundation const* foundation_ = nullptr;

    GPUResourceViewHeap rtvHeap_;
    GPUResourceViewHeap dsvHeap_;
    GPUResourceViewHeap cbvSrvUavHeap_;

    std::vector<std::unique_ptr<GPUResourceView>> resourceViewsList_;
};