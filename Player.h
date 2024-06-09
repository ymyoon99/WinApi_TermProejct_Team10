#pragma once

#include <atlimage.h>
#include <vector>
#include "Obstacle.h"

class Player {
public:
    Player(float x, float y, float speed, float animationSpeed);
    ~Player();

    void Update(float frameTime, const std::vector<Obstacle*>& obstacles);
    void Move(float dx, float dy, const std::vector<Obstacle*>& obstacles);

    float GetX() const;
    float GetY() const;

    void SetBounds(float width, float height);
    void DrawBoundingBox(HDC hdc, float offsetX, float offsetY) const;

    void LoadImages();
    void Draw(HDC hdc, float offsetX, float offsetY);

    void SetDirectionLeft(bool isLeft);
    bool IsDirectionLeft() const;

    void AddExperience(int amount);
    void DrawExperienceBar(HDC hdc, RECT clientRect);

    void LevelUp();
    bool CheckCollision(float newX, float newY, const std::vector<Obstacle*>& obstacles) const;

    float x, y;
    float speed;
    float animationSpeed;
    int currentFrame;
    float frameTimeAccumulator;
    bool moveLeft, moveRight, moveUp, moveDown;
    bool isMoving;
    bool directionLeft;

    float boundWidth, boundHeight;

    int level;
    int experience;
    int experienceToNextLevel;

    CImage idleImages[5];
    CImage runImages[4];
    CImage r_idleImages[5];
    CImage r_runImages[4];

    std::vector<CImage> levelUpEffectImages;
    float levelUpEffectTime;
    const float levelUpEffectDuration = 1.5f;
};
