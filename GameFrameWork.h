#pragma once

#include <windows.h>
#include <atlimage.h> // CImage 사용을 위한 헤더
#include "Player.h"
#include "Camera.h"

class GameFramework {
public:
    GameFramework();
    ~GameFramework();

    void Create(HWND hWnd);
    void Update(float frameTime);
    void Draw(HDC hdc);
    void OnKeyBoardProcessing(UINT iMessage, WPARAM wParam, LPARAM lParam);
    void OnMouseProcessing(UINT iMessage, WPARAM wParam, LPARAM lParam);
    void Clear();

private:
    void InitializeDoubleBuffering(HDC hdc);
    void CleanupDoubleBuffering();

    HWND m_hWnd;
    HDC m_hdcBackBuffer;
    HBITMAP m_hBitmap;
    HBITMAP m_hOldBitmap;

    CImage mapImage;
    Player* player; // Player 객체 추가
    Camera* camera; // Camera 객체 추가

    CImage cursorImage;
    CImage clickImage;
    bool showClickImage;
    float clickImageTimer;
    POINT cursorPos;
};
