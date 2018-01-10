#pragma once

#include <pch.hpp>

#include <Rendering\GPUEngine.hpp>


class GPUResourceView
{
public:
    GPUResourceView();
    GPUResourceView(GPUResourceView const& rhs);
    GPUResourceView(GPUResourceView&& rhs);

    GPUResourceView& operator=(GPUResourceView const& rhs);
    GPUResourceView& operator=(GPUResourceView&& rhs);

    virtual void Bind(GPUEngine& executionEngine) = 0;
};