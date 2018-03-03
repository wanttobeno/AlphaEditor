#include "Win32Editor.h"

#define  ZY_EDITOR_DEFAULE 128
#define TEXTBOX_WIDTH       300      // 文本框宽度
#define TEXTBOX_HEIGHT      20       // 文本框高度
#define  _TextBoxClass  _T("ZY_EDITOR")

#define TEXTBOX_MAXLENGTH   1024 // 文本框中文本的最大长度

TCHAR	MyString[TEXTBOX_MAXLENGTH + 1] = _T("");     // 文本
int    MyStringPosition = ::_tcslen(MyString);        // 光标插入点所在的位置

static CWin32Editor*		g_pEidtor = NULL;

LRESULT WINAPI TextMsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return g_pEidtor->TextBoxWndProc(hWnd, msg, wParam, lParam);
}

CWin32Editor::CWin32Editor(void)
{
	g_pEidtor = this;
	m_pText = (TCHAR*)malloc(ZY_EDITOR_DEFAULE*sizeof(TCHAR));
	ZeroMemory(m_pText, ZY_EDITOR_DEFAULE*sizeof(TCHAR));
	m_bInitSuc = false;
}

CWin32Editor::~CWin32Editor(void)
{
	if (m_pText)
	{
		free(m_pText);
		m_pText= NULL;
	}
}

// 注册应用程序窗口类
ATOM CWin32Editor::RegClass(HINSTANCE hInst)
{
	WNDCLASSEX wc;
	::ZeroMemory(&wc, sizeof(wc));                 // 作为一步清空，是为了让未赋值的字段的默认值为（或NULL）

	wc.cbSize     = sizeof(wc);
	wc.style      = CS_HREDRAW | CS_VREDRAW;  // 指定当窗口横向和纵向的尺寸发生变化时都会重绘窗口
	wc.hInstance  = hInst;
	wc.hbrBackground = (HBRUSH)( COLOR_APPWORKSPACE + 1);  // 指定主窗口背景为“工作区域”系统颜色
	wc.lpszClassName = _TextBoxClass;          // 此为要注册的类名，创建窗口时要以此类名为标识符
	wc.lpfnWndProc = TextMsgProc;                      // 此为处理窗口消息的函数

	return ::RegisterClassEx(&wc);                 // 调用API函数注册窗口类
}

bool CWin32Editor::Create(HWND hParentWnd,HINSTANCE hInst)
{
	bool bRet = false;
	if(RegClass(hInst))
	{
		// 之下代码是为了让文本框显示在父窗口中央，而计算位置
		RECT parentWndRect;
		::GetClientRect(hParentWnd, &parentWndRect);  // 获取父窗口客户区的位置
		int left = (parentWndRect.right - TEXTBOX_WIDTH) / 2, top = (parentWndRect.bottom - TEXTBOX_HEIGHT) / 2;

		// 创建文本框
		HWND hWnd = ::CreateWindow(_TextBoxClass, NULL, WS_CHILDWINDOW | WS_VISIBLE,
			left, top, TEXTBOX_WIDTH, TEXTBOX_HEIGHT,
			hParentWnd, NULL, hInst, NULL);
		DWORD dwErr = ::GetLastError();
		bRet = ::IsWindow(hWnd);
	}
	m_bInitSuc = bRet;
	return bRet;
}

bool CWin32Editor::IsOk()
{
	return m_bInitSuc;
}

HWND CWin32Editor::GetHwnd()
{
	return m_hWnd;
}

// 更新窗口
void CWin32Editor::MyUpdateWindow(HWND hWnd)
{
	RECT rect;
	::GetClientRect(hWnd, &rect);
	::InvalidateRect(hWnd, &rect, TRUE);
	::UpdateWindow(hWnd);
}

// 绘制文本
void CWin32Editor::MyDrawText(HDC hDC)
{
	int len = ::_tcslen(MyString);
	::TextOut(hDC, 4, 2, MyString, len);
}

// 设置光标位置
void CWin32Editor::MySetCaretPos(HWND hWnd)
{
	HDC hDC = ::GetDC(hWnd);

	SIZE size;
	::GetTextExtentPoint(hDC, MyString, MyStringPosition, &size);
	::SetCaretPos(4 + size.cx, 3);

	::ReleaseDC(hWnd, hDC);
}

LRESULT WINAPI CWin32Editor::TextBoxWndProc(HWND hWnd,UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_PAINT: 
	{  // 绘制这里之所以加一对大括号，是为了让之下定义的变量局部化

		static PAINTSTRUCT ps;
		static RECT rect;
		HDC hDC = ::BeginPaint(hWnd, &ps);  // 开始绘制操作

		::GetClientRect(hWnd, &rect);        // 获取客户区的尺寸
		::DrawEdge(hDC, &rect, EDGE_SUNKEN, BF_RECT);  // 绘制边框，EDGE_SUNKEN表示绘制样式为内嵌样式，BF_RECT表示绘制矩形边框
		MyDrawText(hDC);                      // 绘制文本
		::EndPaint(hWnd, &ps);               // 结束绘制操作
	} 
	break;

	case WM_SETFOCUS:
	{    // 获得焦点
		::CreateCaret(hWnd, (HBITMAP)NULL, 1, TEXTBOX_HEIGHT-5);     // 创建光标
		MySetCaretPos(hWnd);                            // 设置光标位置
		::ShowCaret(hWnd);                   // 显示光标
	} 
	break;
	case WM_KILLFOCUS: // 失去焦点
		::HideCaret(hWnd);                   // 隐藏光标
		::DestroyCaret();                    // 销毁光标
		break;

	case WM_SETCURSOR: 
	{  // 设置光标形状
		static HCURSOR hCursor = ::LoadCursor(NULL, IDC_IBEAM);
		::SetCursor(hCursor);	   
	} break;

	case WM_CHAR: 
	{    // 字符消息
		TCHAR code = (TCHAR)wParam;
		int len = ::_tcslen(MyString);
		if(code < (TCHAR)' ' || len >= TEXTBOX_MAXLENGTH)
			return 0;

		::MoveMemory(MyString + MyStringPosition + 1, MyString + MyStringPosition, (len - MyStringPosition + 1) * sizeof(TCHAR));
		MyString[MyStringPosition ++] = code;

		MyUpdateWindow(hWnd);
		MySetCaretPos(hWnd);

	} 
	break;

	case WM_KEYDOWN: 
	{  // 键按下消息
		OnKeyDown(hWnd,wParam, lParam);
	} 
	break;

	case WM_LBUTTONDOWN: 
	{  // 鼠标单击，设置光标位置
		int x = LOWORD(lParam);
		HDC hDc = ::GetDC(hWnd);

		int strLen = ::_tcslen(MyString), strPos = 0;
		SIZE size;

		for (strPos=0; strPos<strLen; strPos++)
		{
			::GetTextExtentPoint(hDc, MyString, strPos, &size);

			if(size.cx + 4 >= x)
				break;
		}
		// 字符的长度
		MyStringPosition = strPos;
		::GetTextExtentPoint(hDc, MyString, strPos, &size);
		::SetCaretPos(size.cx + 4, 3);

		::ReleaseDC(hWnd, hDc);		
	}
	break;

	default:
		return ::DefWindowProc(hWnd, message, wParam, lParam);
	}

	return (LRESULT)0;
}

void CWin32Editor::OnKeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	TCHAR code = (TCHAR)wParam;

	switch (code)
	{
	case VK_LEFT: // 左光标键
		if (MyStringPosition > 0)
			MyStringPosition--;
		break;

	case VK_RIGHT:     // 右光标键
		if (MyStringPosition < (int)::_tcslen(MyString))
			MyStringPosition++;
		break;

	case VK_HOME: // HOME 键
		MyStringPosition = 0;
		break;

	case VK_END:  // END 键
		MyStringPosition = ::_tcslen(MyString);
		break;

	case VK_BACK: // 退格键
		if (MyStringPosition > 0)
		{
			::MoveMemory(MyString + MyStringPosition - 1, MyString + MyStringPosition, (::_tcslen(MyString) - MyStringPosition + 1) * sizeof(TCHAR));
			MyStringPosition--;
			MyUpdateWindow(hWnd);
		}
		break;

	case VK_DELETE: {  // 删除键
		int len = ::_tcslen(MyString);
		if (MyStringPosition < len)
		{
			::MoveMemory(MyString + MyStringPosition, MyString + MyStringPosition + 1, (::_tcslen(MyString) - MyStringPosition + 1) * sizeof(TCHAR));
			MyUpdateWindow(hWnd);
		}
	}
					break;
	}

	MySetCaretPos(hWnd);
}
