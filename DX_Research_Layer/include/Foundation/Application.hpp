#pragma once

#include <pch.hpp>

#include <Foundation\Window.hpp>

class Application final
{
public:
	class Delegate
	{
	public:
        Delegate(Application& application) : application_{ &application } { }

        DXRL_DEFINE_UNCOPYABLE_MOVABLE_DEFAULT(Delegate)

		virtual ~Delegate() = default;

		virtual void start() = 0;
		virtual void update() = 0;
		virtual void shutdown() = 0;

        Application& MainApplication() { return *application_; }

    private:
        Application* application_;
	};

public:
	explicit Application(HINSTANCE instance);
	~Application();

	Application(Application const&) = delete;
	Application(Application&&);

	Application& operator=(Application const&) = delete;
	Application& operator=(Application&&);

	int run();

	Window& window();

	Delegate* const delegate() const;
	Application& delegate(Delegate& delegate);

private:
	Window window_;
	Delegate* delegate_;
	HINSTANCE process_;
};
