#include "Gun.h"

Gun::Gun(int maxAmmo) : maxAmmo(maxAmmo), currentAmmo(maxAmmo), reloading(false), reloadTime(2.0f), reloadTimer(0.0f) {}
Gun::~Gun() {}

void Gun::Reload() {
    reloading = true;
    reloadTimer = 0.0f;
}

bool Gun::FireBullet() {
    if (reloading || currentAmmo <= 0) return false;
    currentAmmo--;
    if (currentAmmo == 0) {
        Reload();
    }
    return true;
}

bool Gun::IsReloading() const {
    return reloading;
}

void Gun::UpdateReload(float frameTime) {
    if (reloading) {
        reloadTimer += frameTime;
        if (reloadTimer >= reloadTime) {
            reloading = false;
            currentAmmo = maxAmmo;
        }
    }
}


void Gun::Draw(HDC hdc, float playerX, float playerY, float cursorX, float cursorY, bool directionLeft) {
    float dx = cursorX - playerX;
    float dy = cursorY - playerY;
    float angle = atan2(dy, dx) * 180.0 / 3.14159265358979323846; 
    // Angle in degrees

    CImage* currentGunImage = directionLeft ? &r_gunImage : &gunImage;

    if (currentGunImage->IsNull()) {
        return; // �̹����� �ε���� �ʾ����� �׸��� ����
    }

    int gunWidth = currentGunImage->GetWidth();
    int gunHeight = currentGunImage->GetHeight();

    // Save the current state of the device context
    int savedDC = SaveDC(hdc);

    // Translate and rotate the device context
    SetGraphicsMode(hdc, GM_ADVANCED);
    XFORM xForm;

    // ���� ������ ���� ������ �ݴ�� �׸�����
    if (directionLeft) {
        angle += 180.0f;
    }

    xForm.eM11 = cos(DEG2RAD(angle));
    xForm.eM12 = sin(DEG2RAD(angle));
    xForm.eM21 = -sin(DEG2RAD(angle));
    xForm.eM22 = cos(DEG2RAD(angle));
    xForm.eDx = playerX + 10.0f; // ĳ������ Width�� ����
    xForm.eDy = playerY + 12.5f; // ĳ������ Height�� ����

    SetWorldTransform(hdc, &xForm);

    // ���� �� �̹��� Draw
    currentGunImage->Draw(hdc, -gunWidth / 2, -gunHeight / 2, gunWidth, gunHeight);

    // Restore the device context to its original state
    RestoreDC(hdc, savedDC);
}

// Revolver class implementation
Revolver::Revolver() : Gun(5) {
    LoadImages();
}

void Revolver::LoadImages() {
    if (FAILED(gunImage.Load(L"./resources/gun/RevolverStill.png"))) {
        MessageBox(NULL, L"Failed to load RevolverStill.png", L"Error", MB_OK);
    }
    if (FAILED(r_gunImage.Load(L"./resources/gun/rRevolverStill.png"))) {
        MessageBox(NULL, L"Failed to load rRevolverStill.png", L"Error", MB_OK);
    }
}

// HeadshotGun class implementation
HeadshotGun::HeadshotGun() : Gun(7) {
    LoadImages();
}

void HeadshotGun::LoadImages() {
    if (FAILED(gunImage.Load(L"./resources/gun/Headshot_Gun.png"))) {
        MessageBox(NULL, L"Failed to load Headshot_Gun.png", L"Error", MB_OK);
    }
    if (FAILED(r_gunImage.Load(L"./resources/gun/rHeadshot_Gun.png"))) {
        MessageBox(NULL, L"Failed to load rHeadshot_Gun.png", L"Error", MB_OK);
    }
}

// ClusterGun class implementation
ClusterGun::ClusterGun() : Gun(10) {
    LoadImages();
}

void ClusterGun::LoadImages() {
    if (FAILED(gunImage.Load(L"./resources/gun/Cluster_Gun.png"))) {
        MessageBox(NULL, L"Failed to load Cluster_Gun.png", L"Error", MB_OK);
    }
    if (FAILED(r_gunImage.Load(L"./resources/gun/rCluster_Gun.png"))) {
        MessageBox(NULL, L"Failed to load rCluster_Gun.png", L"Error", MB_OK);
    }
}

// DualShotgun class implementation
DualShotgun::DualShotgun() : Gun(4) {
    LoadImages();
}

void DualShotgun::LoadImages() {
    if (FAILED(gunImage.Load(L"./resources/gun/DualShotgun_Gun.png"))) {
        MessageBox(NULL, L"Failed to load DualShotgun.png", L"Error", MB_OK);
    }
    if (FAILED(r_gunImage.Load(L"./resources/gun/rDualShotgun_Gun.png"))) {
        MessageBox(NULL, L"Failed to load rDualShotgun.png", L"Error", MB_OK);
    }
}
