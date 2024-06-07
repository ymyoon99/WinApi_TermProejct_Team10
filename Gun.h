#pragma once
#include <atlimage.h> // CImage ����� ���� ���
#include <windows.h>  // For POINT

class Gun {
public:
    Gun();
    virtual ~Gun();

    virtual void LoadImages() = 0;
    void Draw(HDC hdc, float playerX, float playerY, float cursorX, float cursorY, bool directionLeft);

protected:
    CImage gunImage;
    CImage r_gunImage;

    constexpr double DEG2RAD(double deg) const {
        return deg * (3.14159265358979323846 / 180.0);
    }
};

class Revolver : public Gun {
public:
    Revolver();
    virtual void LoadImages() override;
};

class HeadshotGun : public Gun {
public:
    HeadshotGun();
    virtual void LoadImages() override;
};

class ClusterGun : public Gun {
public:
    ClusterGun();
    virtual void LoadImages() override;
};

class DualShotgun : public Gun {
public:
    DualShotgun();
    virtual void LoadImages() override;
};
