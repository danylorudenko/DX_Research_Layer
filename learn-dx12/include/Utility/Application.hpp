#pragma once

#include <pch.hpp>

#include <Utility\Window.hpp>

class Application final
{
public:
	class Delegate
	{
	public:
		virtual ~Delegate() = default;

		virtual void start(Application& application) = 0;
		virtual void update(Application& application) = 0;
		virtual void shutdown(Application& application) = 0;
	};

public:
	explicit Application(HINSTANCE instance);
	~Application();

	Application(Application const&) = delete;
	Application(Application&&);

	Application& operator=(Application const&) = delete;
	Application& operator=(Application&&);

	int run();

	Window const& window() const;

	Delegate* const delegate() const;
	Application& delegate(Delegate& delegate);

private:
	Window window_;
	Delegate* delegate_;
	HINSTANCE process_;
};
