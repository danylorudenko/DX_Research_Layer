#include <Rendering\GPUAccessProvider.hpp>

GPUAccessProvider::GPUAccessProvider(ID3D12Device* device) :
    renderingWorker_(device, GPU_WORKER_TYPE_DIRECT),
    copyWorker_(device, GPU_WORKER_TYPE_COPY),
    computeWorker_(device, GPU_WORKER_TYPE_COPY)
{

}

GPUAccessProvider::GPUAccessProvider(GPUAccessProvider&& rhs) :
    renderingWorker_(std::move(rhs.renderingWorker_)),
    copyWorker_(std::move(rhs.copyWorker_)),
    computeWorker_(std::move(rhs.computeWorker_))
{
    
}

GPUAccessProvider& GPUAccessProvider::operator=(GPUAccessProvider&& rhs)
{
    renderingWorker_ = std::move(rhs.renderingWorker_);
    copyWorker_ = std::move(rhs.copyWorker_);
    computeWorker_ = std::move(rhs.computeWorker_);
}

void GPUAccessProvider::CreateGPUBuffer(GPUResource* dest, std::size_t size)
{

}

void GPUAccessProvider::CreateGPUUploadHeap(GPUUploadHeap* dest, std::size_t size)
{

}

void GPUAccessProvider::ScheduleRender()
{

}

void GPUAccessProvider::TransitionGPUResource(GPUResource& resource, D3D12_RESOURCE_STATES state)
{

}

void GPUAccessProvider::UpdateGPUResource(GPUResource& dest, std::size_t offset, const void* data, std::size_t size)
{

}

void GPUAccessProvider::SetPipelineState()
{

}