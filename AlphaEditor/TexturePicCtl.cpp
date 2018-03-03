#include "TexturePicCtl.h"
#include "TexturePic.h"

// list 排序
#include <algorithm>
#include <numeric>
#include <functional>
using namespace std;

#include "AutoLock.h"
using namespace ALOCK;
Mutex mutex_;


BOOL CmpFuncActive(CTexturePic* a,CTexturePic* b)
{
	if (a->GetActiveState())
		return false;
	return true;
}

CTexturePicCtl::CTexturePicCtl(void)
{
	//m_pTextlist = new list<CTexturePic*>; 
	m_hWnd = NULL;
	m_pTextLastClick = NULL;
}

CTexturePicCtl::~CTexturePicCtl(void)
{	
	std::list<CTexturePic*>::iterator it;
	for (it = m_pTextlist.begin();it!=m_pTextlist.end();it++)
	{
		delete (*it);
	}
	m_pTextlist.clear();
	ZY_TRACEA("%s 析构ok\n",__FUNCTION__);
}

void CTexturePicCtl::Attach(CTexturePic* obv)
{
	m_pTextlist.push_back(obv);
}

void CTexturePicCtl::Detach(CTexturePic* obv)
{
	if (obv)
		m_pTextlist.remove(obv);
}

void CTexturePicCtl::Notify() 
{ 
	list<CTexturePic*>::iterator it; 
	it = m_pTextlist.begin(); 
	for (;it != m_pTextlist.end();it++) 
	{ 
		//关于模板和 iterator 的用法 
		//(*it)->Update(this); 
	} 
} 

void CTexturePicCtl::SetHwnd(HWND hWnd)
{
	m_hWnd = hWnd;
}

CTexturePic *CTexturePicCtl::GetAt(UINT nId)
{
	if (nId > m_pTextlist.size())
		return NULL;
	TextureIt it = m_pTextlist.begin();
	int n =0;
	for (;it != m_pTextlist.end();it++) 
	{ 
		if (n == nId)
		{
			return (*it);
		}
	} 
	return NULL;
}

int CTexturePicCtl::Draw(Graphics & myGraphics)
{
	list<CTexturePic*>::iterator it; 
	it = m_pTextlist.begin(); 
	for (;it != m_pTextlist.end();it++) 
	{ 
		(*it)->Draw(myGraphics); 
	} 
	return true;
}

int CTexturePicCtl::OnMouseMove(int x,int y)
{
	CPoint pt(x,y);
	list<CTexturePic*>::iterator it; 
	it = m_pTextlist.begin(); 

	for (;it != m_pTextlist.end();it++) 
	{ 
		//if ((*it)->GetState() != TEXT_STATE_NORMAL)
		//	break;

		RECT rtPos = (*it)->GetShowPos();
		if (PtInRect(&rtPos,pt))
		{
			if ((*it)->GetState() == TEXT_STATE_NORMAL)
			{
				ZY_TRACEA("激活\n");
				(*it)->SetState(TEXT_STATE_FOCUS);
				::InvalidateRect(m_hWnd,&rtPos,true);
				//break; // 不要跳出，重置其它纹理的状态
			}
		}
		else
		{
			if ((*it)->GetState() != TEXT_STATE_NORMAL)
			{
				ZY_TRACEA("取消激活\n");
				(*it)->SetState(TEXT_STATE_NORMAL);
				::InvalidateRect(m_hWnd,&rtPos,true);
			}
		}
		//(*it)->Draw(myGraphics); 
	} 
	return true;
}

int CTexturePicCtl::OnLButtonUp(int x,int y)
{
	//CPoint pt(x,y);
	//list<CTexturePic*>::iterator it; 
	//it = m_pTextlist.begin(); 
	//for (;it != m_pTextlist.end();it++) 
	//{ 
	//	RECT rtPos = (*it)->GetShowPos();
	//	if (PtInRect(&rtPos,pt))
	//	{
	//		(*it)->SetState(TEXT_STATE_FOCUS);
	//	}
	//	else
	//	{
	//		(*it)->SetState(TEXT_STATE_NORMAL);
	//	}
	//} 
	return true;
}

int CTexturePicCtl::OnLButtonDown(int x,int y)
{
	bool bNeedUpdate = false;
	bool bHaveActive = false; // 每次只能激活一个纹理
	CPoint pt(x,y);
	list<CTexturePic*>::iterator it; 
	it = m_pTextlist.begin(); 
	for (;it != m_pTextlist.end();it++) 
	{ 
		RECT rtPos = (*it)->GetShowPos();
		if (PtInRect(&rtPos,pt))
		{
			//if (!(*it)->GetActiveState()&& !bHaveActive) 
			if (!bHaveActive) 
			{
				(*it)->SetActiveState(true);

				m_pTextLastClick = *it;

				bNeedUpdate = true;
				bHaveActive = true;
			}
		}
		else
		{
			(*it)->SetActiveState(false); // 鼠标不在纹理上，取消一切状态
			(*it)->SetState(TEXT_STATE_NORMAL);
		}
	} 
	// 将激活的纹理置顶
	if (bNeedUpdate)
		m_pTextlist.sort(CmpFuncActive);

	//if (bNeedUpdate)
	{
		::InvalidateRect(m_hWnd,NULL,true);
	}
	return true;
}

int CTexturePicCtl::MoveTextture(int nOffx,int nOffy)
{
	list<CTexturePic*>::iterator it; 
	it = m_pTextlist.begin(); 
	for (;it != m_pTextlist.end();it++) 
	{ 
		if ((*it)->GetActiveState())
		{
			(*it)->OffSetShowPos(nOffx,nOffy);
			::InvalidateRect(m_hWnd,NULL,true);
			break;
		}
	} 
	return 0;
}

CTexturePic*  CTexturePicCtl::GetClickTexture()
{
	return m_pTextLastClick;
}

int CTexturePicCtl::SetClickTextureText(CString szText)
{
	if (m_pTextLastClick)
	{
		m_pTextLastClick->SetText(szText);
		return 1;
	}
	return 0;
}

TCHAR * CTexturePicCtl::GetClickTextureText()
{
	if (m_pTextLastClick)
	{
		return m_pTextLastClick->GetText();
	}
	return NULL;
}

RECT CTexturePicCtl::GetClickTexturePos()
{
	RECT rt = { 0 };
	if (m_pTextLastClick)
	{
		rt = m_pTextLastClick->GetShowPos();
	}
	return rt;
}