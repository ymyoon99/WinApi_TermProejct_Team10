#pragma once

#include <windows.h>
#include <atlimage.h>
#include "Player.h"
#include "Camera.h"
#include "Gun.h"
#include "Enemy.h"
#include <vector>
#include "Obstacle.h"

class GameFramework {
public:
    GameFramework();
    ~GameFramework();

    void Create(HWND hWnd);
    void Update(float frameTime);
    void Draw(HDC hdc);
    void OnKeyBoardProcessing(UINT iMessage, WPARAM wParam, LPARAM lParam);
    void OnMouseProcessing(UINT iMessage, WPARAM wParam, LPARAM lParam);
    void CreateEnemies();
    void CreateObstacles(int numObstacles);
    void Clear();

private:
    void InitializeDoubleBuffering(HDC hdc);
    void CleanupDoubleBuffering();
    void SpawnEnemy();

    HWND m_hWnd;
    HDC m_hdcBackBuffer;
    HBITMAP m_hBitmap;
    HBITMAP m_hOldBitmap;

    CImage mapImage;
    Player* player;
    Camera* camera;

    Gun* currentGun; // 현재 사용 중인 총
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
    const float enemySpawnInterval = 1.0f;

    std::vector<Obstacle*> obstacles;
};
