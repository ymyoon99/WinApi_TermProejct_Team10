#include "Enemy.h"
#include <cmath>

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

void Enemy::Update(float frameTime, float playerX, float playerY) {
    // ���ΰ��� ���� �̵��ϴ� ����
    float dx = playerX - x;
    float dy = playerY - y;
    float distance = sqrt(dx * dx + dy * dy);

    // ����ȭ�� ���� ���� ���
    float directionX = dx / distance;
    float directionY = dy / distance;

    // �̵�
    x += directionX * speed * frameTime;
    y += directionY * speed * frameTime;

    // �ִϸ��̼� ������Ʈ
    frameTimeAccumulator += frameTime;
    if (frameTimeAccumulator >= animationSpeed) {
        currentFrame = (currentFrame + 1) % numFrames;
        frameTimeAccumulator = 0.0f;
    }
}

void Enemy::Draw(HDC hdc, float offsetX, float offsetY) {
    enemyImages[currentFrame].Draw(hdc, static_cast<int>(x - offsetX), static_cast<int>(y - offsetY));
}

void Enemy::TakeDamage(int damage) {
    health -= damage;
}

bool Enemy::IsDead() const {
    return health <= 0;
}

float Enemy::GetX() const {
    return x;
}

float Enemy::GetY() const {
    return y;
}

void Enemy::SetBounds(float width, float height) {
    boundWidth = width;
    boundHeight = height;
}
