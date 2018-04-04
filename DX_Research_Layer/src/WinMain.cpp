#include <pch.hpp>

#include <Foundation\Direct3DAppDelegate.hpp>
#include <Foundation\Application.hpp>

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ PSTR pCmdLine, _In_ int nShowCmd)
{
    Application app{ hInstance };

    Application::Delegate* myDelegate = new Direct3DAppDelegate{ app };
    app.delegate(*myDelegate);

    app.run();
    
	delete myDelegate;

    return 0;
}