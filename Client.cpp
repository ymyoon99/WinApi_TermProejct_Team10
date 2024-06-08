#include "stdafx.h"
#include "Client.h"
#include "GameFramework.h"

#define CLIENT_WIDTH    800
#define CLIENT_HEIGHT   600

#define GAME_TIMER      1    
#define GAME_FRAME_RATE 16

HINSTANCE hInst;
LPCTSTR lpszClass = L"Winapi Term Project";
LPCTSTR lpszWindowName = L"Winapi Term Project";

static GameFramework gameframework;

ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    MyRegisterClass(hInstance);

    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    MSG msg;
    DWORD frameStartTime{};
    DWORD frameEndTime{};
    const DWORD frameDuration = 1000 / 60;
    float frameTime{};

    while (true)
    {
        if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT) break;
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
        }
        else
        {
            frameStartTime = GetTickCount();  // 프레임 시작 시간

            // 게임 프레임 업데이트
            frameTime = 1.0f / 60.0f;  // 약 60FPS로 가정
            gameframework.Update(frameTime);
            InvalidateRect(GetActiveWindow(), NULL, FALSE);

            // 프레임이 너무 빨리 그려지는 경우 대기
            frameEndTime = GetTickCount();
            DWORD frameElapsedTime = frameEndTime - frameStartTime;
            if (frameElapsedTime < GAME_FRAME_RATE)
            {
                Sleep(GAME_FRAME_RATE - frameElapsedTime);
            }
        }
    }

    gameframework.Clear();

    return (int)msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = lpszClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance;

    HWND hWnd = CreateWindowW(
        lpszClass,
        lpszWindowName,
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, // 이 스타일로 윈도우 크기 조절 비활성화
        CW_USEDEFAULT,
        0,
        CLIENT_WIDTH,
        CLIENT_HEIGHT,
        nullptr,
        nullptr,
        hInstance,
        nullptr
    );

    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    // 클라이언트 영역에 들어오면 커서를 숨기고, 나가면 보이도록 설정
    TRACKMOUSEEVENT tme;
    tme.cbSize = sizeof(tme);
    tme.dwFlags = TME_LEAVE;
    tme.hwndTrack = hWnd;
    TrackMouseEvent(&tme);

    return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
        gameframework.Create(hWnd);
        SetTimer(hWnd, GAME_TIMER, GAME_FRAME_RATE, NULL);
        break;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        gameframework.Draw(hdc);
        EndPaint(hWnd, &ps);
    }
    break;

    case WM_TIMER:
        gameframework.Update(0.016f); // 약 60FPS로 가정 (1초 / 60프레임)
        InvalidateRect(hWnd, NULL, FALSE);
        break;

    case WM_KEYDOWN:
    case WM_KEYUP:
        gameframework.OnKeyBoardProcessing(message, wParam, lParam);
        break;

    case WM_LBUTTONDOWN:
    case WM_MOUSEMOVE:
        gameframework.OnMouseProcessing(message, wParam, lParam);
        ShowCursor(FALSE); // 클라이언트 영역에서는 커서를 숨김
        break;

    case WM_MOUSELEAVE:
        ShowCursor(TRUE); // 클라이언트 영역을 벗어나면 커서를 보임
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
