#include <pch.hpp>

#include <Utility\DirectAppDelegate.hpp>
#include <Utility\Application.hpp>

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ PSTR pCmdLine, _In_ int nShowCmd)
{
    Application::Delegate* myDelegate = new DirectAppDelegate{};
    Application app{ hInstance };
    app.delegate(*myDelegate);

    app.run();
    
    return 0;
}