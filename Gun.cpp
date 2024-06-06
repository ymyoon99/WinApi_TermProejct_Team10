#include "Gun.h"

Gun::Gun() {
    LoadImages();
}

Gun::~Gun() {
}

void Gun::LoadImages() {
    gunImage.Load(L"./resources/gun/RevolverStill.png");
    r_gunImage.Load(L"./resources/gun/rRevolverStill.png");
}

void Gun::Draw(HDC hdc, float playerX, float playerY, float cursorX, float cursorY, bool directionLeft) {
    float dx = cursorX - playerX;
    float dy = cursorY - playerY;
    float angle = atan2(dy, dx) * 180.0 / 3.14159265358979323846; // Angle in degrees

    CImage* currentGunImage = directionLeft ? &r_gunImage : &gunImage;

    int gunWidth = currentGunImage->GetWidth();
    int gunHeight = currentGunImage->GetHeight();

    // Save the current state of the device context
    int savedDC = SaveDC(hdc);

    // Translate and rotate the device context
    SetGraphicsMode(hdc, GM_ADVANCED);
    XFORM xForm;

    // If the direction is left, adjust the angle by 180 degrees
    if (directionLeft) {
        angle += 180.0f;
    }

    xForm.eM11 = cos(DEG2RAD(angle));
    xForm.eM12 = sin(DEG2RAD(angle));
    xForm.eM21 = -sin(DEG2RAD(angle));
    xForm.eM22 = cos(DEG2RAD(angle));
    xForm.eDx = playerX;
    xForm.eDy = playerY;

    SetWorldTransform(hdc, &xForm);

    // Draw the gun image
    currentGunImage->Draw(hdc, -gunWidth / 2, -gunHeight / 2, gunWidth, gunHeight);

    // Restore the device context to its original state
    RestoreDC(hdc, savedDC);
}


