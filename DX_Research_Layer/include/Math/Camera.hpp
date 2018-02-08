#pragma once

#include <pch.hpp>
#include <Math\Transform.hpp>

namespace Math
{

class Camera
{
public:
    Camera();
    Camera(float fow, float aspectRatio, float nearPlane, float farPlane);
    Camera(Camera const&);
    Camera(Camera&&);

    Camera& operator=(Camera const&);
    Camera& operator=(Camera&&);

    float Fow() const { return fow_ * RAD_TO_DEGREE; }
    float AspectRatio() const { return aspectRatio_; }
    float NearPlane() const { return nearPlane_; }
    float FarPlane() const { return farPlane_; }

    void Fow(float fow) { fow_ = fow * DEGREE_TO_RAD; perspectiveMatrixDirty_ = true; }
    void NearPlane(float nearPlane) { nearPlane_ = nearPlane; perspectiveMatrixDirty_ = true; }
    void FarPlane(float farPlane) { farPlane_ = farPlane; perspectiveMatrixDirty_ = true; }
    void AspectRatio(float aspect) { aspectRatio_ = aspect; perspectiveMatrixDirty_ = true; }

    DirectX::XMFLOAT4X4A ViewMatrix(DirectX::XMFLOAT3A& pos, DirectX::XMFLOAT3A& direction, DirectX::XMFLOAT3A& up) const;
    DirectX::XMFLOAT4X4A XM_CALLCONV ViewMatrix(DirectX::FXMVECTOR pos, DirectX::FXMVECTOR direction, DirectX::FXMVECTOR up) const;

    DirectX::XMMATRIX XM_CALLCONV ViewMatrixSIMD(DirectX::XMFLOAT3A& pos, DirectX::XMFLOAT3A& direction, DirectX::XMFLOAT3A& up) const;
    DirectX::XMMATRIX XM_CALLCONV ViewMatrixSIMD(DirectX::FXMVECTOR pos, DirectX::FXMVECTOR direction, DirectX::FXMVECTOR up) const;

    DirectX::XMFLOAT4X4A const& PerspectiveMatrix();
    DirectX::XMMATRIX XM_CALLCONV PerspectiveMatrixSIMD() const;

private:
    DirectX::XMFLOAT4X4A perspectiveMatrix_;
    bool perspectiveMatrixDirty_ = true;

    float fow_;
    float aspectRatio_;
    float nearPlane_;
    float farPlane_;
};

}