#pragma once

#include <pch.hpp>

#include <Rendering\GPUWorker.hpp>
#include <Data\GPUResource.hpp>
#include <Data\GPUUploadHeap.hpp>

class GPUAccess
{
public:
    GPUAccess(ID3D12Device* device);
    GPUAccess(const ID3D12Device&) = delete;
    GPUAccess(GPUAccess&& rhs);

    GPUAccess& operator=(const GPUAccess&) = delete;
    GPUAccess& operator=(GPUAccess&&);

    void Begin();
    void End();

    ID3D12CommandList& Commit() { return Worker<GPU_WORKER_TYPE_DIRECT>().Commit(); }

    void CreateGPUUploadHeap(GPUUploadHeap** dest, void const* data, std::size_t elementSize, std::size_t elementsCount = 1, bool isConstBuffer = false);
    void CreateGPUBuffer(GPUResource** dest, std::size_t size);
    void UpdateGPUResource(GPUResource& dest, std::size_t offset, const void* userData, std::size_t size);
    D3D12_RESOURCE_STATES TransitionGPUResource(GPUResource& resource, D3D12_RESOURCE_STATES state, bool immediate = false);

    void SetVertexBuffer(D3D12_VERTEX_BUFFER_VIEW const* bufferView);
    void SetIndexBuffer(D3D12_INDEX_BUFFER_VIEW const* bufferView);

    void ScheduleRender(UINT vertexCount);
    void SheduleIndexedRender(UINT indexCount);

    static void CompileShader(LPWSTR fileName, ID3DBlob* dest, LPCSTR entryPoint, LPCSTR type);

    void CreatePSO(Microsoft::WRL::ComPtr<ID3D12PipelineState>& dest, D3D12_GRAPHICS_PIPELINE_STATE_DESC* desc);

    template<GPU_WORKER_TYPE TYPE = GPU_WROKER_TYPE_DIRECT>
    void SetPSO(ID3D12PipelineState* state) { Worker<TYPE>().Commit().SetPipelineState(state); }

    

private:
    template<GPU_WORKER_TYPE TYPE> GPUWorker& Worker();
    
    template<> constexpr GPUWorker& Worker<GPU_WORKER_TYPE_DIRECT>() { return renderingWorker_; }
    template<> constexpr GPUWorker& Worker<GPU_WORKER_TYPE_COPY>() { return copyWorker_; }
    template<> constexpr GPUWorker& Worker<GPU_WORKER_TYPE_COMPUTE>() { return computeWorker_; }

private:
    ID3D12Device* device_;

    GPUWorker renderingWorker_;
    GPUWorker copyWorker_;
    GPUWorker computeWorker_;
};