#include "Bullet.h"
#include <cmath>

Bullet::Bullet(float x, float y, float targetX, float targetY, int damage, float speed)
    : x(x), y(y), speed(speed), damage(damage) {
    float dx = targetX - x;
    float dy = targetY - y;
    float distance = sqrt(dx * dx + dy * dy);
    directionX = dx / distance;
    directionY = dy / distance;
    bulletImage.Load(L"./resources/gun/SmallCircle.png");
}

Bullet::~Bullet() {}

void Bullet::Update(float frameTime) {
    x += directionX * speed * frameTime;
    y += directionY * speed * frameTime;
}

void Bullet::Draw(HDC hdc, float offsetX, float offsetY) {
    bulletImage.Draw(hdc, static_cast<int>(x - offsetX), static_cast<int>(y - offsetY));
}

bool Bullet::IsOutOfBounds(float width, float height) const {
    return x < 0 || y < 0 || x > width || y > height;
}

bool Bullet::CheckCollision(float enemyX, float enemyY, float enemyWidth, float enemyHeight) const {
    return x < enemyX + enemyWidth && x > enemyX && y < enemyY + enemyHeight && y > enemyY;
}

int Bullet::GetDamage() const {
    return damage;
}

// RevolverBullet
RevolverBullet::RevolverBullet(float x, float y, float targetX, float targetY)
    : Bullet(x, y, targetX, targetY, 50, 500.0f) {}

// HeadshotGunBullet
HeadshotGunBullet::HeadshotGunBullet(float x, float y, float targetX, float targetY)
    : Bullet(x, y, targetX, targetY, 100, 600.0f) {}

// ClusterGunBullet
ClusterGunBullet::ClusterGunBullet(float x, float y, float targetX, float targetY)
    : Bullet(x, y, targetX, targetY, 50, 400.0f) {}

// DualShotgunBullet
DualShotgunBullet::DualShotgunBullet(float x, float y, float targetX, float targetY, float spreadAngle)
    : Bullet(x, y, targetX + cos(spreadAngle) * 100, targetY + sin(spreadAngle) * 100, 100, 300.0f) {}
