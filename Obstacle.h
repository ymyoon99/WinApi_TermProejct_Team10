#pragma once
#include <atlimage.h>

class Obstacle {
public:
    Obstacle(float x, float y);
    ~Obstacle();

    void Draw(HDC hdc, float offsetX, float offsetY);
    float GetX() const;
    float GetY() const;
    float GetWidth() const;  // 추가
    float GetHeight() const; // 추가

private:
    float x, y;
    CImage obstacleImage;
};
