#pragma once

#include <pch.hpp>

#include <Foundation\Application.hpp>
#include <Foundation\PerformanceTimer.hpp>
#include <Rendering\GPUFoundation.hpp>
#include <Rendering\Data\SceneConstantBuffer.hpp>
#include <Rendering\FrameGraph\GPUGraphicsGraphNode.hpp>
#include <Rendering\FrameGraph\GPUPresentGraphNode.hpp>
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

class Direct3DAppDelegate : public Application::Delegate
{
public:
    Direct3DAppDelegate(Application& application);

    virtual void start() override;
    virtual void update() override;
    virtual void shutdown() override;

    PerformanceTimer& Timer();
    void DisplayFrameTime(Application& application, float drawTime);

    void MainUpdate(std::size_t frameIndex);

public:
    
    Window const* mainWindow;
    DirectWinProcDelegate winProcDelegate_;

    std::unique_ptr<DXRL::GPUDelegate> gpuDelegate_;

    PerformanceTimer gameTimer_;
    UINT64 mainUpdateIterator_;

    std::wstring windowText_;
};