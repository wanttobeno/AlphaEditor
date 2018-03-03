#include <Windows.h>
#include <GdiPlus.h>
#include "Win32App.h"
#pragma comment(lib,"gdiplus.lib")

int WINAPI WinMain( __in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in_opt LPSTR lpCmdLine, __in int nShowCmd )
{
	int nRet = 0;
	ULONG_PTR m_gdiplusToken;
	Gdiplus::GdiplusStartupInput StartupInput;
	GdiplusStartup(&m_gdiplusToken, &StartupInput, NULL);

	CWin32App winApp;
	if(winApp.Create(hInstance))
		nRet = winApp.Run();
	Gdiplus::GdiplusShutdown(m_gdiplusToken);
	return nRet;
}