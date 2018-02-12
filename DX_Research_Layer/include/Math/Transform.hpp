#pragma once

#include <pch.hpp>

namespace Math
{

class Transform
{
public:
    Transform();
    Transform(Transform const& rhs);
    Transform(Transform&& rhs);

    Transform& operator=(Transform const& rhs);
    Transform& operator=(Transform&& rhs);

    DirectX::XMVECTOR XM_CALLCONV PositionSIMD() const;
    DirectX::XMVECTOR XM_CALLCONV RotationSIMD() const;
    DirectX::XMVECTOR XM_CALLCONV ScaleSIMD() const;

    DirectX::XMFLOAT3A const& Position() const;
    DirectX::XMFLOAT4A const& Rotation() const;
    DirectX::XMFLOAT3A const& Scale() const;

    void XM_CALLCONV Position(DirectX::FXMVECTOR newPos);
    void XM_CALLCONV Rotation(DirectX::FXMVECTOR newOrientation);
    void XM_CALLCONV RotationRollPitchYaw(DirectX::FXMVECTOR eulerAngles);
    void XM_CALLCONV Scale(DirectX::FXMVECTOR newScale);

    void Position(DirectX::XMFLOAT3A const& pos);
    void Rotation(DirectX::XMFLOAT4A const& orientation);
    void RotationRollPitchYaw(DirectX::XMFLOAT3A const& eulerAngles);
    void Scale(DirectX::XMFLOAT3A const& scale);
    void Scale(float scale);

    DirectX::XMFLOAT3A Forward() const;
    DirectX::XMFLOAT3A Right() const;
    DirectX::XMFLOAT3A Up() const;

    DirectX::XMVECTOR XM_CALLCONV ForwardSIMD() const;
    DirectX::XMVECTOR XM_CALLCONV RightSIMD() const;
    DirectX::XMVECTOR XM_CALLCONV UpSIMD() const;

    DirectX::XMFLOAT4X4A const& WorldMatrix();
    DirectX::XMMATRIX XM_CALLCONV WorldMatrixSIMD() const;

private:
    DirectX::XMFLOAT3A position_;
    DirectX::XMFLOAT4A rotation_; // quaternion
    DirectX::XMFLOAT3A scale_;

    DirectX::XMFLOAT4X4A worldMatrix_;
    bool worldMatrixDirty_ = true;
};

}