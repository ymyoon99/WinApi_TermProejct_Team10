#include "stdafx.h"
#include "GameFramework.h"

GameFramework::GameFramework() : m_hdcBackBuffer(nullptr), m_hBitmap(nullptr), m_hOldBitmap(nullptr), player(nullptr), camera(nullptr) {
    Clear();
    mapImage.Load(L"./resources/background/background.png"); // 배경 이미지 로드
    camera = new Camera(800, 600); // 카메라 초기화
    player = new Player(100.0f, 100.0f, 0.2f); // 플레이어 초기화
}

GameFramework::~GameFramework() {
    CleanupDoubleBuffering();
    delete camera;
    delete player; // 플레이어 삭제
}

void GameFramework::Update(const float frameTime) {
    player->Update(frameTime); // 플레이어 업데이트
    camera->Update(player->GetX(), player->GetY()); // 플레이어 위치로 카메라 업데이트
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

    // 주인공의 현재 프레임을 백 버퍼에 그리기
    if (player->isMoving) {
        player->runImages[player->currentFrame].Draw(m_hdcBackBuffer, static_cast<int>(player->GetX() - offsetX), static_cast<int>(player->GetY() - offsetY));
    }
    else {
        player->idleImages[player->currentFrame].Draw(m_hdcBackBuffer, static_cast<int>(player->GetX() - offsetX), static_cast<int>(player->GetY() - offsetY));
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
        case VK_LEFT:
            player->moveLeft = true;
            break;
        case VK_RIGHT:
            player->moveRight = true;
            break;
        case VK_UP:
            player->moveUp = true;
            break;
        case VK_DOWN:
            player->moveDown = true;
            break;
        }
        break;

    case WM_KEYUP:
        switch (wParam) {
        case VK_LEFT:
            player->moveLeft = false;
            break;
        case VK_RIGHT:
            player->moveRight = false;
            break;
        case VK_UP:
            player->moveUp = false;
            break;
        case VK_DOWN:
            player->moveDown = false;
            break;
        }
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

void GameFramework::Clear() {
    // Clear 메서드 구현
}

void GameFramework::Create(HWND hWnd) {
    m_hWnd = hWnd;
}
