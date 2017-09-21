#pragma once

#include <pch.hpp>

#include <Rendering\GPUCommandList.hpp>

class GPUAccessProvider
{
private:
    GPUCommandList renderingCommandList_;
    GPUCommandList copyCommandList_;
    GPUCommandList computeCommandList_;
};