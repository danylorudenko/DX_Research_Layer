#include <Math\Transform.hpp>

using namespace DirectX;

Transform::Transform() :
    position_(0.0f, 0.0f, 0.0f),
    rotation_(1.0f, 0.0f, 0.0f, 0.0f),
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

DirectX::XMVECTOR Transform::RotationSIMD() const
{
    return DirectX::XMLoadFloat4A(&rotation_);
}

DirectX::XMVECTOR Transform::ScaleSIMD() const
{
    return DirectX::XMLoadFloat3A(&scale_);
}

DirectX::XMFLOAT3A Transform::Position() const
{
    return position_;
}

DirectX::XMFLOAT4A Transform::Rotation() const
{
    return rotation_;
}

DirectX::XMFLOAT3A Transform::Scale() const
{
    return scale_;
}

void Transform::PositionSIMD(DirectX::FXMVECTOR src)
{
    DirectX::XMStoreFloat3A(&position_, src);
}

void Transform::RotationSIMD(DirectX::FXMVECTOR src)
{
    DirectX::XMStoreFloat4A(&rotation_, src);
}

void Transform::ScaleSIMD(DirectX::FXMVECTOR src)
{
    DirectX::XMStoreFloat3A(&scale_, src);
}

void Transform::Position(DirectX::XMFLOAT3A pos)
{
    position_ = pos;
}

void Transform::Rotation(DirectX::XMFLOAT4A orientation)
{
    rotation_ = orientation;
}

void Transform::Rotation(DirectX::FXMVECTOR orientation)
{
    DirectX::XMVECTOR quat = DirectX::XMQuaternionRotationRollPitchYawFromVector((orientation));
    DirectX::XMStoreFloat4A(&rotation_, quat);
}

void Transform::RotationRollPitchYaw(DirectX::XMFLOAT3A orientation)
{
    DirectX::XMVECTOR quat = DirectX::XMQuaternionRotationRollPitchYawFromVector(DirectX::XMLoadFloat3A(&orientation));
    DirectX::XMStoreFloat4A(&rotation_, quat);
}

void Transform::Scale(DirectX::XMFLOAT3A scale)
{
    scale_ = scale;
}

DirectX::XMFLOAT3A Transform::Forward() const
{
    DirectX::XMFLOAT3A forward{};
    DirectX::XMStoreFloat3A(&forward, ForwardSIMD());
    return forward;
}

DirectX::XMVECTOR Transform::ForwardSIMD() const
{
    DirectX::XMVECTOR unitForward = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
    return DirectX::XMVector4Transform(unitForward, DirectX::XMMatrixRotationQuaternion(RotationSIMD()));
}

DirectX::XMFLOAT3A Transform::Right() const
{
    DirectX::XMFLOAT3A right{};
    DirectX::XMStoreFloat3A(&right, RightSIMD());
    return right;
}

DirectX::XMVECTOR Transform::RightSIMD() const
{
    DirectX::XMVECTOR unitRight = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
    return DirectX::XMVector4Transform(unitRight, DirectX::XMMatrixRotationQuaternion(RotationSIMD()));
}

DirectX::XMFLOAT3A Transform::Up() const
{
    DirectX::XMFLOAT3A up{};
    DirectX::XMStoreFloat3A(&up, UpSIMD());
    return up;
}

DirectX::XMVECTOR Transform::UpSIMD() const
{
    DirectX::XMVECTOR unitUp = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    return DirectX::XMVector2Transform(unitUp, DirectX::XMMatrixRotationQuaternion(RotationSIMD()));
}

DirectX::XMMATRIX Transform::WorldMatrix() const
{
    return DirectX::XMMatrixAffineTransformation(ScaleSIMD(), DirectX::XMVectorReplicate(0.0f), RotationSIMD(), PositionSIMD());
}