#include "stdafx.h"
#include "GameFramework.h"

// GameFramework 인스턴스 전역 변수
extern GameFramework gameframework;

GameFramework::GameFramework() : m_hdcBackBuffer(nullptr), m_hBitmap(nullptr), m_hOldBitmap(nullptr), player(nullptr), camera(nullptr), showClickImage(false), clickImageTimer(0.0f), enemySpawnTimer(0.0f) {
    Clear();

    // 배경 이미지 로드
    mapImage.Load(L"./resources/background/background.png");

    // 배경 이미지 크기 가져오기
    int mapWidth = mapImage.GetWidth();
    int mapHeight = mapImage.GetHeight();

    // 플레이어를 배경의 정중앙에 배치
    player = new Player(mapWidth / 2.0f, mapHeight / 2.0f, 0.2f, 5.0f); // 마지막 파라미터가, 애니메이션 속도.
    player->SetBounds(mapWidth, mapHeight); // 플레이어 경계 설정

    // 카메라 초기화
    camera = new Camera(800, 600);
    camera->SetBounds(mapWidth, mapHeight); // 카메라 경계 설정

    // 총 객체는 이제 자동으로 초기화됨

    // 커서 이미지 로드
    cursorImage.Load(L"./resources/ui/icon_TakeAim.png");
    clickImage.Load(L"./resources/ui/T_CursorSprite.png");

    CreateEnemies(); // 적 객체 생성
}

GameFramework::~GameFramework() {
    CleanupDoubleBuffering();
    delete camera;
    delete player; // 플레이어 삭제

    for (Enemy* enemy : enemies) {
        delete enemy;
    }
    enemies.clear();
}

void GameFramework::SpawnEnemy() {
    // 적의 수를 100마리로 제한
    if (enemies.size() >= 100) {
        return;
    }

    float playerX = player->GetX();
    float playerY = player->GetY();
    float spawnRadius = 600.0f; // 적이 생성될 반경

    float angle = (rand() % 360) * 3.14159265358979323846 / 180.0; // 0 ~ 360도 랜덤 각도
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

    float enemyAnimationSpeed = 2.0f; // 적 애니메이션 속도
    float enemySpeed = 5.0f; // 적 이동 속도

    enemies.push_back(new Enemy(spawnX, spawnY, enemySpeed, 10, enemyImages, numFrames, enemyAnimationSpeed));
}

void GameFramework::CreateEnemies() {
    // 초기 적 생성
    for (int i = 0; i < 10; ++i) {
        SpawnEnemy();
    }
}

void GameFramework::Update(float frameTime) {
    player->Update(frameTime);
    camera->Update(player->GetX(), player->GetY());

    for (Enemy* enemy : enemies) {
        enemy->Update(frameTime, player->GetX(), player->GetY());
    }

    // 주기적으로 적 생성
    enemySpawnTimer += frameTime;
    if (enemySpawnTimer >= enemySpawnInterval) {
        SpawnEnemy();
        enemySpawnTimer = 0.0f;
    }

    if (showClickImage) {
        clickImageTimer -= frameTime;
        if (clickImageTimer <= 0.0f) {
            showClickImage = false;
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

    gun.Draw(m_hdcBackBuffer, player->GetX() - offsetX, player->GetY() - offsetY,
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

        // 커서의 위치를 기반으로 플레이어의 방향 결정
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
        clickImageTimer = 0.2f; // 클릭 이미지가 깜빡이는 시간
        cursorPos.x = LOWORD(lParam);
        cursorPos.y = HIWORD(lParam);
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

// Clear 메서드 정의
void GameFramework::Clear() {
    // 필요한 자원 해제 로직 추가
}

// Create 메서드 정의
void GameFramework::Create(HWND hWnd) {
    m_hWnd = hWnd;
}
