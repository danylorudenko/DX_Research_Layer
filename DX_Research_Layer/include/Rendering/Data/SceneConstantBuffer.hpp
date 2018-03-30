#pragma once

#include <pch.hpp>

struct SceneConstantBuffer
{
    DirectX::XMFLOAT4X4A perspectiveMatrix_;
    DirectX::XMFLOAT4X4A viewMatrix_;
    DirectX::XMFLOAT4A cameraPosition = DirectX::XMFLOAT4A{ 0.0f, 0.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT4A lightDir = DirectX::XMFLOAT4A{ 1.0f, 1.0f, 1.0f, 0.0f };
};