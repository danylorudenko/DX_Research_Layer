#include <Math\Camera.hpp>

Camera::Camera(float fow, float aspectRatio, float nearPlane, float farPlane) :
    viewMatrix_{},
    fow_(fow),
    aspectRatio_(aspectRatio_),
    nearPlane_(nearPlane),
    farPlane_(farPlane)
{

}

Camera::Camera(const Camera&) = default;

Camera::Camera(Camera&&) = default;

Camera& Camera::operator=(const Camera&) = default;

Camera& Camera::operator=(Camera&&) = default;