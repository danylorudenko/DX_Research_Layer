#include <pch.hpp>

#include <Utility\DirectAppDelegate.hpp>
#include <Utility\Application.hpp>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR pCmdLine, int nShowCmd)
{
    Application::Delegate* myDelegate = new DirectAppDelegate{};
    Application app{ hInstance };
    app.delegate(*myDelegate);

    app.run();
    
    return 0;
}