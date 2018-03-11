#pragma once

#include <pch.hpp>

#include <Foundation\Application.hpp>
#include <Foundation\PerformanceTimer.hpp>
#include <Rendering\GPUFoundation.hpp>
#include <Rendering\Data\SceneConstantBuffer.hpp>
#include <Rendering\Data\FrameGraph\GPUGraphicsGraphNode.hpp>
#include <Rendering\Data\FrameGraph\GPUPresentGraphNode.hpp>
#include <Math\Camera.hpp>

class DirectWinProcDelegate : public WinProcDelegate
{
public:
    DirectWinProcDelegate() = default;
    DirectWinProcDelegate(void* appDelegate) : WinProcDelegate{ appDelegate } {}
    DirectWinProcDelegate(DirectWinProcDelegate&&) = default;
    DirectWinProcDelegate& operator=(DirectWinProcDelegate&&) = default;

    virtual LRESULT operator()(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) override;

	bool WPressed() const;
	bool APressed() const;
	bool SPressed() const;
	bool DPressed() const;

	bool UPressed() const;
	bool JPressed() const;
	bool IPressed() const;
	bool KPressed() const;

    bool rotationOn = false;

	float mouseXDelta_;
	float mouseYDelta_;

	std::uint32_t keyMap_ = 0; // WASD UJIK

    int prevMouseX_;
    int prevMouseY_;
};

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


    // FOUNDATION DATA
    PerformanceTimer gameTimer_;
    std::wstring windowText_;
    std::unique_ptr<DXRL::GPUFoundation> gpuFoundation_;
    Microsoft::WRL::ComPtr<IDXGraphicsAnalysis> graphicsAnalysis_;

    UINT64 frameIndex_ = 0;


    // CUSTOM APPLICATION DATA
    DirectWinProcDelegate winProcDelegate_;

    Math::Transform* mainObjTransform = nullptr;
    Math::Camera camera_;
	float cameraTargetPitch_ = 0.0f;
	float cameraTargetYaw_ = 0.0f;
    SceneConstantBuffer sceneBufferData_;
    DXRL::GPUResourceViewHandle sceneBuffer_;
};