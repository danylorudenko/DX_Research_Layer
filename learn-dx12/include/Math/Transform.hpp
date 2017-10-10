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

    void XM_CALLCONV PositionSIMD(DirectX::FXMVECTOR newPos);
    void XM_CALLCONV RotationSIMD(DirectX::FXMVECTOR newOrientation);
    void XM_CALLCONV ScaleSIMD(DirectX::FXMVECTOR newScale);

    void Position(DirectX::XMFLOAT3A pos);
    void Rotation(DirectX::XMFLOAT4A orientation);
    void XM_CALLCONV Rotation(DirectX::FXMVECTOR orientation);
    void RotationRollPitchYaw(DirectX::XMFLOAT3A orientation);
    void Scale(DirectX::XMFLOAT3A scale);

    DirectX::XMFLOAT3A Forward() const;
    DirectX::XMFLOAT3A Right() const;
    DirectX::XMFLOAT3A Up() const;

    DirectX::XMVECTOR XM_CALLCONV ForwardSIMD() const;
    DirectX::XMVECTOR XM_CALLCONV RightSIMD() const;
    DirectX::XMVECTOR XM_CALLCONV UpSIMD() const;

    DirectX::XMMATRIX XM_CALLCONV WorldMatrix() const;

private:
    DirectX::XMFLOAT3A position_;
    DirectX::XMFLOAT4A rotation_; // quaternion
    DirectX::XMFLOAT3A scale_;
};