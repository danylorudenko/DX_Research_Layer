#pragma once

#include <pch.hpp>

#include <Foundation\Window.hpp>

class Application final
{
public:
	class Delegate
	{
	public:
        DXRL_DEFINE_UNCOPYABLE_MOVABLE_DEFAULT(Delegate)

        explicit Delegate(Application& application) : application_{ &application } { }
		virtual ~Delegate() = default;

		virtual void start() = 0;
		virtual void update() = 0;
		virtual void shutdown() = 0;

        Application& MainApplication() { return *application_; }

    private:
        Application* application_;
	};

public:
	DXRL_DEFINE_UNCOPYABLE_MOVABLE(Application)
    
    explicit Application(HINSTANCE instance);
    ~Application();

	int run();

	Window& window();

	Delegate* const delegate() const;
	Application& delegate(Delegate& delegate);

private:
	Window window_;
	Delegate* delegate_;
	HINSTANCE process_;
};
