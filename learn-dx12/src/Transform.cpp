#include <Math\Transform.hpp>

using namespace DirectX;

Transform::Transform() :
    position_(0.0f, 0.0f, 0.0f),
    orientation_(1.0f, 0.0f, 0.0f, 0.0f),
    scale_(1.0f, 1.0f, 1.0f)
{ }

Transform::Transform(const Transform& rhs) = default;

Transform::Transform(Transform&& rhs) = default;

Transform& Transform::operator=(const Transform& rhs) = default;

Transform& Transform::operator=(Transform&& rhs) = default;

DirectX::XMVECTOR Transform::PositionSIMD() const
{
    return DirectX::XMLoadFloat3A(&position_);
}

DirectX::XMVECTOR Transform::OrientationSIMD() const
{
    return DirectX::XMLoadFloat4A(&orientation_);
}

DirectX::XMVECTOR Transform::ScaleSIMD() const
{
    return DirectX::XMLoadFloat3A(&scale_);
}

DirectX::XMFLOAT3A Transform::Position() const
{
    return position_;
}

DirectX::XMFLOAT4A Transform::Orientation() const
{
    return orientation_;
}

DirectX::XMFLOAT3A Transform::Scale() const
{
    return scale_;
}

void Transform::PositionSIMD(DirectX::FXMVECTOR src)
{
    DirectX::XMStoreFloat3A(&position_, src);
}

void Transform::OrientationSIMD(DirectX::FXMVECTOR src)
{
    DirectX::XMStoreFloat4A(&orientation_, src);
}

void Transform::ScaleSIMD(DirectX::FXMVECTOR src)
{
    DirectX::XMStoreFloat3A(&scale_, src);
}

void Transform::Position(DirectX::XMFLOAT3A pos)
{
    position_ = pos;
}

void Transform::Orientation(DirectX::XMFLOAT4A orientation)
{
    orientation_ = orientation;
}

void Transform::OrientationRollPitchYaw(DirectX::XMFLOAT3A orientation)
{
    DirectX::XMVECTOR quat = DirectX::XMQuaternionRotationRollPitchYawFromVector(DirectX::XMLoadFloat3A(&orientation));
    DirectX::XMStoreFloat4A(&orientation_, quat);
}

void Transform::Scale(DirectX::XMFLOAT3A scale)
{
    scale_ = scale;
}

DirectX::XMMATRIX Transform::WorldMatrix() const
{
    DirectX::XMMATRIX wMat = DirectX::XMMatrixScalingFromVector(ScaleSIMD());
    DirectX::XMMatrixRotationQuaternion(OrientationSIMD());
    asdhewgdcauwefdnawe
    
}

