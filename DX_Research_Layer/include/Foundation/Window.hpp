#pragma once

#include <pch.hpp>

class WindowClass
{
public:
	WindowClass(HINSTANCE instance, std::wstring name);
	WindowClass();
	~WindowClass();

	WindowClass(WindowClass const&) = delete;
	WindowClass(WindowClass&&);

	WindowClass& operator=(WindowClass const&) = delete;
	WindowClass& operator=(WindowClass&&);

	operator bool() const;

	std::wstring const& name() const;

private:
	std::wstring name_;
	bool successfully_;
}; 

class Application;

class Window
{
public:
	using NativeHandle = HWND;
    using WinProcFunc = LRESULT (*)(HWND, UINT, WPARAM, LPARAM);

    class WinProcDelegate
    {
    public:
        WinProcDelegate();
        WinProcDelegate(Application::Delegate* appDelegate, Window::WinProcFunc winProcHandler) : appDelegate_{ appDelegate_ }, winProcHandler_{ winProcHandler } {}
        WinProcDelegate(WinProcDelegate&& rhs) = default;
        WinProcDelegate(WinProcDelegate const&) = delete;
        WinProcDelegate& operator=(WinProcDelegate&& rhs) = default;
        WinProcDelegate& operator=(WinProcDelegate const&) = delete;

        operator bool() const { return winProcHandler_ != null; }
        LRESULT operator()(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) { }

    private:
        Application::Delegate* appDelegate_;
        WinProcFunc winProcHandler_;
    };

public:
	Window(HINSTANCE instance, std::wstring title, std::uint32_t width, std::uint32_t height);
	Window();
	~Window();

	Window(Window const&) = delete;
	Window(Window&&);

	Window& operator=(Window const&) = delete;
	Window& operator=(Window&&);

	operator bool() const;

	NativeHandle nativeHandle() const;

	std::uint32_t width() const;
	std::uint32_t height() const;

	LRESULT handleEvents(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
    void winProcHandler(WinProcDelegate&& winProcDelegate);

private:
	RECT frame() const;

private:
	WindowClass windowClass_;
    WinProcDelegate winProcDelegate_;
	HWND handle_;
};
