#pragma once
#include <Windows.h>
#include "WtlRect.h"


class CTexturePicCtl;
class CWin32Editor;


class CWin32App
{
public:
	CWin32App(void);
	~CWin32App(void);
public:
	BOOL Create(HINSTANCE hinst);
	BOOL Run();
	BOOL Initialize();
	void RenderScene();
	void Shutdown();

	LRESULT WINAPI WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT OnLButtonDown(WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnLButtonUp(WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnMouseMove(WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnChar(WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	HRESULT OnPaint(WPARAM wParam, LPARAM lParam);
private:
	void _UpdateWindow(HWND hWnd =NULL);
	void _SetCaretPos(HWND hWnd =NULL);
	void _DrawText(HDC hDC);

private:
	BOOL		m_bMousing;
	CPoint		m_ptLastMousePosit;
	CPoint		m_ptCurrentMousePosit;
	HWND		m_hWnd;
	HINSTANCE	m_hInst;
	CTexturePicCtl*		m_pTextCtl;
	CWin32Editor*		m_pEditor;
};
