#pragma once

#include <pch.hpp>

#include <Foundation\Application.hpp>
#include <Foundation\PerformanceTimer.hpp>
#include <Rendering\GPUFoundation.hpp>
#include <Rendering\Data\SceneConstantBuffer.hpp>
#include <Rendering\Data\FrameGraph\GPUGraphicsGraphNode.hpp>
#include <Rendering\Data\FrameGraph\GPUPresentGraphNode.hpp>
#include <Math\Camera.hpp>

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

    void Draw(std::size_t frameIndex);
    void CustomAction(std::size_t frameIndex);

private:
    // FOUNDATION DATA
    PerformanceTimer gameTimer_;
    std::wstring windowText_;
    std::unique_ptr<DXRL::GPUFoundation> gpuFoundation_;
    Microsoft::WRL::ComPtr<IDXGraphicsAnalysis> graphicsAnalysis_;

    UINT64 frameIndex_ = 0;


    // CUSTOM APPLICATION DATA

    Camera camera_;
    SceneConstantBuffer sceneBufferData_;
    DXRL::GPUResourceViewHandle sceneBuffer_;
};