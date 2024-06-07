#include "Player.h"

#define PlayerWidth 20.0f
#define PlayerHeight 25.0f

Player::Player(float x, float y, float speed, float animationSpeed) : x(x), y(y), speed(speed), animationSpeed(animationSpeed), currentFrame(0), frameTimeAccumulator(0.0f), moveLeft(false), moveRight(false), moveUp(false), moveDown(false), isMoving(false), boundWidth(0), boundHeight(0), directionLeft(false) {
    LoadImages();
}

Player::~Player() {
}

void Player::Update(float frameTime) {
    frameTimeAccumulator += frameTime;
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
    if (moveLeft) { Move(-speed, 0); isMoving = true; }
    if (moveRight) { Move(speed, 0); isMoving = true; }
    if (moveUp) { Move(0, -speed); isMoving = true; }
    if (moveDown) { Move(0, speed); isMoving = true; }
}

void Player::Move(float dx, float dy) {
    x += dx;
    y += dy;

    // 경계를 벗어나지 않도록 위치 제한
    if (x < 0) x = 0;
    if (y < 0) y = 0;
    if (x > boundWidth - PlayerWidth) x = boundWidth - PlayerWidth * 2;
    if (y > boundHeight - PlayerHeight) y = boundHeight - PlayerHeight * 2;
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
    // Idle 이미지 로드
    idleImages[0].Load(L"./resources/player/Idle_0.png");
    idleImages[1].Load(L"./resources/player/Idle_1.png");
    idleImages[2].Load(L"./resources/player/Idle_2.png");
    idleImages[3].Load(L"./resources/player/Idle_3.png");
    idleImages[4].Load(L"./resources/player/Idle_4.png");

    // Run 이미지 로드
    runImages[0].Load(L"./resources/player/Run_0.png");
    runImages[1].Load(L"./resources/player/Run_1.png");
    runImages[2].Load(L"./resources/player/Run_2.png");
    runImages[3].Load(L"./resources/player/Run_3.png");

    // Idle 이미지 로드
    r_idleImages[0].Load(L"./resources/player/rIdle_0.png");
    r_idleImages[1].Load(L"./resources/player/rIdle_1.png");
    r_idleImages[2].Load(L"./resources/player/rIdle_2.png");
    r_idleImages[3].Load(L"./resources/player/rIdle_3.png");
    r_idleImages[4].Load(L"./resources/player/rIdle_4.png");

    // Run 이미지 로드
    r_runImages[0].Load(L"./resources/player/rRun_0.png");
    r_runImages[1].Load(L"./resources/player/rRun_1.png");
    r_runImages[2].Load(L"./resources/player/rRun_2.png");
    r_runImages[3].Load(L"./resources/player/rRun_3.png");
}

void Player::DrawBoundingBox(HDC hdc, float offsetX, float offsetY) const {
    HPEN hPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
    HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);

    // Top line
    MoveToEx(hdc, static_cast<int>(x - offsetX), static_cast<int>(y - offsetY), NULL);
    LineTo(hdc, static_cast<int>(x - offsetX + PlayerWidth), static_cast<int>(y - offsetY));

    // Right line
    LineTo(hdc, static_cast<int>(x - offsetX + PlayerWidth), static_cast<int>(y - offsetY + PlayerHeight));

    // Bottom line
    LineTo(hdc, static_cast<int>(x - offsetX), static_cast<int>(y - offsetY + PlayerHeight));

    // Left line
    LineTo(hdc, static_cast<int>(x - offsetX), static_cast<int>(y - offsetY));

    SelectObject(hdc, hOldPen);
    DeleteObject(hPen);
}

void Player::Draw(HDC hdc, float offsetX, float offsetY) {
    // 플레이어 이동 Draw
    if (isMoving) {
        if (directionLeft) {
            if (r_runImages[currentFrame].IsNull() == FALSE) {
                r_runImages[currentFrame].Draw(hdc, static_cast<int>(x - offsetX), static_cast<int>(y - offsetY));
            }
        }
        else {
            if (runImages[currentFrame].IsNull() == FALSE) {
                runImages[currentFrame].Draw(hdc, static_cast<int>(x - offsetX), static_cast<int>(y - offsetY));
            }
        }
    }
    else {
        if (directionLeft) {
            if (r_idleImages[currentFrame].IsNull() == FALSE) {
                r_idleImages[currentFrame].Draw(hdc, static_cast<int>(x - offsetX), static_cast<int>(y - offsetY));
            }
        }
        else {
            if (idleImages[currentFrame].IsNull() == FALSE) {
                idleImages[currentFrame].Draw(hdc, static_cast<int>(x - offsetX), static_cast<int>(y - offsetY));
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
