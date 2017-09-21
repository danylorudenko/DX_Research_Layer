#pragma once

#include <pch.hpp>

class GPUFence
{
private:
    Microsoft::WRL::ComPtr<ID3D12Fence> fence_;

    UINT64 targetValue_;
};