#pragma once
#include "WtlRect.h"
#include "WtlString.h"
#include <GdiPlus.h>
#include "BasicDebugInfo.h"
#include "TextureDef.h"
using namespace Gdiplus;
class CGdiPlusBitmapResource;


class CTexturePic
{
public:
	CTexturePic(void);
	~CTexturePic(void);
	// 初始化，加载位图
	void Init(UINT nId);
	void SetShowPos(int x,int y);
	RECT GetShowPos();
	int OffSetShowPos(int nOffx,int nOffy);
	CSize GetTextureSize();
	// 文件显示状态
	int SetState(TEXT_STATE state);
	TEXT_STATE GetState();
	// 纹理激活状态
	int SetActiveState(bool bActive=true);
	bool GetActiveState();
	// 绘制
	void Draw(Graphics &myGraphics);
	// 设置显示的位置
	int SetText(CString szText);
	TCHAR* GetText();
private:
	CGdiPlusBitmapResource*		m_pBmpRes;
	Gdiplus::Bitmap*	m_pBitmap;
	bool		m_bActive;		// 被点击了，一次只能有一个被点击
	bool		m_bVisual;
	double		m_Alpha;		// 透明度
	double		m_Angle;		// 旋转角度
	POINT		m_ptShowPos;	// 显示的左上角位置
	CString		m_strText;		// 输入的文字
	TEXT_STATE	m_state;
};
