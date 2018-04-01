#include <pch.hpp>

#include <Foundation\DirectAppDelegate.hpp>
#include <Foundation\Application.hpp>

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ PSTR pCmdLine, _In_ int nShowCmd)
{
    Application::Delegate* myDelegate = new Direct3DAppDelegate{};
    Application app{ hInstance };
    app.delegate(*myDelegate);

    app.run();
    
	delete myDelegate;

    return 0;
}