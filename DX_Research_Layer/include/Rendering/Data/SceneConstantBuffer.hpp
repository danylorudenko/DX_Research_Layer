#pragma once

#include <pch.hpp>

struct SceneConstantBuffer
{
    DirectX::XMFLOAT4X4A perspectiveMatrix_;
    DirectX::XMFLOAT4X4A viewMatrix_;
    DirectX::XMFLOAT3A cameraPosition_ = DirectX::XMFLOAT3A{ 0.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT3A lightDirection_ = DirectX::XMFLOAT3A{ 1.0f, 0.0f, 0.0f };
};