#include "Player.h"

Player::Player(float x, float y, float speed) : x(x), y(y), speed(speed), currentFrame(0), frameTimeAccumulator(0.0f), moveLeft(false), moveRight(false), moveUp(false), moveDown(false), isMoving(false) {
    LoadImages();
}

Player::~Player() {
}

void Player::Update(float frameTime) {
    frameTimeAccumulator += frameTime;
    if (frameTimeAccumulator >= 0.1f) {
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
}