#include "Enemy.h"
#include <cmath>

#define PlayerWidth 20.0f
#define PlayerHeight 25.0f

Enemy::Enemy(float x, float y, float speed, int health, const wchar_t* imagePaths[], int numFrames, float animationSpeed)
    : x(x), y(y), speed(speed), health(health), boundWidth(0), boundHeight(0), currentFrame(0), numFrames(numFrames), frameTimeAccumulator(0.0f), animationSpeed(animationSpeed) {
    enemyImages = new CImage[numFrames];
    for (int i = 0; i < numFrames; ++i) {
        enemyImages[i].Load(imagePaths[i]);
    }
}

Enemy::~Enemy() {
    delete[] enemyImages;
}

void Enemy::Update(float frameTime, float playerX, float playerY, const std::vector<Obstacle*>& obstacles) {
    float dx = playerX - x;
    float dy = playerY - y;
    float distance = sqrt(dx * dx + dy * dy);

    float directionX = dx / distance;
    float directionY = dy / distance;

    float newX = x + directionX * speed * frameTime;
    float newY = y + directionY * speed * frameTime;

    if (!CheckCollision(newX, newY, obstacles)) {
        x = newX;
        y = newY;
    }

    frameTimeAccumulator += frameTime;
    if (frameTimeAccumulator >= animationSpeed) {
        currentFrame = (currentFrame + 1) % numFrames;
        frameTimeAccumulator = 0.0f;
    }
}

bool Enemy::CheckCollision(float newX, float newY, const std::vector<Obstacle*>& obstacles) const {
    for (const auto& obstacle : obstacles) {
        float ox = obstacle->GetX();
        float oy = obstacle->GetY();
        float ow = obstacle->GetWidth();
        float oh = obstacle->GetHeight();

        if (newX < ox + ow &&
            newX + PlayerWidth > ox &&
            newY < oy + oh &&
            newY + PlayerHeight > oy) {
            return true;
        }
    }
    return false;
}

void Enemy::Draw(HDC hdc, float offsetX, float offsetY) {
    enemyImages[currentFrame].Draw(hdc, static_cast<int>(x - offsetX), static_cast<int>(y - offsetY));
}

float Enemy::GetX() const {
    return x;
}

float Enemy::GetY() const {
    return y;
}

void Enemy::TakeDamage(int damage) {
    health -= damage;
    if (health < 0) {
        health = 0;
    }
}

bool Enemy::IsDead() const {
    return health <= 0;
}