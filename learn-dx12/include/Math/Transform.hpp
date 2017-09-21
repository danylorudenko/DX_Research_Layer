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


private:
    DirectX::XMFLOAT4 orientation_;
    DirectX::XMFLOAT3 position_;
    DirectX::XMFLOAT3 scale_;
};