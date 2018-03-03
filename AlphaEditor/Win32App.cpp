#include "Win32App.h"
#include <tchar.h>
#include <GdiPlus.h>
#include "WtlCDC.h"
#include "WtlString.h"
#include "CGdiPlusBitmap.h"
#include "resource.h"
#include "TexturePicCtl.h"
#include "TexturePic.h"
#include "Win32Editor.h"


using namespace Gdiplus;

#define WINDOW_CLASS    _T("Win32")
#define WINDOW_NAME     _T("Win32App")
#define WINDOW_WIDTH    640
#define WINDOW_HEIGHT   480

#ifndef nullptr
#define nullptr NULL;
#endif

CWin32App	*g_pApp = NULL;

#define TEXTBOX_MAXLENGTH   1024 // 文本框中文本的最大长度
TCHAR _String[TEXTBOX_MAXLENGTH + 1] = _T("");     // 文本
int    _StringPosition = ::_tcslen(_String);        // 光标插入点所在的位置


// 更新窗口
void CWin32App::_UpdateWindow(HWND hWnd)
{
	if (hWnd == NULL)
		m_hWnd = hWnd;
	RECT rect;
	::GetClientRect(hWnd, &rect);
	::InvalidateRect(hWnd, &rect, TRUE);
	::UpdateWindow(hWnd);
}

// 设置光标位置
void CWin32App::_SetCaretPos(HWND hWnd)
{
	if (hWnd == NULL)
		m_hWnd = hWnd;
	HDC hDC = ::GetDC(hWnd);

	SIZE size;
	::GetTextExtentPoint(hDC, _String, _StringPosition, &size);
	::SetCaretPos(4 + size.cx, 3); // 这里决定了输入法的显示位置

	::ReleaseDC(hWnd, hDC);
}

// 绘制文本
void CWin32App::_DrawText(HDC hDC)
{
	int len = ::_tcslen(_String);
	::SetBkMode(hDC, TRANSPARENT);
	::TextOut(hDC, 4, 2, _String, len);
	m_pTextCtl->SetClickTextureText(_String);
	//::InvalidateRect(m_hWnd, &(m_pTextCtl->GetClickTexturePos()), false);
}

LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return g_pApp->WinProc(hWnd,msg,wParam,lParam);
}

CWin32App::CWin32App(void)
{
	m_hWnd = NULL;
	g_pApp = this;
	m_pEditor = NULL;
}

CWin32App::~CWin32App(void)
{
	ZY_TRACEA("%s 析构ok\n",__FUNCTION__);
}

BOOL CWin32App::Run()
{
	// Show the window
	ShowWindow(m_hWnd, SW_SHOWDEFAULT);
	UpdateWindow(m_hWnd);

	// Enter the message loop
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	while(msg.message != WM_QUIT)
	{
		if(PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			Sleep(5);
			RenderScene();
		}
	}
	Shutdown();
	return 0;
}

void CWin32App::Shutdown()
{
	delete m_pTextCtl;
}

void CWin32App::RenderScene()
{
	
}

BOOL CWin32App::Create(HINSTANCE hinst)
{
	m_hInst = hinst;
	// Register the window class
	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L,
		GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
		WINDOW_CLASS, NULL };
	RegisterClassEx(&wc);

	// Create the application's window
	m_hWnd = CreateWindow(WINDOW_CLASS, WINDOW_NAME, WS_OVERLAPPEDWINDOW,
		100, 100, WINDOW_WIDTH, WINDOW_HEIGHT, GetDesktopWindow(),
		NULL, wc.hInstance, NULL);

	BOOL bRet = IsWindow(m_hWnd);
	if (bRet)
		Initialize();
	return bRet;
}

BOOL  CWin32App::Initialize()
{
	m_pTextCtl = new CTexturePicCtl;
	m_pTextCtl->SetHwnd(m_hWnd);
	int i=0;
	for (;i< 2 ;i++)
	{
		CTexturePic *pPic = new CTexturePic();

		pPic->Init(IDB_PNG2+i);
		pPic->SetShowPos(50+300*i,100);
		m_pTextCtl->Attach(pPic);
	}

	//m_pEditor = new CWin32Editor;
	//bool bRet = m_pEditor->Create(m_hWnd,m_hInst);

	return TRUE;
}
 

LRESULT WINAPI CWin32App::WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	BOOL bHandle = false;
	switch(msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
		break;
	case WM_PAINT:
		OnPaint(wParam,lParam);
		break;
	case WM_ACTIVATE:                // 当窗口被激活时，将焦点设置在文本框上
		if (m_pEditor && m_pEditor->IsOk())
		{
			::SetFocus(m_pEditor->GetHwnd());
		}
		break;
#ifndef ZY_CLOSE_INPUT
	case WM_SETFOCUS:
		{
			::CreateCaret(m_hWnd, (HBITMAP)NULL, 1, 20-5);     // 创建光标
			_SetCaretPos(m_hWnd);                            // 设置光标位置
			::ShowCaret(m_hWnd);                   // 显示光标
		}
		break;
	case WM_CHAR:
		OnChar(wParam,lParam,bHandle);
		break;
#endif // !ZY_CLOSE_INPUT
	case WM_ERASEBKGND:
			return true;
		break;
	case  WM_LBUTTONDOWN:
		OnLButtonDown(wParam,lParam,bHandle);
		break;
	case WM_LBUTTONUP:
		OnLButtonUp(wParam,lParam,bHandle);
		break;
	case WM_MOUSEMOVE:
		OnMouseMove(wParam,lParam,bHandle);
		break;
#ifndef ZY_CLOSE_INPUT
	case WM_KEYDOWN: 
	{  // 键按下消息
		TCHAR code = (TCHAR)wParam;
		switch (code)
		{
		case VK_LEFT: // 左光标键
			if (_StringPosition > 0)
				_StringPosition--;
			break;

		case VK_RIGHT:     // 右光标键
			if (_StringPosition < (int)::_tcslen(_String))
				_StringPosition++;
			break;

		case VK_HOME: // HOME 键
			_StringPosition = 0;
			break;

		case VK_END:  // END 键
			_StringPosition = ::_tcslen(_String);
			break;
		case VK_BACK: // 退格键
			if (_StringPosition > 0)
			{
				::MoveMemory(_String + _StringPosition - 1, _String + _StringPosition, (::_tcslen(_String) - _StringPosition + 1) * sizeof(TCHAR));
				_StringPosition--;
				_UpdateWindow(hWnd);
			}
			break;

		case VK_DELETE: {  // 删除键
			int len = ::_tcslen(_String);
			if (_StringPosition < len)
			{
				::MoveMemory(_String + _StringPosition, _String + _StringPosition + 1, (::_tcslen(_String) - _StringPosition + 1) * sizeof(TCHAR));
				_UpdateWindow(hWnd);
			}

		} break;
		default:
			break;
		}
	}
	_SetCaretPos(hWnd); // 更新光标位置
	break;
#else
	case WM_KEYUP:
		if(wParam == VK_ESCAPE) PostQuitMessage(0);
		break;
#endif  //! ZY_CLOSE_INPUT
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}


TCHAR buffer[1024];        // used to print strings


LRESULT CWin32App::OnChar(WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	TCHAR code = (TCHAR)wParam;
	int len = ::_tcslen(_String);
	if(code < (TCHAR)' ' || len >= TEXTBOX_MAXLENGTH)
		return 0;

	::MoveMemory(_String + _StringPosition + 1, _String + _StringPosition, (len - _StringPosition + 1) * sizeof(TCHAR));
	_String[_StringPosition ++] = code;
	_UpdateWindow(m_hWnd);
	_SetCaretPos(m_hWnd);

#ifdef ZY_TEST

#endif // ZY_TEST

	return 0;
}

LRESULT CWin32App::OnLButtonDown(WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SetCapture(m_hWnd);
	m_bMousing  = true;
	int x = LOWORD(lParam);
	int y = HIWORD(lParam);
	m_ptLastMousePosit.x = m_ptCurrentMousePosit.x = x;
	m_ptLastMousePosit.y = m_ptCurrentMousePosit.y = y;

	m_pTextCtl->OnLButtonDown(x,y);


	CString pBuf = m_pTextCtl->GetClickTextureText();
	_tcscpy(_String, pBuf.GetBuffer(pBuf.GetLength()));
	_StringPosition = pBuf.GetLength();
	_SetCaretPos(m_hWnd);
	
	return 0;
}

LRESULT CWin32App::OnLButtonUp(WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	ReleaseCapture(); 
	m_bMousing = false;
	int x = LOWORD(lParam);
	int y = HIWORD(lParam);
	m_pTextCtl->OnLButtonUp(x,y);
	return 0;
}

LRESULT CWin32App::OnMouseMove(WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	int x = LOWORD(lParam);
	int y = HIWORD(lParam);

	m_ptCurrentMousePosit.x = x;  
	m_ptCurrentMousePosit.y = y;  

	m_pTextCtl->OnMouseMove(x,y);

	if (m_bMousing)
	{
		int nOffx = m_ptCurrentMousePosit.x - m_ptLastMousePosit.x;  
		int nOffY = m_ptCurrentMousePosit.y - m_ptLastMousePosit.y;  
		// 移动操作
			m_pTextCtl->MoveTextture(nOffx,nOffY);
	}
	m_ptLastMousePosit.x = m_ptCurrentMousePosit.x;  
	m_ptLastMousePosit.y = m_ptCurrentMousePosit.y;  

	return 0;
}

HRESULT CWin32App::OnPaint(WPARAM wParam, LPARAM lParam)
{
	CPaintDC dc(m_hWnd);
	CRect rtClient;
	GetClientRect(m_hWnd,&rtClient);
	
	_DrawText(dc);                      // 绘制文本

	Bitmap* pMemoryBMap = new Bitmap(rtClient.Width(), rtClient.Height());
	Graphics* pCanvas = Graphics::FromImage(pMemoryBMap);

	//pCanvas->SetSmoothingMode(SmoothingModeAntiAlias); // 会影响边框的绘制

	//pCanvas->Clear(Color().White);
	//SolidBrush b(Color(255, 0, 0, 0));
	//pCanvas->FillRectangle(&b, 0, 0, rtClient.Width(), rtClient.Height());

	Image image(L"sea.jpg");
	int nBkHeight = image.GetHeight();
	int nBkWidth =  image.GetWidth();
	pCanvas->DrawImage(&image,0,0,nBkWidth,nBkHeight);



	m_pTextCtl->Draw(*pCanvas);


	Graphics* pGraphics = new Graphics(dc);
	pGraphics->DrawImage(pMemoryBMap, 0, 0);
	CachedBitmap* m_pCachedBitmap = new CachedBitmap(pMemoryBMap, pGraphics);
	pGraphics->DrawCachedBitmap(m_pCachedBitmap, 0, 0);

	delete pMemoryBMap;
	delete pCanvas;
	delete m_pCachedBitmap;

	RECT rect ={0};
	::GetClientRect(m_hWnd, &rect);        // 获取客户区的尺寸
	::DrawEdge(dc, &rect, EDGE_SUNKEN, BF_RECT);  // 绘制边框，EDGE_SUNKEN表示绘制样式为内嵌样式，BF_RECT表示绘制矩形边框
	

	return 0;
}
