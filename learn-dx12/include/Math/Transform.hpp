#pragma once

#include <pch.hpp>

class Transform
{
public:
    Transform();
    Transform(const Transform& rhs);
    Transform(Transform&& rhs);
    
    Transform& operator=(const Transform& rhs);
    Transform& operator=(Transform&& rhs);

    DirectX::XMVECTOR XM_CALLCONV PositionSIMD() const;
    DirectX::XMVECTOR XM_CALLCONV RotationSIMD() const;
    DirectX::XMVECTOR XM_CALLCONV ScaleSIMD() const;

    DirectX::XMFLOAT3A Position() const;
    DirectX::XMFLOAT4A Rotation() const;
    DirectX::XMFLOAT3A Scale() const;

    void XM_CALLCONV Position(DirectX::FXMVECTOR newPos);
    void XM_CALLCONV Rotation(DirectX::FXMVECTOR newOrientation);
    void XM_CALLCONV RotationRollPitchYaw(DirectX::FXMVECTOR eulerAngles);
    void XM_CALLCONV Scale(DirectX::FXMVECTOR newScale);

    void Position(const DirectX::XMFLOAT3A& pos);
    void Rotation(const DirectX::XMFLOAT4A& orientation);
    void RotationRollPitchYaw(const DirectX::XMFLOAT3A& eulerAngles);
    void Scale(const DirectX::XMFLOAT3A& scale);

    DirectX::XMFLOAT3A Forward() const;
    DirectX::XMFLOAT3A Right() const;
    DirectX::XMFLOAT3A Up() const;

    DirectX::XMVECTOR XM_CALLCONV ForwardSIMD() const;
    DirectX::XMVECTOR XM_CALLCONV RightSIMD() const;
    DirectX::XMVECTOR XM_CALLCONV UpSIMD() const;

    DirectX::XMFLOAT4X4A WorldMatrix();
    DirectX::XMMATRIX XM_CALLCONV WorldMatrixSIMD() const;

private:
    DirectX::XMFLOAT3A position_;
    DirectX::XMFLOAT4A rotation_; // quaternion
    DirectX::XMFLOAT3A scale_;

    DirectX::XMFLOAT4X4A worldMatrix_;
    bool worldMatrixDirty_ = true;
};