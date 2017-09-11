#pragma once

#include <Utility\Window.hpp>

//
// WindowClass's implementation
//
namespace 
{
	LRESULT CALLBACK windowEventHandler(HWND handle, UINT message, WPARAM wparam, LPARAM lparam)
	{
		auto data = ::GetWindowLongPtr(handle, GWLP_USERDATA);
		auto window = reinterpret_cast<Window*>(data);

		window->handleEvents(message);

		return ::DefWindowProc(handle, message, wparam, lparam);
	}
}

WindowClass::WindowClass(HINSTANCE instance, std::wstring name) : name_{ std::move(name) }, successfully_{ false }
{
	WNDCLASS wc = {
		CS_VREDRAW | CS_HREDRAW,
		windowEventHandler,
		0, 0,
		instance,
		LoadIcon(nullptr, IDI_APPLICATION),
		LoadCursor(nullptr, IDC_ARROW),
		nullptr,
		nullptr,
		name_.c_str()
	};

	successfully_ = ::RegisterClass(&wc) != 0;
}

WindowClass::WindowClass() : name_{}, successfully_{ false } {}

WindowClass::~WindowClass()
{
	if (successfully_) {
		::UnregisterClass(name_.c_str(), ::GetModuleHandle(nullptr));
	}

	successfully_ = false;
}

WindowClass::WindowClass(WindowClass&& src) : name_{ std::move(src.name_) }, successfully_{ src.successfully_ }
{
	src.successfully_ = false;
}

WindowClass& WindowClass::operator=(WindowClass&& rhs)
{
	name_ = std::move(rhs.name_);
	successfully_ = rhs.successfully_;

	rhs.successfully_ = false;

	return *this;
}

WindowClass::operator bool() const 
{
	return successfully_;
}

std::wstring const& WindowClass::name() const
{
	return name_;
}

//
// Window's implementation
//

Window::Window(HINSTANCE instance, std::wstring title, std::uint32_t width, std::uint32_t height) : windowClass_{ instance, L"DX12WindowClass" }, handle_ { nullptr }
{
	if (windowClass_) {
		handle_ = ::CreateWindow(windowClass_.name().c_str(), title.c_str(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, nullptr, nullptr, instance, nullptr);

		if (handle_) {
			::SetWindowLongPtr(handle_, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

			::ShowWindow(handle_, SW_SHOWDEFAULT);
			::UpdateWindow(handle_);
		}
	}
}

Window::Window() = default;

Window::~Window()
{
	if (handle_) {
		DestroyWindow(handle_);
	}

	handle_ = nullptr;
}

Window::Window(Window&& src) : windowClass_{ std::move(src.windowClass_) }, handle_{ src.handle_ }
{
	src.handle_ = nullptr;
}

Window& Window::operator=(Window&& rhs)
{
	windowClass_ = std::move(rhs.windowClass_); 
	handle_ = rhs.handle_;

	rhs.handle_ = nullptr;

	return *this;
}

Window::operator bool() const
{
	return handle_ != nullptr;
}

Window::NativeHandle Window::nativeHandle() const
{
	return reinterpret_cast<NativeHandle>(handle_);
}

std::uint32_t Window::width() const
{
	auto const rect = frame();
	return rect.right - rect.left;
}

std::uint32_t Window::height() const
{
	auto const rect = frame();
	return rect.bottom - rect.top;
}

RECT Window::frame() const
{
	RECT rect{};
	GetWindowRect(handle_, &rect);
	return rect;
}

void Window::handleEvents(UINT message)
{
	switch (message) {
		case WM_CLOSE:
			PostQuitMessage(0);
			break;

		default:
			break;
	}
}
