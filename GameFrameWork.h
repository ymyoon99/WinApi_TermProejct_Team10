#pragma once

class GameFramework
{
public:
	GameFramework();
	~GameFramework();

	virtual void Reset();
	virtual void Clear();

	virtual void Create(HWND hWnd);
	virtual void Draw(HDC hdc);
	virtual void Update(const float frameTime = 0.16f);// 60fps
	virtual void OnKeyBoardProcessing(UINT iMessage, WPARAM wParam, LPARAM lParam);

	HWND	m_hWnd;
};

