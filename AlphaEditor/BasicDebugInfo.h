#ifndef BASICDEBUGINFO_H
#define BASICDEBUGINFO_H


#ifdef _DEBUG
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include <Windows.h> // OutputDebugString
#include <string>


#define ZY_ASSERT(x)		assert(x)
#define ZY_VERIFY(x)		assert(x)


#define ZY_TRACE			ZYDebugTrace


#define ZY_TRACEA			ZYDebugTraceA
#define ZY_TRACEW			ZYDebugTraceW

#ifndef _countof
#define _countof(a)		(sizeof(a) / sizeof(a[0]))
#endif

#if _MSC_VER < 1300
#define __FUNCTION__  __FILE__
#endif  //_MSC_VER < 1300



void __inline __cdecl ZYDebugTraceA(const char* pszFormat, ...)
{
	char szBuffer[1024];
	va_list arglist;
	va_start(arglist, pszFormat);
	ZY_VERIFY(vsprintf(szBuffer, pszFormat, arglist) < _countof(szBuffer));
	OutputDebugStringA(szBuffer);
	va_end(arglist);
}

void __inline __cdecl ZYDebugTraceW(const wchar_t* pszFormat, ...)
{
	va_list arglist;
	va_start(arglist, pszFormat);
	wchar_t szBuffer[1024];
#ifdef WIN32
	ZY_VERIFY(vswprintf(szBuffer,sizeof szBuffer / sizeof *szBuffer,pszFormat, arglist) < _countof(szBuffer));
	OutputDebugStringW(szBuffer);
#else
	ZY_VERIFY(vsprintf(szBuffer, pszFormat, arglist) < _countof(szBuffer));
	fwprintf(stderr, szBuffer);
#endif
	va_end(arglist);
}

void __inline __cdecl ZYDebugTrace(LPCTSTR pszFormat, ...)
{
	TCHAR szBuffer[1024];
	va_list arglist;
	va_start(arglist, pszFormat);
#ifdef WIN32
	ZY_VERIFY(wvsprintf(szBuffer, pszFormat, arglist) < _countof(szBuffer));
	OutputDebugString(szBuffer);
#else
	ZY_VERIFY(vsprintf(szBuffer, pszFormat, arglist) < _countof(szBuffer));
	fprintf(stderr, szBuffer);
#endif
	va_end(arglist);
}

#else // !_DEBUG

#include <string>

#define ZY_ASSERT(x)
#define ZY_VERIFY(x)		(x)
#define ZY_TRACE			(void)
#define ZY_TRACEA			(void)
#endif // _DEBUG



__inline std::string format( const char* fmt, ... )
{
	char buf[1 << 16];
	va_list args;
	va_start( args, fmt );
	vsprintf( buf, fmt, args );
	return std::string(buf);
}



__inline std::wstring format( const wchar_t* fmt, ... )
{
	wchar_t buf[1 << 16];
	va_list args;
	va_start( args, fmt );
	vswprintf( buf, 1 << 16,fmt, args );
	return std::wstring(buf);
}






#endif // !BASICDEBUGINFO_H
