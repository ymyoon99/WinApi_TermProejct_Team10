#pragma once

#include <windows.h>
#include <atlimage.h>
#include "Player.h"
#include "Camera.h"
#include "Gun.h"
#include "Enemy.h"
#include <vector>
#include "Obstacle.h"
#include "Bullet.h"
#include "Item.h"

class GameFramework {
public:
    GameFramework();
    ~GameFramework();

    void Create(HWND hWnd);
    void Update(float frameTime);
    void Draw(HDC hdc);
    void OnKeyBoardProcessing(UINT iMessage, WPARAM wParam, LPARAM lParam);
    void OnMouseProcessing(UINT iMessage, WPARAM wParam, LPARAM lParam);

    void Clear();

    void SpawnItem(float x, float y);

private:
    void InitializeDoubleBuffering(HDC hdc);
    void CleanupDoubleBuffering();

    void StartCreateEnemies();
    void CreateObstacles(int numObstacles);

    void FireBullet(float x, float y, float targetX, float targetY);

    void SpawnBrainMonster();
    void SpawnEyeMonster();
    void SpawnBigBoomer();
    void SpawnLamprey();
    void SpawnYog();

    void DrawBulletUI(HDC hdc);
    void DrawReloadingUI(HDC hdc);
    void DrawFrameTime(HDC hdc); 
    void DrawGameTime(HDC hdc);


    HWND m_hWnd;
    HDC m_hdcBackBuffer;
    HBITMAP m_hBitmap;
    HBITMAP m_hOldBitmap;

    CImage mapImage;
    Player* player;
    Camera* camera;

    Gun* currentGun;
    Revolver revolver;
    HeadshotGun headshotGun;
    ClusterGun clusterGun;
    DualShotgun dualShotgun;

    CImage cursorImage;
    CImage clickImage;
    bool showClickImage;
    float clickImageTimer;
    POINT cursorPos;

    std::vector<Enemy*> enemies;

    float enemySpawnTimer;
    float bigBoomerSpawnTimer;
    float lampreySpawnTimer;
    float yogSpawnTimer;

    const float enemySpawnInterval = 10.0f;
    const float bigBoomerSpawnInterval = 30.0f;
    const float lampreySpawnInterval = 45.0f;
    const float yogSpawnInterval = 60.0f;

    std::vector<Obstacle*> obstacles;
    std::vector<Bullet*> bullets;
    std::vector<Item*> items;

    float frameTime;  // 프레임 타임을 저장
    int gameTimeSeconds;

    CImage bulletUI;
    CImage bulletUsedUI;

};
