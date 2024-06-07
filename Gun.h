#pragma once
#include <atlimage.h> // CImage 사용을 위한 헤더
#include <windows.h>  // For POINT

class Gun {
public:
    Gun();
    ~Gun();

    void LoadImages();
    void Draw(HDC hdc, float playerX, float playerY, float cursorX, float cursorY, bool directionLeft);

private:
    CImage gunImage;
    CImage r_gunImage;

    constexpr double DEG2RAD(double deg) const {
        return deg * (3.14159265358979323846 / 180.0);
    }
};

class Revolver : public Gun {
public:
    Revolver();
    virtual void LoadImages();

private:
    CImage gunImage;
    CImage r_gunImage;
};

class HeadshotGun : public Gun {
public:
    HeadshotGun();
    virtual void LoadImages();

private:
    CImage gunImage;
    CImage r_gunImage;
};

class ClusterGun : public Gun {
public:
    ClusterGun();
    virtual void LoadImages();

private:
    CImage gunImage;
    CImage r_gunImage;
};

class DualShotgun : public Gun {
public:
    DualShotgun();
    virtual void LoadImages();

private:
    CImage gunImage;
    CImage r_gunImage;
};