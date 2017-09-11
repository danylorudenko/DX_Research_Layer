#pragma once

#include <string>
#include <functional>
#include <Windows.h>

enum class Event
{
	create,
	close
};

using WindowAction = std::function<void(Event)>;

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

class Window
{
public:
	using NativeHandle = void*;

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

	void handleEvents(UINT message);

private:
	RECT frame() const;

private:
	WindowClass windowClass_;
	HWND handle_;
};
