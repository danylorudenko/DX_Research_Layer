#pragma once

#include <pch.hpp>

struct SceneConstantBuffer
{
    DirectX::XMFLOAT4X4A perspectiveMatrix_;
    DirectX::XMFLOAT4X4A viewMatrix_;
    DirectX::XMFLOAT4A cameraPosition_roghness_ = DirectX::XMFLOAT4A{ 0.0f, 0.0f, 0.0f, 0.1f };
	float metalness = 0.02f;
};