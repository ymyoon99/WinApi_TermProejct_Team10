#pragma once
#include <atlimage.h>
#include <vector>
#include "Obstacle.h"
#include "Player.h"

class Enemy {
public:
    Enemy(float x, float y, float speed, int health, const wchar_t* imagePaths[], int numFrames, float animationSpeed = 0.2f);
    ~Enemy();

    void Update(float frameTime, float playerX, float playerY, const std::vector<Obstacle*>& obstacles);
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
    float animationSpeed;
    CImage* enemyImages;

    bool CheckCollision(float newX, float newY, const std::vector<Obstacle*>& obstacles) const;
};
