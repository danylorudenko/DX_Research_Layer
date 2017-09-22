#pragma once

#include <pch.hpp>

#include <Rendering\GPUWorker.hpp>
#include <Data\GPUResource.hpp>
#include <Data\GPUUploadHeap.hpp>

class GPUAccessProvider
{
public:
    GPUAccessProvider(ID3D12Device* device);
    GPUAccessProvider(const ID3D12Device&) = delete;
    GPUAccessProvider(GPUAccessProvider&& rhs);

    GPUAccessProvider& operator=(const GPUAccessProvider&) = delete;
    GPUAccessProvider& operator=(GPUAccessProvider&&);

    void CreateGPUBuffer(GPUResource* dest, std::size_t size);
    void UpdateGPUResource(GPUResource& dest, std::size_t offset, const void* userData, std::size_t size);
    void CreateGPUUploadHeap(GPUUploadHeap* dest, std::size_t size);

    void SetPipelineState(/*state for main rendering pipeline*/);

    void TransitionGPUResource(GPUResource& resource, D3D12_RESOURCE_STATES state);
    
    void ScheduleRender(/*what to render*/);


private:
    template<GPU_WORKER_TYPE TYPE> GPUWorker& Worker();
    template<> GPUWorker& Worker<GPU_WORKER_TYPE_DIRECT>() { return renderingWorker_; }
    template<> GPUWorker& Worker<GPU_WORKER_TYPE_COPY>() { return copyWorker_; }
    template<> GPUWorker& Worker<GPU_WORKER_TYPE_COMPUTE>() { return computeWorker_; }

private:
    GPUWorker renderingWorker_;
    GPUWorker copyWorker_;
    GPUWorker computeWorker_;
};