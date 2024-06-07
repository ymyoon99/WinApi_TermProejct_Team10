#pragma once
#include <atlimage.h>

class Enemy {
public:
    Enemy(float x, float y, float speed, int health, const wchar_t* imagePaths[], int numFrames, float animationSpeed = 0.2f);
    ~Enemy();

    void Update(float frameTime, float playerX, float playerY);
    void Draw(HDC hdc, float offsetX, float offsetY);

    void TakeDamage(int damage);
    bool IsDead() const;

    float GetX() const;
    float GetY() const;

    void SetBounds(float width, float height);

private:
    float x, y;
    float speed;
    int health;
    float boundWidth, boundHeight;

    int currentFrame;
    int numFrames;
    float frameTimeAccumulator;
    float animationSpeed; // 애니메이션 속도
    CImage* enemyImages;
};
