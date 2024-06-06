#pragma once
#include <atlimage.h> // CImage 사용을 위한 헤더

class Player {
public:
    Player(float x, float y, float speed);
    ~Player();

    void Update(float frameTime);
    void Move(float dx, float dy);

    float GetX() const;
    float GetY() const;

    void SetBounds(float width, float height); // 경계를 설정하는 메서드 추가
    void DrawBoundingBox(HDC hdc, float offsetX, float offsetY) const;

    float x, y;
    float speed;
    int currentFrame;
    float frameTimeAccumulator;
    bool moveLeft, moveRight, moveUp, moveDown;
    bool isMoving;

    float boundWidth, boundHeight; // 경계를 위한 변수

    // 플레이어 이미지 로드 메서드들
    void LoadImages();
    // 플레이어 상태에 따른 이미지 배열
    CImage idleImages[5];
    CImage runImages[4];
};
