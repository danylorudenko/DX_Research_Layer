#include <Foundation\Application.hpp>
#include <Memory/Memory.hpp>

Application::Application(HINSTANCE instance) : window_{ instance, L"DirectX Sample", 800, 600 }, delegate_{}, process_{ instance } {}

Application::~Application()
{
	process_ = nullptr;
}

Application::Application(Application&& src) : window_{ std::move(src.window_) }, delegate_{ std::move(src.delegate_) }, process_{ src.process_ }
{
	src.process_ = nullptr;
}

Application& Application::operator=(Application&& rhs)
{
	window_ = std::move(rhs.window_);
	delegate_ = std::move(rhs.delegate_);
	process_ = rhs.process_;

	rhs.process_ = nullptr;

	return *this;
}

int Application::run()
{
	assert(delegate_);

	delegate_->start();

	MSG message{};
	message.message = window_ ? WM_NULL : WM_QUIT;

	while (message.message != WM_QUIT) {
		if (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE)) {
			TranslateMessage(&message);
            DispatchMessage(&message);
		} else {
			delegate_->update();
		}
	}

	delegate_->shutdown();

	return 0;
}

Window& Application::window()
{
	return window_;
}

Application::Delegate* const Application::delegate() const
{
	return delegate_;
}

Application& Application::delegate(Application::Delegate& delegate)
{
	delegate_ = &delegate;
	return *this;
}