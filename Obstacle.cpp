#include "Obstacle.h"

Obstacle::Obstacle(float x, float y) : x(x), y(y) {
    obstacleImage.Load(L"./resources/ui/HeartAnimation_0.png");
}

Obstacle::~Obstacle() {
}

void Obstacle::Draw(HDC hdc, float offsetX, float offsetY) {
    obstacleImage.Draw(hdc, static_cast<int>(x - offsetX), static_cast<int>(y - offsetY));
}

float Obstacle::GetX() const {
    return x;
}

float Obstacle::GetY() const {
    return y;
}

float Obstacle::GetWidth() const {  // 추가
    return static_cast<float>(obstacleImage.GetWidth());
}

float Obstacle::GetHeight() const { // 추가
    return static_cast<float>(obstacleImage.GetHeight());
}
