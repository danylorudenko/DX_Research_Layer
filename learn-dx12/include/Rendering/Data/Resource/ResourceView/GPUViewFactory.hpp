#pragma once

#include <pch.hpp>

class GPUViewFactory
{
public:
    GPUViewFactory();
    GPUViewFactory(GPUViewFactory const&) = delete;
    GPUViewFactory(GPUViewFactory&& rhs);

    GPUViewFactory operator=(GPUViewFactory const&) = delete;
    GPUViewFactory operator=(GPUViewFactory&& rhs);

    
};