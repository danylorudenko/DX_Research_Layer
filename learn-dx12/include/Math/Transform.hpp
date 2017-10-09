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

    DirectX::XMVECTOR Position() const;
    DirectX::XMVECTOR Orientation() const;
    DirectX::XMVECTOR Scale() const;

    void Position(DirectX::FXMVECTOR newPos);
    void Orientation(DirectX::FXMVECTOR newOrientation);
    void Scale(DirectX::FXMVECTOR newScale);

    void AddPosition(DirectX::FXMVECTOR newPos);
    void AddOrientation(DirectX::FXMVECTOR newOrientation);
    void AddScale(DirectX::FXMVECTOR newScale);

private:
    DirectX::XMFLOAT3A position_;
    DirectX::XMFLOAT4A orientation_; // quaternion
    DirectX::XMFLOAT3A scale_;
};