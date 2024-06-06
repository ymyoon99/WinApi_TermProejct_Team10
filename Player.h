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

    float x, y;
    float speed;
    int currentFrame;
    float frameTimeAccumulator;
    bool moveLeft, moveRight, moveUp, moveDown;
    bool isMoving;

    // 플레이어 이미지 로드 메서드들
    void LoadImages();
    // 플레이어 상태에 따른 이미지 배열
    CImage idleImages[5];
    CImage runImages[4];
};
