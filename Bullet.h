#pragma once
#include <atlimage.h> // CImage 사용을 위한 헤더
#include <vector>

class Bullet {
public:
    Bullet(float x, float y, float targetX, float targetY, int damage, float speed);
    ~Bullet();

    void Update(float frameTime);
    void Draw(HDC hdc, float offsetX, float offsetY);
    bool IsOutOfBounds(float width, float height) const;
    bool CheckCollision(float enemyX, float enemyY, float enemyWidth, float enemyHeight) const;
    int GetDamage() const;

private:
    float x, y;
    float speed;
    float directionX, directionY;
    int damage;
    CImage bulletImage;
};

class RevolverBullet : public Bullet {
public:
    RevolverBullet(float x, float y, float targetX, float targetY);
};

class HeadshotGunBullet : public Bullet {
public:
    HeadshotGunBullet(float x, float y, float targetX, float targetY);
};

class ClusterGunBullet : public Bullet {
public:
    ClusterGunBullet(float x, float y, float targetX, float targetY);
};

class DualShotgunBullet : public Bullet {
public:
    DualShotgunBullet(float x, float y, float targetX, float targetY, float spreadAngle);
    float directionX, directionY;
};
