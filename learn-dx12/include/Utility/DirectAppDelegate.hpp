#pragma once

#include <pch.hpp>

#include <Utility\Application.hpp>
#include <Utility\PerformanceTimer.hpp>
#include <Data\FrameResource.hpp>
#include <Rendering\GPUAccess.hpp>

class DirectAppDelegate : public Application::Delegate
{
public:
    virtual void start(Application& application) override;
    virtual void update(Application& application) override;
    virtual void shutdown(Application& application) override;

    //ID3D12Device& Device();
    PerformanceTimer& Timer();
    void DisplayFrameTime(Application& application, float drawTime);

    void CreateRootSignature();
    void CreatePipelineState();

    void LoadTriangleVertices(ID3D12GraphicsCommandList* startupCommandList);
    void CreateConstantBufferDescriptorHeap();
    void LoadConstantBuffers();
    
    //void Draw();
    void CustomAction();

private:
    PerformanceTimer gameTimer_;
    std::unique_ptr<GPUAccess> gpuAccess_ = nullptr;

    Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState_;

    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> cbvHeap_;

    Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_;


    // Additional data and resources.

    GPUUploadHeap* constantBuffer_ = nullptr;
    SceneConstantBuffer constantBufferData_;
    UINT8* constantBufferMappedData_;

    GPUUploadHeap* triangleVertices_ = nullptr;
    D3D12_VERTEX_BUFFER_VIEW triangleVerticesView_;

};