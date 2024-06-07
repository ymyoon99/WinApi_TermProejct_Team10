#include "stdafx.h"
#include "GameFramework.h"

// GameFramework 인스턴스 전역 변수
extern GameFramework gameframework;

GameFramework::GameFramework() 
    : m_hdcBackBuffer(nullptr),
    m_hBitmap(nullptr), 
    m_hOldBitmap(nullptr), 
    player(nullptr), 
    camera(nullptr), 
    showClickImage(false), 
    clickImageTimer(0.0f), 
    enemySpawnTimer(0.0f),
    currentGun(&revolver) // 초기 총
{
    Clear();

    // 배경 이미지 로드
    mapImage.Load(L"./resources/background/background.png");

    // 배경 이미지 크기 가져오기
    int mapWidth = mapImage.GetWidth();
    int mapHeight = mapImage.GetHeight();

    // 플레이어를 배경의 정중앙에 배치
    player = new Player(mapWidth / 2.0f, mapHeight / 2.0f, 1.0f, 5.0f);
    player->SetBounds(mapWidth, mapHeight);

    // 카메라 초기화
    camera = new Camera(800, 600);
    camera->SetBounds(mapWidth, mapHeight);

    // 커서 이미지 로드
    cursorImage.Load(L"./resources/ui/icon_TakeAim.png");
    clickImage.Load(L"./resources/ui/T_CursorSprite.png");

    // 적 생성
    CreateEnemies();

    srand(static_cast<unsigned int>(time(NULL)));

    // 장애물 생성
    CreateObstacles(10);
}

GameFramework::~GameFramework() {
    CleanupDoubleBuffering();

    delete camera;
    delete player;

    for (Obstacle* obstacle : obstacles) {
        delete obstacle;
    }
    obstacles.clear();

    for (Enemy* enemy : enemies) {
        delete enemy;
    }
    enemies.clear();
}

void GameFramework::SpawnEnemy() {
    if (enemies.size() >= 100) {
        return;
    }

    float playerX = player->GetX();
    float playerY = player->GetY();
    float spawnRadius = 600.0f;

    float angle = (rand() % 360) * 3.14159265358979323846 / 180.0;
    float spawnX = playerX + spawnRadius * cos(angle);
    float spawnY = playerY + spawnRadius * sin(angle);

    const wchar_t* enemyImages[] = {
        L"./resources/enemy/T_Lamprey_0.png",
        L"./resources/enemy/T_Lamprey_1.png",
        L"./resources/enemy/T_Lamprey_2.png",
        L"./resources/enemy/T_Lamprey_3.png",
        L"./resources/enemy/T_Lamprey_4.png"
    };
    int numFrames = sizeof(enemyImages) / sizeof(enemyImages[0]);

    float enemyAnimationSpeed = 2.0f;
    float enemySpeed = 5.0f;

    enemies.push_back(new Enemy(spawnX, spawnY, enemySpeed, 10, enemyImages, numFrames, enemyAnimationSpeed));
}

void GameFramework::CreateEnemies() {
    for (int i = 0; i < 10; ++i) {
        SpawnEnemy();
    }
}

void GameFramework::Update(float frameTime) {
    player->Update(frameTime, obstacles);
    camera->Update(player->GetX(), player->GetY());

    // 적 업데이트
    auto enemyIter = enemies.begin();
    while (enemyIter != enemies.end()) {
        Enemy* enemy = *enemyIter;
        enemy->Update(frameTime, player->GetX(), player->GetY(), obstacles);

        if (enemy->IsDead()) { // 죽었으면 삭제 되도록.
            delete enemy;
            enemyIter = enemies.erase(enemyIter);
        }
        else {
            ++enemyIter;
        }
    }

    // 총알 업데이트
    for (Bullet* bullet : bullets) {
        bullet->Update(frameTime);
    }

    // 총알 충돌 검사 및 화면 밖 제거
    auto bulletIter = bullets.begin();
    while (bulletIter != bullets.end()) {
        Bullet* bullet = *bulletIter;
        bool bulletRemoved = false;
        if (bullet->IsOutOfBounds(mapImage.GetWidth(), mapImage.GetHeight())) {
            delete bullet;
            bulletIter = bullets.erase(bulletIter);
            bulletRemoved = true;
        }
        else {
            for (Enemy* enemy : enemies) {
                if (bullet->CheckCollision(enemy->GetX(), enemy->GetY(), 20.0f, 25.0f)) {
                    enemy->TakeDamage(bullet->GetDamage());
                    delete bullet;
                    bulletIter = bullets.erase(bulletIter);
                    bulletRemoved = true;
                    break;
                }
            }
        }
        if (!bulletRemoved) {
            ++bulletIter;
        }
    }

    // 적 스폰 타이머
    enemySpawnTimer += frameTime;
    if (enemySpawnTimer >= enemySpawnInterval) {
        SpawnEnemy();
        enemySpawnTimer = 0.0f;
    }

    // 마우스 클릭 이미지
    if (showClickImage) {
        clickImageTimer -= frameTime;
        if (clickImageTimer <= 0.0f) {
            showClickImage = false;
        }
    }
}

void GameFramework::CreateObstacles(int numObstacles) {
    int mapWidth = mapImage.GetWidth();
    int mapHeight = mapImage.GetHeight();

    for (int i = 0; i < numObstacles; ++i) {
        float x = static_cast<float>(rand() % mapWidth);
        float y = static_cast<float>(rand() % mapHeight);
        obstacles.push_back(new Obstacle(x, y));
    }
}

// FireBullet 함수 구현
void GameFramework::FireBullet(float x, float y, float targetX, float targetY) {
    if (dynamic_cast<Revolver*>(currentGun)) {
        bullets.push_back(new RevolverBullet(x, y, targetX, targetY));
    }
    else if (dynamic_cast<HeadshotGun*>(currentGun)) {
        bullets.push_back(new HeadshotGunBullet(x, y, targetX, targetY));
    }
    else if (dynamic_cast<ClusterGun*>(currentGun)) {
        bullets.push_back(new ClusterGunBullet(x, y, targetX, targetY));
        bullets.push_back(new ClusterGunBullet(x, y, targetX, targetY + 10));
    }
    else if (dynamic_cast<DualShotgun*>(currentGun)) {
        for (int i = -2; i <= 2; ++i) {
            float spreadAngle = i * 10.0f * (3.14159265358979323846 / 180.0f);
            bullets.push_back(new DualShotgunBullet(x, y, targetX, targetY, spreadAngle));
        }
    }
}

void GameFramework::Draw(HDC hdc) {
    if (!m_hdcBackBuffer) {
        InitializeDoubleBuffering(hdc);
    }

    RECT clientRect;
    GetClientRect(m_hWnd, &clientRect);

    FillRect(m_hdcBackBuffer, &clientRect, (HBRUSH)(COLOR_WINDOW + 1));

    float offsetX = camera->GetOffsetX();
    float offsetY = camera->GetOffsetY();

    mapImage.Draw(m_hdcBackBuffer, -static_cast<int>(offsetX), -static_cast<int>(offsetY));

    player->Draw(m_hdcBackBuffer, offsetX, offsetY);
    player->DrawBoundingBox(m_hdcBackBuffer, offsetX, offsetY);

    for (Enemy* enemy : enemies) {
        enemy->Draw(m_hdcBackBuffer, offsetX, offsetY);
    }

    for (Obstacle* obstacle : obstacles) {
        obstacle->Draw(m_hdcBackBuffer, offsetX, offsetY);
    }

    for (Bullet* bullet : bullets) {
        bullet->Draw(m_hdcBackBuffer, offsetX, offsetY);
    }

    // 현재 총 그리기
    currentGun->Draw(m_hdcBackBuffer, player->GetX() - offsetX, player->GetY() - offsetY,
        cursorPos.x, cursorPos.y, player->IsDirectionLeft());

    int cursorWidth = cursorImage.GetWidth();
    int cursorHeight = cursorImage.GetHeight();
    int clickWidth = clickImage.GetWidth();
    int clickHeight = clickImage.GetHeight();

    if (showClickImage) {
        clickImage.Draw(m_hdcBackBuffer, cursorPos.x - clickWidth / 2, cursorPos.y - clickHeight / 2);
    }
    else {
        cursorImage.Draw(m_hdcBackBuffer, cursorPos.x - cursorWidth / 2, cursorPos.y - cursorHeight / 2);
    }

    BitBlt(hdc, 0, 0, clientRect.right, clientRect.bottom, m_hdcBackBuffer, 0, 0, SRCCOPY);
}

void GameFramework::OnKeyBoardProcessing(UINT iMessage, WPARAM wParam, LPARAM lParam) {
    switch (iMessage) {
    case WM_KEYDOWN:
        if (wParam == 'Q') {
            SendMessage(m_hWnd, WM_DESTROY, 0, 0);
            return;
        }
        switch (wParam) {
        case 'A':
        case 'a':
            player->moveLeft = true;
            break;
        case 'D':
        case 'd':
            player->moveRight = true;
            break;
        case 'W':
        case 'w':
            player->moveUp = true;
            break;
        case 'S':
        case 's':
            player->moveDown = true;
            break;
        case '1':
            currentGun = &revolver;
            break;
        case '2':
            currentGun = &headshotGun;
            break;
        case '3':
            currentGun = &clusterGun;
            break;
        case '4':
            currentGun = &dualShotgun;
            break;
        }
        break;

    case WM_KEYUP:
        switch (wParam) {
        case 'A':
        case 'a':
            player->moveLeft = false;
            break;
        case 'D':
        case 'd':
            player->moveRight = false;
            break;
        case 'W':
        case 'w':
            player->moveUp = false;
            break;
        case 'S':
        case 's':
            player->moveDown = false;
            break;
        }
        break;
    }
}

void GameFramework::OnMouseProcessing(UINT iMessage, WPARAM wParam, LPARAM lParam) {
    switch (iMessage) {
    case WM_MOUSEMOVE: {
        cursorPos.x = LOWORD(lParam);
        cursorPos.y = HIWORD(lParam);

        float playerScreenX = player->GetX() - camera->GetOffsetX();
        if (cursorPos.x < playerScreenX) {
            player->SetDirectionLeft(true);
        }
        else {
            player->SetDirectionLeft(false);
        }
        break;
    }
    case WM_LBUTTONDOWN:
        showClickImage = true;
        clickImageTimer = 0.2f;
        cursorPos.x = LOWORD(lParam);
        cursorPos.y = HIWORD(lParam);
        FireBullet(player->GetX(), player->GetY(), 
            cursorPos.x + camera->GetOffsetX(), cursorPos.y + camera->GetOffsetY());
        break;
    }
}

void GameFramework::InitializeDoubleBuffering(HDC hdc) {
    RECT clientRect;
    GetClientRect(m_hWnd, &clientRect);

    m_hdcBackBuffer = CreateCompatibleDC(hdc);
    m_hBitmap = CreateCompatibleBitmap(hdc, clientRect.right, clientRect.bottom);
    m_hOldBitmap = (HBITMAP)SelectObject(m_hdcBackBuffer, m_hBitmap);
}

void GameFramework::CleanupDoubleBuffering() {
    if (m_hdcBackBuffer) {
        SelectObject(m_hdcBackBuffer, m_hOldBitmap);
        DeleteObject(m_hBitmap);
        DeleteDC(m_hdcBackBuffer);
        m_hdcBackBuffer = nullptr;
    }
}

void GameFramework::Clear() {
    // 필요한 자원 해제 로직 추가
}

void GameFramework::Create(HWND hWnd) {
    m_hWnd = hWnd;
}
