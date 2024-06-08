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
<<<<<<< Updated upstream
=======

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


class BossYog : public Enemy {
public:
    BossYog(float x, float y, float speed, int health = 1000, float eWidth = 256.0f, float eHeight = 216.0f);
    void Update(float frameTime, float playerX, float playerY, const std::vector<Obstacle*>& obstacles);
    void Draw(HDC hdc, float offsetX, float offsetY);
    void LoadImages();

private:
    bool isPreparing;
    bool isCharging;
    float prepareTime;
    float prepareDuration;
    float chargeSpeed;
    float chargeCooldown;
    float chargeTimer;
    float chargeDistance;
    std::vector<CImage> prepareImages;
    int prepareFrame;
    float prepareFrameTime;
    float prepareFrameDuration;
};

>>>>>>> Stashed changes
