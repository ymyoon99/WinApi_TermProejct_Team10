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
