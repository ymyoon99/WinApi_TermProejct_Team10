#pragma once

class Camera
{
public:
    Camera(float width, float height);
    ~Camera();

    void Update(float playerX, float playerY);
    float GetOffsetX() const;
    float GetOffsetY() const;

private:
    float width, height;
    float offsetX, offsetY;
};
