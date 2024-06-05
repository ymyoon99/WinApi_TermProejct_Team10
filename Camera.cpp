#include "Camera.h"

Camera::Camera(float width, float height) : width(width), height(height), offsetX(0), offsetY(0)
{
}

Camera::~Camera()
{
}

void Camera::Update(float playerX, float playerY)
{
    offsetX = playerX - width / 2;
    offsetY = playerY - height / 2;
}

float Camera::GetOffsetX() const
{
    return offsetX;
}

float Camera::GetOffsetY() const
{
    return offsetY;
}
