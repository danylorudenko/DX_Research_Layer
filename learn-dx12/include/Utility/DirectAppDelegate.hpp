#pragma once

#include <pch.hpp>

#include <Utility\Application.hpp>
#include <Utility\PerformanceTimer.hpp>
#include <Rendering\GPUFoundation.hpp>
#include <Rendering\Data\GeometryMesh.hpp>
#include <Rendering\Data\GPUUploadHeap.hpp>
#include <Rendering\Data\SceneConstantBuffer.hpp>
#include <Rendering\Data\FrameGraph\GPUGraphicsGraphNode.hpp>
#include <Rendering\Data\FrameGraph\GPUPresentGraphNode.hpp>

#include <DXProgrammableCapture.h>

class DirectAppDelegate : public Application::Delegate
{
public:
    virtual void start(Application& application) override;
    virtual void update(Application& application) override;
    virtual void shutdown(Application& application) override;

    PerformanceTimer& Timer();
    void DisplayFrameTime(Application& application, float drawTime);

    Microsoft::WRL::ComPtr<ID3D12RootSignature> CreateRootSignature();
    Microsoft::WRL::ComPtr<ID3D12PipelineState> CreatePipelineState(Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature);

    void Draw(int frameIndex);
    void CustomAction(int frameIndex);

private:
    // FOUNDATION DATA
    PerformanceTimer gameTimer_;
    std::wstring windowText_;
    GPUFoundation gpuAccess_;

    UINT64 frameIndex_ = 0;


    // CUSTOM APPLICATION DATA
    GPUUploadHeap constantBuffer_;
    GPUFrameResourceDescriptor constantBufferView_;
    SceneConstantBuffer constantBufferData_;

    GPURootSignature triangleRootSignature_;
    GPUPipelineState trianglePipelineState_;

    GPUGraphicsGraphNode triangleGraphNode_;
    GPUPresentGraphNode presentNode_;

    GPUFrameResource triangleMesh_;
    GeometryMesh triangleMeshData_;

    Microsoft::WRL::ComPtr<IDXGraphicsAnalysis> graphicsAnalysis_;
};