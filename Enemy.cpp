#include "Enemy.h"
#include <cmath>

#define EnemyWidth 50.0f
#define EnemyHeight 50.0f

Enemy::Enemy(float x, float y, float speed, int health, float animationSpeed, float eWidth, float eHeight)
    : x(x), y(y), speed(speed), health(health), boundWidth(0), boundHeight(0), currentFrame(0), frameTimeAccumulator(0.0f), animationSpeed(animationSpeed), eWidth(eWidth), eHeight(eHeight) {
    LoadImages();
}

Enemy::~Enemy() {
}

void Enemy::LoadImages() {
    idleImages.resize(5);
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
        currentFrame = (currentFrame + 1) % idleImages.size();
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
            newX + eWidth > ox &&
            newY < oy + oh &&
            newY + eHeight > oy) {
            return true;
        }
    }
    return false;
}

void Enemy::Draw(HDC hdc, float offsetX, float offsetY) {
    if (!idleImages[currentFrame].IsNull()) {
        idleImages[currentFrame].Draw(hdc, static_cast<int>(x - offsetX), static_cast<int>(y - offsetY));
    }
}

float Enemy::GetX() const {
    return x;
}

float Enemy::GetY() const {
    return y;
}

float Enemy::GetWidth() const {
    return eWidth;
}

float Enemy::GetHeight() const {
    return eHeight;
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

// BrainMonster
BrainMonster::BrainMonster(float x, float y, float speed, int health, float eWidth, float eHeight)
    : Enemy(x, y, speed, health, 1.0f, eWidth, eHeight) {
    LoadImages();
}

void BrainMonster::LoadImages() {
    idleImages.resize(4);
    idleImages[0].Load(L"./resources/enemy/BrainMonster_0.png");
    idleImages[1].Load(L"./resources/enemy/BrainMonster_1.png");
    idleImages[2].Load(L"./resources/enemy/BrainMonster_2.png");
    idleImages[3].Load(L"./resources/enemy/BrainMonster_3.png");
}

// EyeMonster
EyeMonster::EyeMonster(float x, float y, float speed, int health, float eWidth, float eHeight)
    : Enemy(x, y, speed, health, 1.0f, eWidth, eHeight) {
    LoadImages();
}

void EyeMonster::LoadImages() {
    idleImages.resize(3);
    idleImages[0].Load(L"./resources/enemy/EyeMonster_0.png");
    idleImages[1].Load(L"./resources/enemy/EyeMonster_1.png");
    idleImages[2].Load(L"./resources/enemy/EyeMonster_2.png");
}

// BigBoomer
BigBoomer::BigBoomer(float x, float y, float speed, int health, float eWidth, float eHeight)
    : Enemy(x, y, speed, health, 1.0f, eWidth, eHeight) {
    LoadImages();
}

void BigBoomer::LoadImages() {
    idleImages.resize(4);
    idleImages[0].Load(L"./resources/enemy/BigBoomer_0.png");
    idleImages[1].Load(L"./resources/enemy/BigBoomer_1.png");
    idleImages[2].Load(L"./resources/enemy/BigBoomer_2.png");
    idleImages[3].Load(L"./resources/enemy/BigBoomer_3.png");
}

// Lamprey
Lamprey::Lamprey(float x, float y, float speed, int health, float eWidth, float eHeight)
    : Enemy(x, y, speed, health, 1.0f, eWidth, eHeight) {
    LoadImages();
}

void Lamprey::LoadImages() {
    idleImages.resize(5);
    idleImages[0].Load(L"./resources/enemy/T_Lamprey_0.png");
    idleImages[1].Load(L"./resources/enemy/T_Lamprey_1.png");
    idleImages[2].Load(L"./resources/enemy/T_Lamprey_2.png");
    idleImages[3].Load(L"./resources/enemy/T_Lamprey_3.png");
    idleImages[4].Load(L"./resources/enemy/T_Lamprey_4.png");
}

// Yog
Yog::Yog(float x, float y, float speed, int health, float eWidth, float eHeight)
    : Enemy(x, y, speed, health, 1.0f, eWidth, eHeight) {
    LoadImages();
}

void Yog::LoadImages() {
    idleImages.resize(4);
    idleImages[0].Load(L"./resources/enemy/T_Yog_0.png");
    idleImages[1].Load(L"./resources/enemy/T_Yog_1.png");
    idleImages[2].Load(L"./resources/enemy/T_Yog_2.png");
    idleImages[3].Load(L"./resources/enemy/T_Yog_3.png");
}
