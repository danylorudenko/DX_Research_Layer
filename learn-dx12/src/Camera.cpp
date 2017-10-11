#include <Math\Camera.hpp>

Camera::Camera(float fow, float aspectRatio, float nearPlane, float farPlane) :
    fow_(fow),
    aspectRatio_(aspectRatio_),
    nearPlane_(nearPlane),
    farPlane_(farPlane)
{

}

Camera::Camera(const Camera&) = default;

Camera::Camera(Camera&&) = default;

Camera& Camera::operator=(const Camera&) = default;

Camera& Camera::operator=(Camera&&) = default;

DirectX::XMFLOAT4X4A Camera::ViewMatrix(DirectX::XMFLOAT3A& pos, DirectX::XMFLOAT3A& direction, DirectX::XMFLOAT3A& up) const
{
    DirectX::XMFLOAT4X4A viewMatrix;
    DirectX::XMStoreFloat4x4A(&viewMatrix, ViewMatrixSIMD(DirectX::XMLoadFloat3A(&pos), DirectX::XMLoadFloat3A(&direction), DirectX::XMLoadFloat3A(&up)));
    return viewMatrix;
}

DirectX::XMFLOAT4X4A Camera::ViewMatrix(DirectX::FXMVECTOR pos, DirectX::FXMVECTOR direction, DirectX::FXMVECTOR up) const
{
    DirectX::XMFLOAT4X4A viewMatrix;
    DirectX::XMStoreFloat4x4A(&viewMatrix, ViewMatrixSIMD(pos, direction, up));

    return viewMatrix;
}

DirectX::XMMATRIX Camera::ViewMatrixSIMD(DirectX::FXMVECTOR pos, DirectX::FXMVECTOR direction, DirectX::FXMVECTOR up) const
{
    return DirectX::XMMatrixLookToLH(pos, direction, up);
}

DirectX::XMMATRIX Camera::ViewMatrixSIMD(DirectX::XMFLOAT3A& pos, DirectX::XMFLOAT3A& direction, DirectX::XMFLOAT3A& up) const
{
    return ViewMatrixSIMD(DirectX::XMLoadFloat3A(&pos), DirectX::XMLoadFloat3A(&direction), DirectX::XMLoadFloat3A(&up));
}

DirectX::XMFLOAT4X4A Camera::PerspectiveMatrix()
{
    if (perspectiveMatrixDirty_) {
        DirectX::XMStoreFloat4x4A(&perspectiveMatrix_, PerspectiveMatrixSIMD());
        perspectiveMatrixDirty_ = false;
    }

    return perspectiveMatrix_;
}

DirectX::XMMATRIX Camera::PerspectiveMatrixSIMD() const
{
    return DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PI / 180 * fow_, aspectRatio_, nearPlane_, farPlane_);
}