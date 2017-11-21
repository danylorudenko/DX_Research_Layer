#pragma once

#include <pch.hpp>

#include <Utility\Application.hpp>
#include <Utility\PerformanceTimer.hpp>
#include <Rendering\GPUAccess.hpp>
#include <Rendering\Data\GeometryMesh.hpp>
#include <Rendering\Data\GPUUploadHeap.hpp>
#include <Rendering\Data\SceneConstantBuffer.hpp>

class DirectAppDelegate : public Application::Delegate
{
public:
    virtual void start(Application& application) override;
    virtual void update(Application& application) override;
    virtual void shutdown(Application& application) override;

    //ID3D12Device& Device();
    PerformanceTimer& Timer();
    void DisplayFrameTime(Application& application, float drawTime);

    Microsoft::WRL::ComPtr<ID3D12RootSignature> CreateRootSignature();
    Microsoft::WRL::ComPtr<ID3D12PipelineState> CreatePipelineState(Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature);

    void LoadTriangleVertices();
    void LoadConstantBuffers();
    
    void Draw();
    void CustomAction();

private:
    PerformanceTimer gameTimer_;
    std::wstring windowText_;
    GPUAccess gpuAccess_;

    GPUUploadHeap constantBuffer_;
    GPUFrameResourceDescriptor constantBufferView_;
    SceneConstantBuffer constantBufferData_;

    GeometryMesh triangleMesh_;
};