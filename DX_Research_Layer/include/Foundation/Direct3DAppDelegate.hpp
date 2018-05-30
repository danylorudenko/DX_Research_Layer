#pragma once

#include <Foundation\Macro.hpp>
#include <Foundation\Types.hpp>
#include <Foundation\Application.hpp>
#include <Foundation\PerformanceTimer.hpp>
#include <Rendering\GPUFoundation.hpp>

class DirectWinProcDelegate : public WinProcDelegate
{
public:
    DXRL_DEFINE_UNCOPYABLE_MOVABLE_DEFAULT(DirectWinProcDelegate)

    DirectWinProcDelegate(void* appDelegate) : WinProcDelegate{ appDelegate } {}

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

	DXRL::U32_t keyMap_ = 0; // WASD UJIK

    DXRL::S32_t prevMouseX_;
    DXRL::S32_t prevMouseY_;
};

class Direct3DAppDelegate : public Application::Delegate
{
public:
    Direct3DAppDelegate(Application& application);

    virtual void start() override;
    virtual void update() override;
    virtual void shutdown() override;

    DXRL::PerformanceTimer& Timer();
    void DisplayFrameTime(Application& application, DXRL::Single drawTime);

    void MainUpdate(DXRL::U64_t frameIndex);

public:
    
    Window const* mainWindow;
    DirectWinProcDelegate winProcDelegate_;
    DXRL::WideString windowText_;

    DXRL::Memory::UniquePtr<DXRL::GPUDelegate> gpuDelegate_;

    DXRL::PerformanceTimer gameTimer_;
    DXRL::U64_t mainUpdateIterator_;

};