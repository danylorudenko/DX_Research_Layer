#pragma once

#include <pch.hpp>

class Camera
{
private:
    float fow_;
    float aspectRatio_;
    float nearPlane_;
    float farPlane_;
};