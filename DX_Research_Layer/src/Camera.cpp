#include <pch.hpp>

#include <Math\Camera.hpp>

namespace Math
{

Camera::Camera() = default;

Camera::Camera(float fow, float aspectRatio, float nearPlane, float farPlane) :
    fow_(fow),
    aspectRatio_(aspectRatio_),
    nearPlane_(nearPlane),
    farPlane_(farPlane)
{

}

Camera::Camera(Camera const&) = default;

Camera::Camera(Camera&&) = default;

Camera& Camera::operator=(Camera const&) = default;

Camera& Camera::operator=(Camera&&) = default;

DirectX::XMFLOAT4X4A Camera::ViewMatrix() const
{
    DirectX::XMFLOAT4X4A viewMatrix;
    DirectX::XMStoreFloat4x4A(&viewMatrix, ViewMatrixSIMD());
    return viewMatrix;
}

DirectX::XMMATRIX Camera::ViewMatrixSIMD() const
{
	//DirectX::XMFLOAT3A pos = transform_.Position();
	//DirectX::XMFLOAT4A rot = transform_.Rotation();
	//DirectX::XMFLOAT3A scl = transform_.Scale();
	//
	//DirectX::XMFLOAT3A forward = transform_.Forward();
	//DirectX::XMFLOAT3A up = transform_.Up();
	
	return DirectX::XMMatrixLookToLH(transform_.PositionSIMD(), transform_.ForwardSIMD(), transform_.UpSIMD());
}

DirectX::XMFLOAT4X4A const& Camera::PerspectiveMatrix()
{
    if (perspectiveMatrixDirty_) {
        DirectX::XMStoreFloat4x4A(&perspectiveMatrix_, PerspectiveMatrixSIMD());
        perspectiveMatrixDirty_ = false;
    }

    return perspectiveMatrix_;
}

DirectX::XMMATRIX Camera::PerspectiveMatrixSIMD() const
{
    return DirectX::XMMatrixPerspectiveFovLH(fow_, aspectRatio_, nearPlane_, farPlane_);
}

}