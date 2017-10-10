#pragma once

#include <pch.hpp>

class Camera
{
public:
    Camera(float fow, float aspectRatio, float nearPlane, float farPlane);
    Camera(const Camera&);
    Camera(Camera&&);

    Camera& operator=(const Camera&);
    Camera& operator=(Camera&&);

    float Fow() const { return fow_; }
    float AspectRation() const { return aspectRatio_; }
    float NearPlane() const { return nearPlane_; }
    float FarPlane() const { return farPlane_; }

    DirectX::XMFLOAT4X4A ViewMatrix() const;
    DirectX::XMMATRIX XM_CALLCONV ViewMatrixSIMD() const;

private:
    DirectX::XMFLOAT4X4A viewMatrix_;

    float fow_;
    float aspectRatio_;
    float nearPlane_;
    float farPlane_;
};