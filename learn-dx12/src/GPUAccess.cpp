#include <Rendering\GPUAccess.hpp>

GPUAccess::GPUAccess(ID3D12Device* device) :
    device_(device),
    renderingWorker_(device, GPU_WORKER_TYPE_DIRECT),
    copyWorker_(device, GPU_WORKER_TYPE_COPY),
    computeWorker_(device, GPU_WORKER_TYPE_COPY)
{

}

GPUAccess::GPUAccess(GPUAccess&& rhs) :
    renderingWorker_(std::move(rhs.renderingWorker_)),
    copyWorker_(std::move(rhs.copyWorker_)),
    computeWorker_(std::move(rhs.computeWorker_))
{
    
}

GPUAccess& GPUAccess::operator=(GPUAccess&& rhs)
{
    renderingWorker_ = std::move(rhs.renderingWorker_);
    copyWorker_ = std::move(rhs.copyWorker_);
    computeWorker_ = std::move(rhs.computeWorker_);

    return *this;
}

void GPUAccess::Begin()
{
    Worker<GPU_WORKER_TYPE_DIRECT>().Reset();
    Worker<GPU_WORKER_TYPE_COPY>().Reset();
    Worker<GPU_WORKER_TYPE_COMPUTE>().Reset();
}

void GPUAccess::End()
{
    Worker<GPU_WORKER_TYPE_DIRECT>().Finalize();
    Worker<GPU_WORKER_TYPE_COPY>().Finalize();
    Worker<GPU_WORKER_TYPE_COMPUTE>().Finalize();
}

void GPUAccess::CreateGPUBuffer(GPUResource** dest, std::size_t size)
{
    *dest = new GPUResource{ device_, static_cast<UINT64>(size) };
}

void GPUAccess::CreateGPUUploadHeap(GPUUploadHeap** dest, void const* data, std::size_t elementSize, std::size_t elementsCount, bool isConstBuffer)
{
    *dest = new GPUUploadHeap{ device_, data, elementSize, elementsCount, isConstBuffer };
}

void GPUAccess::ScheduleRender(UINT vertexCount)
{
    Worker<GPU_WORKER_TYPE_DIRECT>().Commit().DrawInstanced(vertexCount, 1, 0, 0);
}

void GPUAccess::SheduleIndexedRender(UINT indexCount)
{
    Worker<GPU_WORKER_TYPE_DIRECT>().Commit().DrawIndexedInstanced(indexCount, 0, 0, 0, 0);
}

void GPUAccess::CompileShader(LPWSTR fileName, ID3DBlob * dest, LPCSTR entryPoint, LPCSTR type)
{
#if defined (_DEBUG) | (DEBUG)
    UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
    UINT compileFlags = 0;
#endif

    ThrowIfFailed(D3DCompileFromFile(fileName, nullptr, nullptr, entryPoint, type, compileFlags, 0, &dest, nullptr));
}

void GPUAccess::CreatePSO(Microsoft::WRL::ComPtr<ID3D12PipelineState>& dest, D3D12_GRAPHICS_PIPELINE_STATE_DESC* desc)
{
    ThrowIfFailed(device_->CreateGraphicsPipelineState(desc, IID_PPV_ARGS(&dest)));
}

D3D12_RESOURCE_STATES GPUAccess::TransitionGPUResource(GPUResource& resource, D3D12_RESOURCE_STATES state, bool immediate)
{
    auto prevState = resource.SetTargetState(state);
    Worker<GPU_WORKER_TYPE_COPY>().Commit().ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(resource.Get(), prevState, resource.TargetState()));

    if (immediate) {
        Worker<GPU_WORKER_TYPE_COPY>().Finalize();
        Worker<GPU_WORKER_TYPE_COPY>().Reset();
    }

    return prevState;
}

void GPUAccess::UpdateGPUResource(GPUResource& dest, std::size_t offset, const void* data, std::size_t size)
{
    assert(size + offset <= dest.Size());

    const D3D12_RANGE mapRange = { offset, offset + size };
    void* bufferPtr = nullptr;
    dest.Map(&bufferPtr, mapRange);

    memcpy(bufferPtr, data, size);
    dest.Unmap(mapRange);
}

void GPUAccess::SetVertexBuffer(D3D12_VERTEX_BUFFER_VIEW const* bufferView)
{
    Worker<GPU_WORKER_TYPE_DIRECT>().Commit().IASetVertexBuffers(0, 1, bufferView);
}

void GPUAccess::SetIndexBuffer(D3D12_INDEX_BUFFER_VIEW const* bufferView)
{
    Worker<GPU_WORKER_TYPE_DIRECT>().Commit().IASetIndexBuffer(bufferView);
}

