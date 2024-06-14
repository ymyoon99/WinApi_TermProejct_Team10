#include "stdafx.h"
#include "GameFramework.h"

extern GameFramework gameframework;
extern HFONT hFont;
std::vector<Enemy*> enemies;

GameFramework::GameFramework()
    : m_hdcBackBuffer(nullptr),
    m_hBitmap(nullptr),
    m_hOldBitmap(nullptr),
    player(nullptr),
    camera(nullptr),
    showClickImage(false),
    clickImageTimer(0.0f),
    enemySpawnTimer(0.0f),
    bigBoomerSpawnTimer(0.0f),
    lampreySpawnTimer(0.0f),
    yogSpawnTimer(0.0f),
    currentGun(&revolver),
    frameTime(0.0f),
    gameTimeSeconds(0),
    isPaused(false) {
    Clear();

    mapImage.Load(L"./resources/background/background.png");
    pauseUIImage.Load(L"./resources/ui/T_PauseMenu.png");

    int mapWidth = mapImage.GetWidth();
    int mapHeight = mapImage.GetHeight();

    player = new Player(mapWidth / 2.0f, mapHeight / 2.0f, 2.0f, 0.2f); // 플레이어 생성
    // xPos, yPos, speed, animationSpeed
    player->SetBounds(mapWidth, mapHeight);

    camera = new Camera(800, 600);
    camera->SetBounds(mapWidth, mapHeight);

    cursorImage.Load(L"./resources/ui/icon_TakeAim.png");
    clickImage.Load(L"./resources/ui/T_CursorSprite.png");

    StartCreateEnemies();
    srand(static_cast<unsigned int>(time(NULL)));
    CreateObstacles(10);

    bulletUI.Load(L"./resources/ui/bullet_ui.png");
    bulletUsedUI.Load(L"./resources/ui/bullet_used_ui.png");
}

HFONT hFont = nullptr;

void InitializeFont() {

    AddFontResourceEx(L"./resources/font/ChevyRay - Lantern.ttf", FR_PRIVATE, nullptr);

    hFont = CreateFont(
        -30,                      // Height of the font
        0,                        // Width of the font
        0,                        // Escapement angle
        0,                        // Orientation angle
        FW_NORMAL,                // Font weight
        FALSE,                    // Italic attribute
        FALSE,                    // Underline attribute
        FALSE,                    // Strikeout attribute
        ANSI_CHARSET,             // Character set identifier
        OUT_TT_PRECIS,            // Output precision
        CLIP_DEFAULT_PRECIS,      // Clipping precision
        ANTIALIASED_QUALITY,      // Output quality
        DEFAULT_PITCH | FF_DONTCARE,  // Pitch and family
        L"ChevyRay - Lantern"     // Font name
    );
}

GameFramework::~GameFramework() {
    CleanupDoubleBuffering();

    if (hFont) { // 폰트 Release
        DeleteObject(hFont);
        hFont = nullptr;
    }

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

void GameFramework::TogglePause() {
    isPaused = !isPaused;
}

void GameFramework::StartCreateEnemies() {
    for (int i = 0; i < 10; ++i) {
        SpawnBrainMonster();
        SpawnEyeMonster();
    }
}

void GameFramework::SpawnBrainMonster() {
    float playerX = player->GetX();
    float playerY = player->GetY();
    float spawnRadius = 600.0f;


    float angle = (rand() % 360) * 3.14159265358979323846 / 180.0;
    float spawnX = playerX + spawnRadius * cos(angle);
    float spawnY = playerY + spawnRadius * sin(angle);

    enemies.push_back(new BrainMonster(spawnX, spawnY, 5.0f));
}

void GameFramework::SpawnEyeMonster() {
    float playerX = player->GetX();
    float playerY = player->GetY();
    float spawnRadius = 600.0f;

    float angle = (rand() % 360) * 3.14159265358979323846 / 180.0;
    float spawnX = playerX + spawnRadius * cos(angle);
    float spawnY = playerY + spawnRadius * sin(angle);

    enemies.push_back(new EyeMonster(spawnX, spawnY, 5.0f));
}

void GameFramework::SpawnBigBoomer() {
    float playerX = player->GetX();
    float playerY = player->GetY();
    float spawnRadius = 600.0f;

    float angle = (rand() % 360) * 3.14159265358979323846 / 180.0;
    float spawnX = playerX + spawnRadius * cos(angle);
    float spawnY = playerY + spawnRadius * sin(angle);

    enemies.push_back(new BigBoomer(spawnX, spawnY, 5.0f));
}

void GameFramework::SpawnLamprey() {
    float playerX = player->GetX();
    float playerY = player->GetY();
    float spawnRadius = 600.0f;

    for (int i = 0; i < 2; ++i) {
        float angle = (rand() % 360) * 3.14159265358979323846 / 180.0;
        float spawnX = playerX + spawnRadius * cos(angle);
        float spawnY = playerY + spawnRadius * sin(angle);

        enemies.push_back(new Lamprey(spawnX, spawnY, 5.0f));
    }
}

void GameFramework::SpawnYog() {
    float playerX = player->GetX();
    float playerY = player->GetY();
    float spawnRadius = 600.0f;

    float angle = (rand() % 360) * 3.14159265358979323846 / 180.0;
    float spawnX = playerX + spawnRadius * cos(angle);
    float spawnY = playerY + spawnRadius * sin(angle);

    enemies.push_back(new Yog(spawnX, spawnY, 5.0f));
}

void GameFramework::DrawBulletUI(HDC hdc) {
    int x = 10;
    int y = 80;
    for (int i = 0; i < currentGun->maxAmmo; i++) {
        if (i < currentGun->currentAmmo) {
            bulletUI.Draw(m_hdcBackBuffer, x + i * 20, y);
        }
        else {
            bulletUsedUI.Draw(m_hdcBackBuffer, x + i * 20, y);
        }
    }
}

void GameFramework::DrawReloadingUI(HDC hdc) {
    if (currentGun->IsReloading()) {
        RECT rect;
        rect.left = static_cast<LONG>(player->GetX() - camera->GetOffsetX() - 15);
        rect.top = static_cast<LONG>(player->GetY() - camera->GetOffsetY() - 20);
        rect.right = rect.left + 50;
        rect.bottom = rect.top + 5;

        // 흰색 배경
        HBRUSH whiteBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
        FillRect(m_hdcBackBuffer, &rect, whiteBrush);

        // 빨간색 진행 바
        int width = static_cast<int>((currentGun->reloadTimer / currentGun->reloadTime) * 50);
        rect.right = rect.left + width;
        HBRUSH redBrush = CreateSolidBrush(RGB(255, 0, 0));
        FillRect(m_hdcBackBuffer, &rect, redBrush);
        DeleteObject(redBrush);  // 브러시 삭제
    }
}

void GameFramework::SpawnItem(float x, float y) {
    items.push_back(new Item(x, y));
}

void GameFramework::Update(float frameTime) {
    if (isPaused) {
        return; 
    }

    this->frameTime = frameTime;  // 프레임 타임 저장

    static float timeAccumulator = 0.0f;
    timeAccumulator += frameTime;
    if (timeAccumulator >= 1.0f) {
        gameTimeSeconds += static_cast<int>(timeAccumulator);
        timeAccumulator = 0.0f;
    }

    // 플레이어 업데이트
    player->Update(frameTime, obstacles);
    camera->Update(player->GetX(), player->GetY());

    // 플레이어와 적 충돌체크
    for (Enemy* enemy : enemies) {
        if (!player->IsInvincible() &&
            abs(player->GetX() - enemy->GetX()) < (20.0f + enemy->GetWidth()) / 2 &&
            abs(player->GetY() - enemy->GetY()) < (25.0f + enemy->GetHeight()) / 2) {
            player->TakeDamage(1);
        }
    }
    
    // 아이템 업데이트 및 수집
    auto itemIter = items.begin();
    while (itemIter != items.end()) {
        Item* item = *itemIter;
        item->Update(frameTime);

        // 플레이어와 아이템의 충돌 검사 및 수집
        if (abs(player->GetX() - item->GetX()) < 20.0f && abs(player->GetY() - item->GetY()) < 25.0f) {
            player->AddExperience(10);
            delete item;
            itemIter = items.erase(itemIter);
        }
        else {
            ++itemIter;
        }
    }

    // 적 업데이트
    auto enemyIter = enemies.begin();
    while (enemyIter != enemies.end()) {
        Enemy* enemy = *enemyIter;
        enemy->Update(frameTime, player->GetX(), player->GetY(), obstacles);

        if (enemy->IsDead()) {
            // 적이 죽을 때 아이템 드롭
            SpawnItem(enemy->GetX(), enemy->GetY());

            delete enemy;
            enemyIter = enemies.erase(enemyIter);
        }
        else {
            ++enemyIter;
        }
    }

    // 총알 업데이트
    auto bulletIter = bullets.begin();
    while (bulletIter != bullets.end()) {
        Bullet* bullet = *bulletIter;
        bullet->Update(frameTime);

        bool bulletRemoved = false;
        if (bullet->IsOutOfBounds(mapImage.GetWidth(), mapImage.GetHeight())) {
            bulletRemoved = true;
        }
        else if (bullet->isHit) {
            if (bullet->isEffectFinished()) {
                bulletRemoved = true;
            }
        }
        else {
            for (Enemy* enemy : enemies) {
                if (bullet->CheckCollision(enemy->GetX(), enemy->GetY(), enemy->GetWidth(), enemy->GetHeight())) {
                    enemy->TakeDamage(bullet->GetDamage());
                    bullet->isHit = true; // Set bullet hit
                    break;
                }
            }
        }

        if (bulletRemoved) {
            delete bullet;
            bulletIter = bullets.erase(bulletIter);
        }
        else {
            ++bulletIter;
        }
    }

    // 적 스폰 시스템
    enemySpawnTimer += frameTime;
    if (enemySpawnTimer >= enemySpawnInterval) {
        for (int i = 0; i < 10; i++) {
            SpawnBrainMonster();
            SpawnEyeMonster();
        }
        enemySpawnTimer = 0.0f;
    }

    bigBoomerSpawnTimer += frameTime;
    if (bigBoomerSpawnTimer >= bigBoomerSpawnInterval) {
        SpawnBigBoomer();
        SpawnBigBoomer();
        SpawnBigBoomer();
        bigBoomerSpawnTimer = 0.0f;
    }

    lampreySpawnTimer += frameTime;
    if (lampreySpawnTimer >= lampreySpawnInterval) {
        SpawnLamprey();
        SpawnLamprey();
        lampreySpawnTimer = 0.0f;
    }

    yogSpawnTimer += frameTime;
    if (yogSpawnTimer >= yogSpawnInterval) {
        SpawnYog();
        yogSpawnTimer = 0.0f;
    }

    // 커서 이미지 업데이트
    if (showClickImage) {
        clickImageTimer -= frameTime;
        if (clickImageTimer <= 0.0f) {
            showClickImage = false;
        }
    }

    // 총 장전 업데이트
    currentGun->UpdateReload(frameTime);

    // 디버그 키 업데이트
    HandleDebugKeys();
}

void GameFramework::HandleDebugKeys() {
    if (GetAsyncKeyState(VK_F1) & 0x8000) {
        if (!f1Pressed) {
            player->maxHealth += 1;
            player->health += 1;
            f1Pressed = true;
        }
    }
    else {
        f1Pressed = false;
    }

    if (GetAsyncKeyState(VK_F2) & 0x8000) {
        if (!f2Pressed) {
            revolver.maxAmmo += 1;
            headshotGun.maxAmmo += 1;
            clusterGun.maxAmmo += 1;
            dualShotgun.maxAmmo += 1;
            f2Pressed = true;
        }
    }
    else {
        f2Pressed = false;
    }

    if (GetAsyncKeyState(VK_F3) & 0x8000) {
        if (!f3Pressed) {
            player->speed += 1.0f;
            f3Pressed = true;
        }
    }
    else {
        f3Pressed = false;
    }

    if (GetAsyncKeyState(VK_F4) & 0x8000) {
        if (!f4Pressed) {
            // 총 업그레이드
            if (currentGun == &revolver) {
                currentGun = &headshotGun;
            }
            else if (currentGun == &headshotGun) {
                currentGun = &clusterGun;
            }
            else if (currentGun == &clusterGun) {
                currentGun = &dualShotgun;
            }
            f4Pressed = true;
        }
    }
    else {
        f4Pressed = false;
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

void GameFramework::FireBullet(float x, float y, float targetX, float targetY) {
    if (currentGun->FireBullet()) {
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
            int numBullets = 5; // 발사할 총알의 개수
            float spreadAngle = 10.0f * (3.14159265358979323846 / 180.0f); // 스프레드 각도(라디안 단위로 변환)
            float baseAngle = atan2(targetY - y, targetX - x);

            for (int i = 0; i < numBullets; ++i) {
                float angle = baseAngle + spreadAngle * (i - numBullets / 2);
                float newTargetX = x + cos(angle) * 100;
                float newTargetY = y + sin(angle) * 100;
                bullets.push_back(new DualShotgunBullet(x, y, newTargetX, newTargetY, 0));
            }
        }
    }
}

void GameFramework::DrawGameTime(HDC hdc) {
    // Initialize the font if it hasn't been initialized
    if (!hFont) {
        InitializeFont();
    }

    // 기존 폰트 저장
    HFONT hOldFont = (HFONT)SelectObject(m_hdcBackBuffer, hFont);

    RECT rect;
    rect.left = 625;  // 중앙 상단
    rect.top = 40;
    rect.right = rect.left + 200;
    rect.bottom = rect.top + 40;

    int minutes = gameTimeSeconds / 60;
    int seconds = gameTimeSeconds % 60;

    wchar_t gameTimeText[100];
    swprintf_s(gameTimeText, L"%02d:%02d", minutes, seconds);

    SetBkMode(m_hdcBackBuffer, TRANSPARENT);  // 배경 투명하게 설정
    SetTextColor(m_hdcBackBuffer, RGB(255, 255, 255));  // 흰색 글씨
    DrawText(m_hdcBackBuffer, gameTimeText, -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    SelectObject(m_hdcBackBuffer, hOldFont);
}

void GameFramework::DrawPauseMenu(HDC hdc) {
    if (!hFont) {
        InitializeFont();
    }

    HFONT hOldFont = (HFONT)SelectObject(m_hdcBackBuffer, hFont);

    SetBkMode(m_hdcBackBuffer, TRANSPARENT);

    RECT resumeRect = { 300, 200, 500, 250 };
    RECT mainMenuRect = { 300, 260, 500, 310 };
    RECT quitRect = { 300, 320, 500, 370 };

    SetTextColor(m_hdcBackBuffer, selectedMenuIndex == 0 ? RGB(255, 255, 0) : RGB(255, 255, 255));
    DrawText(m_hdcBackBuffer, L"Resume", -1, &resumeRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    SetTextColor(m_hdcBackBuffer, selectedMenuIndex == 1 ? RGB(255, 255, 0) : RGB(255, 255, 255));
    DrawText(m_hdcBackBuffer, L"Main Menu", -1, &mainMenuRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    SetTextColor(m_hdcBackBuffer, selectedMenuIndex == 2 ? RGB(255, 255, 0) : RGB(255, 255, 255));
    DrawText(m_hdcBackBuffer, L"Quit", -1, &quitRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    SelectObject(m_hdcBackBuffer, hOldFont);
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

    for (Item* item : items) {
        item->Draw(m_hdcBackBuffer, offsetX, offsetY);
    }

    currentGun->Draw(m_hdcBackBuffer, player->GetX() - offsetX, player->GetY() - offsetY, cursorPos.x, cursorPos.y, player->IsDirectionLeft());

    // Draw UI
    player->DrawHealth(m_hdcBackBuffer, offsetX, offsetY);
    player->DrawExperienceBar(m_hdcBackBuffer, clientRect);
    player->DrawInvincibilityIndicator(m_hdcBackBuffer, offsetX, offsetY);

    int cursorWidth = cursorImage.GetWidth();
    int cursorHeight = cursorImage.GetHeight();
    int clickWidth = clickImage.GetWidth();
    int clickHeight = clickImage.GetHeight();

    DrawBulletUI(m_hdcBackBuffer);
    DrawReloadingUI(m_hdcBackBuffer);
    DrawGameTime(m_hdcBackBuffer);

    if (isPaused) { // 정지화면 UI
        int panelWidth = pauseUIImage.GetWidth();
        int panelHeight = pauseUIImage.GetHeight();
        int x = (clientRect.right - panelWidth) / 2;
        int y = (clientRect.bottom - panelHeight) / 2;
        pauseUIImage.Draw(m_hdcBackBuffer, x, y);
        DrawPauseMenu(m_hdcBackBuffer);
    }

    if (showClickImage) {
        clickImage.Draw(m_hdcBackBuffer, cursorPos.x - clickWidth / 2, cursorPos.y - clickHeight / 2);
    }
    else {
        cursorImage.Draw(m_hdcBackBuffer, cursorPos.x - cursorWidth / 2, cursorPos.y - cursorHeight / 2);
    }

    BitBlt(hdc, 0, 0, clientRect.right, clientRect.bottom, m_hdcBackBuffer, 0, 0, SRCCOPY);
}

void GameFramework::OnMenuSelect() {
    switch (selectedMenuIndex) {
    case 0:
        TogglePause();
        break;
    case 1:
        // Main menu로 돌아가는 로직 추가
        break;
    case 2:
        PostMessage(m_hWnd, WM_CLOSE, 0, 0);
        break;
    }
}

// 키 다운 이벤트 처리
void GameFramework::OnKeyDown(WPARAM wParam) {
    switch (wParam) {
    case VK_UP:
        selectedMenuIndex = (selectedMenuIndex - 1 + 3) % 3; // 메뉴 항목 수에 따라 변경
        break;
    case VK_DOWN:
        selectedMenuIndex = (selectedMenuIndex + 1) % 3;
        break;
    case VK_RETURN:
        OnMenuSelect();
        break;
    }
}

// 키 업 이벤트 처리 (필요시 구현)
void GameFramework::OnKeyUp(WPARAM wParam) {
    // 필요한 경우 키 업 이벤트 처리
}

void GameFramework::OnKeyBoardProcessing(UINT iMessage, WPARAM wParam, LPARAM lParam) {
    if (isPaused) {
        switch (iMessage) {
        case WM_KEYDOWN:
            OnKeyDown(wParam);
            break;
        case WM_KEYUP:
            OnKeyUp(wParam);
            break;
        }
    }
    else {
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
    case WM_LBUTTONDOWN: {
        showClickImage = true;
        clickImageTimer = 0.2f;
        cursorPos.x = LOWORD(lParam);
        cursorPos.y = HIWORD(lParam);

        if (isPaused) {
            return;
        }
        else {
            FireBullet(player->GetX(), player->GetY(), cursorPos.x + camera->GetOffsetX(), cursorPos.y + camera->GetOffsetY());
        }
        break;
    }
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
