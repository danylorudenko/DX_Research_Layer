#pragma once

#include <pch.hpp>

class GPUCommandList
{
private:
    Microsoft::WRL::ComPtr<ID3D12CommandList> commandList_;
};