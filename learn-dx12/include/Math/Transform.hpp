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
    DirectX::XMVECTOR XM_CALLCONV OrientationSIMD() const;
    DirectX::XMVECTOR XM_CALLCONV ScaleSIMD() const;

    DirectX::XMFLOAT3A Position() const;
    DirectX::XMFLOAT4A Orientation() const;
    DirectX::XMFLOAT3A Scale() const;

    void XM_CALLCONV PositionSIMD(DirectX::FXMVECTOR newPos);
    void XM_CALLCONV OrientationSIMD(DirectX::FXMVECTOR newOrientation);
    void XM_CALLCONV ScaleSIMD(DirectX::FXMVECTOR newScale);

    void Position(DirectX::XMFLOAT3A pos);
    void Orientation(DirectX::XMFLOAT4A orientation);
    void OrientationRollPitchYaw(DirectX::XMFLOAT3A orientation);
    void Scale(DirectX::XMFLOAT3A scale);

    DirectX::XMMATRIX XM_CALLCONV WorldMatrix() const;

private:
    DirectX::XMFLOAT3A position_;
    DirectX::XMFLOAT4A orientation_; // quaternion
    DirectX::XMFLOAT3A scale_;
};