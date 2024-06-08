#pragma once
#include <atlimage.h>
#include <vector>
#include "Obstacle.h"

class Enemy {
public:
    Enemy(float x, float y, float speed, int health, float animationSpeed = 0.2f, float eWidth = 50.0f, float eHeight = 50.0f);
    virtual ~Enemy();

    void Update(float frameTime, float playerX, float playerY, const std::vector<Obstacle*>& obstacles);
    void Draw(HDC hdc, float offsetX, float offsetY);

    void TakeDamage(int damage);
    bool IsDead() const;

    float GetX() const;
    float GetY() const;
    float GetWidth() const;
    float GetHeight() const;

    void SetBounds(float width, float height);

protected:
    virtual void LoadImages(); // virtual to allow override

    float x, y;
    float eWidth, eHeight;
    float speed;
    int health;
    float boundWidth, boundHeight;

    int currentFrame;
    float frameTimeAccumulator;
    float animationSpeed;

    std::vector<CImage> idleImages;

    bool CheckCollision(float newX, float newY, const std::vector<Obstacle*>& obstacles) const;
};

// BrainMonster
class BrainMonster : public Enemy {
public:
    BrainMonster(float x, float y, float speed, int health = 10, float eWidth = 27.0f, float eHeight = 36.0f);
protected:
    void LoadImages() override; // Override LoadImages for specific images
};

// EyeMonster
class EyeMonster : public Enemy {
public:
    EyeMonster(float x, float y, float speed, int health = 10, float eWidth = 37.0f, float eHeight = 29.0f);
protected:
    void LoadImages() override; // Override LoadImages for specific images
};

// BigBoomer
class BigBoomer : public Enemy {
public:
    BigBoomer(float x, float y, float speed, int health = 20, float eWidth = 45.0f, float eHeight = 51.0f);
protected:
    void LoadImages() override; // Override LoadImages for specific images
};

// Lamprey
class Lamprey : public Enemy {
public:
    Lamprey(float x, float y, float speed, int health = 50, float eWidth = 50.0f, float eHeight = 50.0f);
protected:
    void LoadImages() override; // Override LoadImages for specific images
};

// Yog
class Yog : public Enemy {
public:
    Yog(float x, float y, float speed, int health = 50, float eWidth = 64.0f, float eHeight = 54.0f);
protected:
    void LoadImages() override; // Override LoadImages for specific images
};
