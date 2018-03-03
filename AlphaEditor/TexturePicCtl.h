/*
		负责给所有的文字纹理的鼠标消息
*/
#pragma once
#include <Windows.h>
#include <GdiPlus.h>
#include <list>
#include "WtlString.h"
using namespace std;
using namespace Gdiplus;

class CTexturePic;



typedef list<CTexturePic*>::iterator TextureIt;


class CTexturePicCtl
{
public:
	CTexturePicCtl(void);
	~CTexturePicCtl(void);
	virtual void Attach(CTexturePic* obv);  // 添加
	virtual void Detach(CTexturePic* obv);  // 移除
	virtual void Notify();  // 遍历容器调用

	void	SetHwnd(HWND hWnd);
	
	int Draw(Graphics & myGraphics);
	int OnMouseMove(int x,int y);
	int OnLButtonUp(int x,int y);
	int OnLButtonDown(int x,int y);
	
	int MoveTextture(int nOffx,int nOffy);

	CTexturePic* GetAt(UINT nId);
	CTexturePic* GetClickTexture();

	int SetClickTextureText(CString szText);
	TCHAR* GetClickTextureText();
	RECT	GetClickTexturePos();
private:
	//list<CTexturePic*>*		m_pTextlist;
	list<CTexturePic*>			m_pTextlist;
	CTexturePic*				m_pTextLastClick;
	HWND						m_hWnd;
};
