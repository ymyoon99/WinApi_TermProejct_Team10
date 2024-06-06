#pragma once

#include <atlimage.h> // CImage 사용을 위한 헤더

class Player {
public:
    Player(float x, float y, float speed, float animationSpeed);
    ~Player();

    void Update(float frameTime);
    void Move(float dx, float dy);

    float GetX() const;
    float GetY() const;

    // 플레이어의 경계를 설정하는 메서드 추가
    void SetBounds(float width, float height);
    void DrawBoundingBox(HDC hdc, float offsetX, float offsetY) const;

    // 플레이어 이미지 로드 메서드들
    void LoadImages();
    void Draw(HDC hdc, float offsetX, float offsetY);

    // 방향을 설정하기 위한 메서드
    void SetDirectionLeft(bool isLeft);
    bool IsDirectionLeft() const;

    float x, y;
    float speed;
    float animationSpeed;
    int currentFrame;
    float frameTimeAccumulator;
    bool moveLeft, moveRight, moveUp, moveDown;
    bool isMoving;
    bool directionLeft;

    float boundWidth, boundHeight;

    // 이미지 로드할 배열 선언
    CImage idleImages[5];
    CImage runImages[4];
    CImage r_idleImages[5];
    CImage r_runImages[4];
};
