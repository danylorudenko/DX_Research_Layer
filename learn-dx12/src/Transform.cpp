#include <Math\Transform.hpp>

using namespace DirectX;

Transform::Transform() :
    position_(0.0f, 0.0f, 0.0f),
    orientation_(1.0f, 0.0f, 0.0f, 0.0f),
    scale_(1.0f, 1.0f, 1.0f)
{ }
