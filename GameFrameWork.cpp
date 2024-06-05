#include "stdafx.h"
#include "GameFramework.h"


GameFramework::GameFramework()
{
	Clear();
}

GameFramework::~GameFramework()
{
}

void GameFramework::Reset()
{
}

void GameFramework::Clear()
{
}

void GameFramework::Create(HWND hWnd)
{
	m_hWnd = hWnd;
}

void GameFramework::Draw(HDC hdc)
{

}

void GameFramework::Update(const float frameTime)
{
}

void GameFramework::OnKeyBoardProcessing(UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
		case WM_KEYDOWN:
		{
			if (wParam == 'Q')
			{
				SendMessage(m_hWnd, WM_DESTROY, 0, 0);
				return;
			}
		}
		break;
		

		case WM_KEYUP:
		{

		}
		break;


	}

}
