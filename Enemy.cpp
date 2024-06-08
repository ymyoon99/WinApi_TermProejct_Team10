#include "Enemy.h"
#include <cmath>
#include <iostream> // 디버깅 메시지 출력을 위해 추가

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
<<<<<<< Updated upstream
    enemyImages[currentFrame].Draw(hdc, static_cast<int>(x - offsetX), static_cast<int>(y - offsetY));
=======
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
>>>>>>> Stashed changes
}

BossYog::BossYog(float x, float y, float speed, int health, float eWidth, float eHeight)
    : Enemy(x, y, speed, health, 1.0f, eWidth, eHeight),
    isPreparing(false),
    isCharging(false),
    prepareTime(0.0f),
    prepareDuration(1.0f), // 준비 동작 지속 시간
    chargeSpeed(300.0f),
    chargeCooldown(4.0f), // 돌진 주기를 4초로 설정
    chargeTimer(0.0f),
    chargeDistance(0.0f), // 돌진 거리 초기화
    prepareFrame(0),
    prepareFrameTime(0.0f),
    prepareFrameDuration(0.1f) { // 준비 동작 프레임 속도
    LoadImages();
}

void BossYog::LoadImages() {
    idleImages.resize(5);
    idleImages[0].Load(L"./resources/enemy/WingedMonster_0.png");
    idleImages[1].Load(L"./resources/enemy/WingedMonster_1.png");
    idleImages[2].Load(L"./resources/enemy/WingedMonster_2.png");
    idleImages[3].Load(L"./resources/enemy/WingedMonster_3.png");
    idleImages[4].Load(L"./resources/enemy/WingedMonster_4.png");

    // 준비 동작 이미지를 로드
    prepareImages.resize(6);
    prepareImages[0].Load(L"./resources/effect/T_FireExplosionSmall_0.png");
    prepareImages[1].Load(L"./resources/effect/T_FireExplosionSmall_1.png");
    prepareImages[2].Load(L"./resources/effect/T_FireExplosionSmall_2.png");
    prepareImages[3].Load(L"./resources/effect/T_FireExplosionSmall_3.png");
    prepareImages[4].Load(L"./resources/effect/T_FireExplosionSmall_4.png");
    prepareImages[5].Load(L"./resources/effect/T_FireExplosionSmall_5.png");

    // 히트박스를 이미지 크기에 맞춰 조정
    eWidth = static_cast<float>(idleImages[0].GetWidth());
    eHeight = static_cast<float>(idleImages[0].GetHeight());
}

void BossYog::Update(float frameTime, float playerX, float playerY, const std::vector<Obstacle*>& obstacles) {
    chargeTimer += frameTime;

    if (isPreparing) {
        prepareTime += frameTime;
        prepareFrameTime += frameTime;

        if (prepareFrameTime >= prepareFrameDuration) {
            prepareFrame = (prepareFrame + 1) % prepareImages.size();
            prepareFrameTime = 0.0f;
        }

        if (prepareTime >= prepareDuration) {
            isPreparing = false;
            isCharging = true;
            prepareTime = 0.0f;

            float dx = playerX - x;
            float dy = playerY - y;
            float distance = sqrt(dx * dx + dy * dy);
            chargeDistance = distance; // 플레이어와의 간격만큼 돌진
        }
    }
    else if (isCharging) {
        float dx = playerX - x;
        float dy = playerY - y;
        float distance = sqrt(dx * dx + dy * dy);

        float directionX = dx / distance;
        float directionY = dy / distance;

        float newX = x + directionX * chargeSpeed * frameTime;
        float newY = y + directionY * chargeSpeed * frameTime;

        if (!CheckCollision(newX, newY, obstacles)) {
            x = newX;
            y = newY;
            chargeDistance -= chargeSpeed * frameTime;
        }

        if (chargeDistance <= 0.0f) { // 돌진 거리가 0 이하가 되면 돌진 멈춤
            isCharging = false;
            chargeTimer = 0.0f;
        }
    }
    else {
        float dx = playerX - x;
        float dy = playerY - y;
        float distance = sqrt(dx * dx + dy * dy);

        if (chargeTimer >= chargeCooldown) {
            isPreparing = true;
            chargeTimer = 0.0f;
        }
        else {
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
    }
}

void BossYog::Draw(HDC hdc, float offsetX, float offsetY) {
    if (isPreparing && !prepareImages[prepareFrame].IsNull()) {
        prepareImages[prepareFrame].Draw(hdc, static_cast<int>(x - offsetX), static_cast<int>(y - offsetY));
    }
    else if (!idleImages[currentFrame].IsNull()) {
        idleImages[currentFrame].Draw(hdc, static_cast<int>(x - offsetX), static_cast<int>(y - offsetY));
    }
}