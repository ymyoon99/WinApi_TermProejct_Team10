#include "Player.h"
#include "GameFrameWork.h"
#include <cmath>
#include <iostream>

#define PlayerWidth 20.0f
#define PlayerHeight 25.0f

Player::Player(float x, float y, float speed, float animationSpeed)
    : x(x), y(y), speed(speed), animationSpeed(animationSpeed), currentFrame(0), frameTimeAccumulator(0.0f),
    moveLeft(false), moveRight(false), moveUp(false), moveDown(false), isMoving(false),
    boundWidth(0), boundHeight(0), directionLeft(false),
    level(1), experience(0), experienceToNextLevel(100), levelUpEffectTime(0.0f) {
    LoadImages();
}

Player::~Player() {
}

void Player::Update(float frameTime, const std::vector<Obstacle*>& obstacles) {
    frameTimeAccumulator += frameTime;
    levelUpEffectTime -= frameTime;

    if (frameTimeAccumulator >= animationSpeed) {
        if (isMoving) {
            currentFrame = (currentFrame + 1) % 4; // Run 애니메이션이 4 프레임이므로
        }
        else {
            currentFrame = (currentFrame + 1) % 5; // Idle 애니메이션이 5 프레임이므로
        }
        frameTimeAccumulator = 0.0f;
    }

    isMoving = false;
    if (moveLeft) { Move(-speed, 0, obstacles); isMoving = true; }
    if (moveRight) { Move(speed, 0, obstacles); isMoving = true; }
    if (moveUp) { Move(0, -speed, obstacles); isMoving = true; }
    if (moveDown) { Move(0, speed, obstacles); isMoving = true; }
}

void Player::Move(float dx, float dy, const std::vector<Obstacle*>& obstacles) {
    float newX = x + dx;
    float newY = y + dy;

    if (!CheckCollision(newX, newY, obstacles)) {
        x = newX;
        y = newY;
    }

    // 경계를 벗어나지 않도록 위치 제한
    if (x < 0) x = 0;
    if (y < 0) y = 0;
    if (x > boundWidth - PlayerWidth) x = boundWidth - PlayerWidth * 2;
    if (y > boundHeight - PlayerHeight) y = boundHeight - PlayerHeight * 2;
}

bool Player::CheckCollision(float newX, float newY, const std::vector<Obstacle*>& obstacles) const {
    for (const auto& obstacle : obstacles) {
        float ox = obstacle->GetX();
        float oy = obstacle->GetY();
        float ow = obstacle->GetWidth();
        float oh = obstacle->GetHeight();

        if (newX < ox + ow &&
            newX + PlayerWidth > ox &&
            newY < oy + oh &&
            newY + PlayerHeight > oy) {
            return true;
        }
    }
    return false;
}

void Player::SetBounds(float width, float height) {
    boundWidth = width;
    boundHeight = height;
}

float Player::GetX() const {
    return x;
}

float Player::GetY() const {
    return y;
}

void Player::LoadImages() {
    idleImages[0].Load(L"./resources/player/Idle_0.png");
    idleImages[1].Load(L"./resources/player/Idle_1.png");
    idleImages[2].Load(L"./resources/player/Idle_2.png");
    idleImages[3].Load(L"./resources/player/Idle_3.png");
    idleImages[4].Load(L"./resources/player/Idle_4.png");

    runImages[0].Load(L"./resources/player/Run_0.png");
    runImages[1].Load(L"./resources/player/Run_1.png");
    runImages[2].Load(L"./resources/player/Run_2.png");
    runImages[3].Load(L"./resources/player/Run_3.png");

    r_idleImages[0].Load(L"./resources/player/rIdle_0.png");
    r_idleImages[1].Load(L"./resources/player/rIdle_1.png");
    r_idleImages[2].Load(L"./resources/player/rIdle_2.png");
    r_idleImages[3].Load(L"./resources/player/rIdle_3.png");
    r_idleImages[4].Load(L"./resources/player/rIdle_4.png");

    r_runImages[0].Load(L"./resources/player/rRun_0.png");
    r_runImages[1].Load(L"./resources/player/rRun_1.png");
    r_runImages[2].Load(L"./resources/player/rRun_2.png");
    r_runImages[3].Load(L"./resources/player/rRun_3.png");

    levelUpEffectImages.resize(9);
    levelUpEffectImages[0].Load(L"./resources/effect/T_LevelUpFX_0.png");
    levelUpEffectImages[1].Load(L"./resources/effect/T_LevelUpFX_1.png");
    levelUpEffectImages[2].Load(L"./resources/effect/T_LevelUpFX_2.png");
    levelUpEffectImages[3].Load(L"./resources/effect/T_LevelUpFX_3.png");
    levelUpEffectImages[4].Load(L"./resources/effect/T_LevelUpFX_4.png");
    levelUpEffectImages[5].Load(L"./resources/effect/T_LevelUpFX_5.png");
    levelUpEffectImages[6].Load(L"./resources/effect/T_LevelUpFX_6.png");
    levelUpEffectImages[7].Load(L"./resources/effect/T_LevelUpFX_7.png");
    levelUpEffectImages[8].Load(L"./resources/effect/T_LevelUpFX_8.png");
}

void Player::DrawBoundingBox(HDC hdc, float offsetX, float offsetY) const {
    HPEN hPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
    HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);

    MoveToEx(hdc, static_cast<int>(x - offsetX), static_cast<int>(y - offsetY), NULL);
    LineTo(hdc, static_cast<int>(x - offsetX + PlayerWidth), static_cast<int>(y - offsetY));

    LineTo(hdc, static_cast<int>(x - offsetX + PlayerWidth), static_cast<int>(y - offsetY + PlayerHeight));

    LineTo(hdc, static_cast<int>(x - offsetX), static_cast<int>(y - offsetY + PlayerHeight));

    LineTo(hdc, static_cast<int>(x - offsetX), static_cast<int>(y - offsetY));

    SelectObject(hdc, hOldPen);
    DeleteObject(hPen);
}

void Player::Draw(HDC hdc, float offsetX, float offsetY) {
    if (isMoving) {
        if (directionLeft) {
            if (!r_runImages[currentFrame].IsNull()) {
                r_runImages[currentFrame].Draw(hdc, static_cast<int>(x - offsetX), static_cast<int>(y - offsetY));
            }
        }
        else {
            if (!runImages[currentFrame].IsNull()) {
                runImages[currentFrame].Draw(hdc, static_cast<int>(x - offsetX), static_cast<int>(y - offsetY));
            }
        }
    }
    else {
        if (directionLeft) {
            if (!r_idleImages[currentFrame].IsNull()) {
                r_idleImages[currentFrame].Draw(hdc, static_cast<int>(x - offsetX), static_cast<int>(y - offsetY));
            }
        }
        else {
            if (!idleImages[currentFrame].IsNull()) {
                idleImages[currentFrame].Draw(hdc, static_cast<int>(x - offsetX), static_cast<int>(y - offsetY));
            }
        }
    }

    // 레벨업 이펙트 그리기
    if (levelUpEffectTime > 0) {
        int frame = static_cast<int>((levelUpEffectDuration - levelUpEffectTime) / levelUpEffectDuration * levelUpEffectImages.size());
        if (frame >= 0 && frame < levelUpEffectImages.size()) {
            if (frame <= 5) {
                // 번개 이펙트 위치
                int drawX = static_cast<int>(x + 10.0f - offsetX - levelUpEffectImages[frame].GetWidth() / 2);
                int drawY = static_cast<int>(y + 12.5f - offsetY - levelUpEffectImages[frame].GetHeight());
                levelUpEffectImages[frame].Draw(hdc, drawX, drawY);
            }
            else {
                // 날개 이펙트 위치
                int drawX = static_cast<int>(x + 10.0f - offsetX - levelUpEffectImages[frame].GetWidth() / 2);
                int drawY = static_cast<int>(y + 12.5f - offsetY - levelUpEffectImages[frame].GetHeight() / 2);
                levelUpEffectImages[frame].Draw(hdc, drawX, drawY);
            }
        }
    }
}


void Player::SetDirectionLeft(bool isLeft) {
    directionLeft = isLeft;
}

bool Player::IsDirectionLeft() const {
    return directionLeft;
}

void Player::AddExperience(int amount) {
    experience += amount;
    while (experience >= experienceToNextLevel) {
        experience -= experienceToNextLevel;
        LevelUp();
    }
}

void Player::LevelUp() {
    level++;
    experienceToNextLevel = static_cast<int>(experienceToNextLevel * 1.3f);
    levelUpEffectTime = levelUpEffectDuration;

    // BrainMonster와 EyeMonster 제거 -> 수정필요
    extern std::vector<Enemy*> enemies;
    auto it = enemies.begin();
    while (it != enemies.end()) {
        if (dynamic_cast<BrainMonster*>(*it) || dynamic_cast<EyeMonster*>(*it)) {
            delete* it;
            it = enemies.erase(it);
        }
        else {
            ++it;
        }
    }
}

void Player::DrawExperienceBar(HDC hdc, RECT clientRect) {
    // 회색 배경 (전체 너비)
    RECT backgroundRect;
    backgroundRect.left = 0;
    backgroundRect.top = 0;
    backgroundRect.right = clientRect.right;
    backgroundRect.bottom = 20;

    HBRUSH grayBrush = CreateSolidBrush(RGB(128, 128, 128));
    FillRect(hdc, &backgroundRect, grayBrush);
    DeleteObject(grayBrush);

    // 연한 초록색 경험치 바 (현재 경험치에 비례한 너비)
    float percent = static_cast<float>(experience) / experienceToNextLevel;
    RECT experienceRect = backgroundRect;
    experienceRect.right = static_cast<LONG>(percent * clientRect.right);

    HBRUSH greenBrush = CreateSolidBrush(RGB(144, 238, 144));
    FillRect(hdc, &experienceRect, greenBrush);
    DeleteObject(greenBrush);
}