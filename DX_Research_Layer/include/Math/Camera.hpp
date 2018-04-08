#pragma once

#include <Foundation\Constant.hpp>
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

    float Fow() const { return fow_ * DXRL::RAD_TO_DEGREE; }
    float AspectRatio() const { return aspectRatio_; }
    float NearPlane() const { return nearPlane_; }
    float FarPlane() const { return farPlane_; }

	Transform& Transform() { return transform_; }
	Math::Transform const& Transform() const { return transform_; }

    void Fow(float fow) { fow_ = fow * DXRL::DEGREE_TO_RAD; perspectiveMatrixDirty_ = true; }
    void NearPlane(float nearPlane) { nearPlane_ = nearPlane; perspectiveMatrixDirty_ = true; }
    void FarPlane(float farPlane) { farPlane_ = farPlane; perspectiveMatrixDirty_ = true; }
    void AspectRatio(float aspect) { aspectRatio_ = aspect; perspectiveMatrixDirty_ = true; }

    DirectX::XMFLOAT4X4A ViewMatrix() const;
    DirectX::XMMATRIX XM_CALLCONV ViewMatrixSIMD() const;

    DirectX::XMFLOAT4X4A const& PerspectiveMatrix();
    DirectX::XMMATRIX XM_CALLCONV PerspectiveMatrixSIMD() const;

private:
	Math::Transform transform_;
	DirectX::XMFLOAT4X4A perspectiveMatrix_;
    bool perspectiveMatrixDirty_ = true;

    float fow_;
    float aspectRatio_;
    float nearPlane_;
    float farPlane_;
};

}