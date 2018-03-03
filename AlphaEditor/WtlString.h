/*!
 * \file AltString.h
 * \date 2017/04/30 2:04
 *
 * \author ZhanYue
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * Code Form WTL91_5321_Final
 * Mod By ZY
*/
#ifndef _WTL_NO_CSTRING

#define  _WTL_NO_CSTRING
#include <stdio.h>
#include <tchar.h>
#include <Windows.h>
#include <assert.h>

#include <crtdbg.h>
#ifndef ATLASSERT
#define ATLASSERT(expr) _ASSERTE(expr)
#endif // ATLASSERT

#define  ZY_TRACE_CODE_INGO

__inline void TraceInfoA(const char *pszFormat,...)
{
#ifdef ZY_TRACE_CODE_INGO
	char szBuffer[1028]={0};
	va_list arglist;
	va_start(arglist, pszFormat);
	vsprintf_s(szBuffer,1028, pszFormat, arglist);
	OutputDebugStringA(szBuffer);
	va_end(arglist);
#endif // ZY_TRACE_CODE_INGO
}

__inline void TraceInfoW( const wchar_t *pszFormat,...)
{
#ifdef ZY_TRACE_CODE_INGO
	wchar_t szBuffer[1028]={0};
	va_list arglist;
	va_start(arglist, pszFormat);
	vswprintf_s(szBuffer,1028, pszFormat, arglist);
	OutputDebugStringW(szBuffer);
	va_end(arglist);
#endif // ZY_TRACE_CODE_INGO
}

#ifdef UNICODE
#define ATLTRACE2 TraceInfoW
#else
#define ATLTRACE2 TraceInfoA
#endif // UNICODE


__inline void TraceFileAndLineInfo(char* pBuf,int nLine)
{
#ifdef ZY_TRACE_CODE_INGO
	char buf[1028]={0};
	sprintf_s(buf,1028,"%s %d\n",pBuf,nLine);
	OutputDebugStringA(buf);
#endif // ZY_TRACE_CODE_INGO
}


#ifndef ATLTRACE
#define ATLTRACE TraceFileAndLineInfo(__FILE__, __LINE__)
//#define ATLTRACE2 ATLTRACE
#endif


#define ATLTRYALLOC(x) __pragma(warning(push)) __pragma(warning(disable: 4571)) try{x;} catch(...) {} __pragma(warning(pop))

#ifndef ATLTRY
#define ATLTRY(x) ATLTRYALLOC(x)
#endif	//ATLTRY


/////////////////////////////////////////////////////
namespace SecureHelper
{
	__inline void memcpy_x(void* pDest, size_t cbDest, const void* pSrc, size_t cbSrc)
	{		
		memcpy_s(pDest, cbDest, pSrc, cbSrc);
	}
	inline void memmove_x(void* pDest, size_t cbDest, const void* pSrc, size_t cbSrc)
	{
		memmove_s(pDest, cbDest, pSrc, cbSrc);
	}
	inline int vsprintf_x(LPTSTR lpstrBuff, size_t cchBuff, LPCTSTR lpstrFormat, va_list args)
	{
#if _SECURE_ATL && !defined(_ATL_MIN_CRT) && !defined(_WIN32_WCE)
		return _vstprintf_s(lpstrBuff, cchBuff, lpstrFormat, args);
#else
		cchBuff;   // Avoid unused argument warning
#pragma warning(push)
#pragma warning(disable: 4996)
		return _vstprintf(lpstrBuff, lpstrFormat, args);
#pragma warning(pop)
#endif
	}

	inline int wvsprintf_x(LPTSTR lpstrBuff, size_t cchBuff, LPCTSTR lpstrFormat, va_list args)
	{
#if _SECURE_ATL && !defined(_ATL_MIN_CRT) && !defined(_WIN32_WCE)
		return _vstprintf_s(lpstrBuff, cchBuff, lpstrFormat, args);
#else
		cchBuff;   // Avoid unused argument warning
		return ::wvsprintf(lpstrBuff, lpstrFormat, args);
#endif
	}

	inline int sprintf_x(LPTSTR lpstrBuff, size_t cchBuff, LPCTSTR lpstrFormat, ...)
	{
		va_list args;
		va_start(args, lpstrFormat);
		int nRes = vsprintf_x(lpstrBuff, cchBuff, lpstrFormat, args);
		va_end(args);
		return nRes;
	}

	inline int wsprintf_x(LPTSTR lpstrBuff, size_t cchBuff, LPCTSTR lpstrFormat, ...)
	{
		va_list args;
		va_start(args, lpstrFormat);
		int nRes = wvsprintf_x(lpstrBuff, cchBuff, lpstrFormat, args);
		va_end(args);
		return nRes;
	}
	////////////////
	inline errno_t strncpyW_x(wchar_t* lpstrDest, size_t cchDest, const wchar_t* lpstrSrc, size_t cchCount)
	{
		return ::wcsncpy_s(lpstrDest, cchDest, lpstrSrc, cchCount);
	}

	inline errno_t strncpy_x(LPTSTR lpstrDest, size_t cchDest, LPCTSTR lpstrSrc, size_t cchCount)
	{
#ifdef _UNICODE
		return strncpyW_x(lpstrDest, cchDest, lpstrSrc, cchCount);
#else
		return strncpyA_x(lpstrDest, cchDest, lpstrSrc, cchCount);
#endif
	}

	inline void strcatA_x(char* lpstrDest, size_t cchDest, const char* lpstrSrc)
	{
		 ::strcat_s(lpstrDest, cchDest, lpstrSrc);
	}

	inline void strcatW_x(wchar_t* lpstrDest, size_t cchDest, const wchar_t* lpstrSrc)
	{
		::wcscat_s(lpstrDest, cchDest, lpstrSrc);
	}

	inline void strcat_x(LPTSTR lpstrDest, size_t cchDest, LPCTSTR lpstrSrc)
	{
#ifdef _UNICODE
		strcatW_x(lpstrDest, cchDest, lpstrSrc);
#else
		strcatA_x(lpstrDest, cchDest, lpstrSrc);
#endif
	}
}
/////////////////////////////////////////////////////
namespace ModuleHelper
{
	__inline HMODULE GetResourceInstance()
	{
		return ::GetModuleHandle(NULL);
	}
}
/////////////////////////////////////////////////////
namespace MinCrtHelper
{
	inline int _isspace(TCHAR ch)
	{
#ifndef _ATL_MIN_CRT
		return _istspace(ch);
#else // _ATL_MIN_CRT
		WORD type = 0;
		::GetStringTypeEx(::GetThreadLocale(), CT_CTYPE1, &ch, 1, &type);
		return (type & C1_SPACE) == C1_SPACE;
#endif // _ATL_MIN_CRT
	}

	inline int _isdigit(TCHAR ch)
	{
#ifndef _ATL_MIN_CRT
		return _istdigit(ch);
#else // _ATL_MIN_CRT
		WORD type = 0;
		::GetStringTypeEx(::GetThreadLocale(), CT_CTYPE1, &ch, 1, &type);
		return (type & C1_DIGIT) == C1_DIGIT;
#endif // _ATL_MIN_CRT
	}

	inline int _atoi(LPCTSTR str)
	{
#ifndef _ATL_MIN_CRT
		return _ttoi(str);
#else // _ATL_MIN_CRT
		while(_isspace(*str) != 0)
			++str;

		TCHAR ch = *str++;
		TCHAR sign = ch;   // save sign indication
		if(ch == _T('-') || ch == _T('+'))
			ch = *str++;   // skip sign

		int total = 0;
		while(_isdigit(ch) != 0)
		{
			total = 10 * total + (ch - '0');   // accumulate digit
			ch = *str++;        // get next char
		}

		return (sign == '-') ? -total : total;   // return result, negated if necessary
#endif // _ATL_MIN_CRT
	}

	inline LPCTSTR _strrchr(LPCTSTR str, TCHAR ch)
	{
#ifndef _ATL_MIN_CRT
		return _tcsrchr(str, ch);
#else // _ATL_MIN_CRT
		LPCTSTR lpsz = NULL;
		while(*str != 0)
		{
			if(*str == ch)
				lpsz = str;
			str = ::CharNext(str);
		}
		return lpsz;
#endif // _ATL_MIN_CRT
	}

	inline LPTSTR _strrchr(LPTSTR str, TCHAR ch)
	{
#ifndef _ATL_MIN_CRT
		return _tcsrchr(str, ch);
#else // _ATL_MIN_CRT
		LPTSTR lpsz = NULL;
		while(*str != 0)
		{
			if(*str == ch)
				lpsz = str;
			str = ::CharNext(str);
		}
		return lpsz;
#endif // _ATL_MIN_CRT
	}
}
/////////////////////////////////////////////////////
struct CStringData
{
	long nRefs;     // reference count
	int nDataLength;
	int nAllocLength;
	// TCHAR data[nAllocLength]

	TCHAR* data()
	{ return (TCHAR*)(this + 1); }
};
/////////////////////////////////////////////////////
// Globals

// For an empty string, m_pchData will point here
// (note: avoids special case of checking for NULL m_pchData)
// empty string data (and locked)
_declspec(selectany) int rgInitData[] = { -1, 0, 0, 0 };
_declspec(selectany) CStringData* _atltmpDataNil = (CStringData*)&rgInitData;
_declspec(selectany) LPCTSTR _atltmpPchNil = (LPCTSTR)(((BYTE*)&rgInitData) + sizeof(CStringData));

/////////////////////////////////////////////////////
class CString
{
public:
	// Constructors
	CString()
	{
		Init();
	}

	CString(const CString& stringSrc)
	{
		ATLASSERT(stringSrc.GetData()->nRefs != 0);
		if (stringSrc.GetData()->nRefs >= 0)
		{
			ATLASSERT(stringSrc.GetData() != _atltmpDataNil);
			m_pchData = stringSrc.m_pchData;
			InterlockedIncrement(&GetData()->nRefs);
		}
		else
		{
			Init();
			*this = stringSrc.m_pchData;
		}
	}

	CString(TCHAR ch, int nRepeat = 1)
	{
		ATLASSERT(!_istlead(ch));   // can't create a lead byte string
		Init();
		if (nRepeat >= 1)
		{
			if(AllocBuffer(nRepeat))
			{
#ifdef _UNICODE
				for (int i = 0; i < nRepeat; i++)
					m_pchData[i] = ch;
#else
				memset(m_pchData, ch, nRepeat);
#endif
			}
		}
	}

	CString(LPCTSTR lpsz)
	{
		Init();
		if (lpsz != NULL && HIWORD(lpsz) == NULL)
		{
			UINT nID = LOWORD((DWORD_PTR)lpsz);
			if (!LoadString(nID))
				//ATLTRACE2(atlTraceUI, 0, _T("Warning: implicit LoadString(%u) in CString failed\n"), nID);
				ATLTRACE2(_T("Warning: implicit LoadString(%u) in CString failed\n"), nID);
		}
		else
		{
			int nLen = SafeStrlen(lpsz);
			if (nLen != 0)
			{
				if(AllocBuffer(nLen))
					SecureHelper::memcpy_x(m_pchData, (nLen + 1) * sizeof(TCHAR), lpsz, nLen * sizeof(TCHAR));

			}
		}
	}

#ifdef _UNICODE
	CString(LPCSTR lpsz)
	{
		Init();
#if defined(_WIN32_WCE) && (_ATL_VER >= 0x0800)
		int nSrcLen = (lpsz != NULL) ? ATL::lstrlenA(lpsz) : 0;
#else
		int nSrcLen = (lpsz != NULL) ? lstrlenA(lpsz) : 0;
#endif
		if (nSrcLen != 0)
		{
			if(AllocBuffer(nSrcLen))
			{
				_mbstowcsz(m_pchData, lpsz, nSrcLen + 1);
				ReleaseBuffer();
			}
		}
	}
#else // !_UNICODE
	CString(LPCWSTR lpsz)
	{
		Init();
		int nSrcLen = (lpsz != NULL) ? (int)wcslen(lpsz) : 0;
		if (nSrcLen != 0)
		{
			if(AllocBuffer(nSrcLen * 2))
			{
				_wcstombsz(m_pchData, lpsz, (nSrcLen * 2) + 1);
				ReleaseBuffer();
			}
		}
	}
#endif // !_UNICODE

	CString(LPCTSTR lpch, int nLength)
	{
		Init();
		if (nLength != 0)
		{
			if(AllocBuffer(nLength))
				SecureHelper::memcpy_x(m_pchData, (nLength + 1) * sizeof(TCHAR), lpch, nLength * sizeof(TCHAR));
		}
	}

#ifdef _UNICODE
	CString(LPCSTR lpsz, int nLength)
	{
		Init();
		if (nLength != 0)
		{
			if(AllocBuffer(nLength))
			{
				int n = ::MultiByteToWideChar(CP_ACP, 0, lpsz, nLength, m_pchData, nLength + 1);
				ReleaseBuffer((n >= 0) ? n : -1);
			}
		}
	}
#else // !_UNICODE
	CString(LPCWSTR lpsz, int nLength)
	{
		Init();
		if (nLength != 0)
		{
			if(((nLength * 2) > nLength) && AllocBuffer(nLength * 2))
			{
				int n = ::WideCharToMultiByte(CP_ACP, 0, lpsz, nLength, m_pchData, (nLength * 2) + 1, NULL, NULL);
				ReleaseBuffer((n >= 0) ? n : -1);
			}
		}
	}
#endif // !_UNICODE

	CString(const unsigned char* lpsz)
	{
		Init();
		*this = (LPCSTR)lpsz;
	}

	// Attributes & Operations
	int GetLength() const   // as an array of characters
	{
		return GetData()->nDataLength;
	}

	BOOL IsEmpty() const
	{
		return GetData()->nDataLength == 0;
	}

	void Empty()   // free up the data
	{
		if (GetData()->nDataLength == 0)
			return;

		if (GetData()->nRefs >= 0)
			Release();
		else
			*this = _T("");

		ATLASSERT(GetData()->nDataLength == 0);
		ATLASSERT(GetData()->nRefs < 0 || GetData()->nAllocLength == 0);
	}

	TCHAR GetAt(int nIndex) const   // 0 based
	{
		ATLASSERT(nIndex >= 0);
		ATLASSERT(nIndex < GetData()->nDataLength);
		return m_pchData[nIndex];
	}

	TCHAR operator [](int nIndex) const   // same as GetAt
	{
		// same as GetAt
		ATLASSERT(nIndex >= 0);
		ATLASSERT(nIndex < GetData()->nDataLength);
		return m_pchData[nIndex];
	}

	void SetAt(int nIndex, TCHAR ch)
	{
		ATLASSERT(nIndex >= 0);
		ATLASSERT(nIndex < GetData()->nDataLength);

		CopyBeforeWrite();
		m_pchData[nIndex] = ch;
	}

	operator LPCTSTR() const   // as a C string
	{
		return m_pchData;
	}

	// overloaded assignment
	CString& operator =(const CString& stringSrc)
	{
		if (m_pchData != stringSrc.m_pchData)
		{
			if ((GetData()->nRefs < 0 && GetData() != _atltmpDataNil) || stringSrc.GetData()->nRefs < 0)
			{
				// actual copy necessary since one of the strings is locked
				AssignCopy(stringSrc.GetData()->nDataLength, stringSrc.m_pchData);
			}
			else
			{
				// can just copy references around
				Release();
				ATLASSERT(stringSrc.GetData() != _atltmpDataNil);
				m_pchData = stringSrc.m_pchData;
				InterlockedIncrement(&GetData()->nRefs);
			}
		}
		return *this;
	}

	CString& operator =(TCHAR ch)
	{
		ATLASSERT(!_istlead(ch));   // can't set single lead byte
		AssignCopy(1, &ch);
		return *this;
	}

#ifdef _UNICODE
	CString& operator =(char ch)
	{
		*this = (TCHAR)ch;
		return *this;
	}
#endif

	CString& operator =(LPCTSTR lpsz)
	{
		ATLASSERT(lpsz == NULL || _IsValidString(lpsz));
		AssignCopy(SafeStrlen(lpsz), lpsz);
		return *this;
	}

#ifdef _UNICODE
	CString& operator =(LPCSTR lpsz)
	{
#if defined(_WIN32_WCE) && (_ATL_VER >= 0x0800)
		int nSrcLen = (lpsz != NULL) ? ATL::lstrlenA(lpsz) : 0;
#else
		int nSrcLen = (lpsz != NULL) ? lstrlenA(lpsz) : 0;
#endif
		if(AllocBeforeWrite(nSrcLen))
		{
			_mbstowcsz(m_pchData, lpsz, nSrcLen + 1);
			ReleaseBuffer();
		}
		return *this;
	}
#else // !_UNICODE
	CString& operator =(LPCWSTR lpsz)
	{
		int nSrcLen = (lpsz != NULL) ? (int)wcslen(lpsz) : 0;
		if(AllocBeforeWrite(nSrcLen * 2))
		{
			_wcstombsz(m_pchData, lpsz, (nSrcLen * 2) + 1);
			ReleaseBuffer();
		}
		return *this;
	}
#endif  // !_UNICODE

	CString& operator =(const unsigned char* lpsz)
	{
		*this = (LPCSTR)lpsz;
		return *this;
	}

	// string concatenation
	CString& operator +=(const CString& string)
	{
		ConcatInPlace(string.GetData()->nDataLength, string.m_pchData);
		return *this;
	}

	CString& operator +=(TCHAR ch)
	{
		ConcatInPlace(1, &ch);
		return *this;
	}

#ifdef _UNICODE
	CString& operator +=(char ch)
	{
		*this += (TCHAR)ch;
		return *this;
	}
#endif

	CString& operator +=(LPCTSTR lpsz)
	{
		ATLASSERT(lpsz == NULL || _IsValidString(lpsz));
		ConcatInPlace(SafeStrlen(lpsz), lpsz);
		return *this;
	}

	friend CString __stdcall operator +(const CString& string1, const CString& string2);
	friend CString __stdcall operator +(const CString& string, TCHAR ch);
	friend CString __stdcall operator +(TCHAR ch, const CString& string);
#ifdef _UNICODE
	friend CString __stdcall operator +(const CString& string, char ch);
	friend CString __stdcall operator +(char ch, const CString& string);
#endif
	friend CString __stdcall operator +(const CString& string, LPCTSTR lpsz);
	friend CString __stdcall operator +(LPCTSTR lpsz, const CString& string);

	// string comparison
	int Compare(LPCTSTR lpsz) const   // straight character (MBCS/Unicode aware)
	{
		return _cstrcmp(m_pchData, lpsz);
	}

	int CompareNoCase(LPCTSTR lpsz) const   // ignore case (MBCS/Unicode aware)
	{
		return _cstrcmpi(m_pchData, lpsz);
	}

#ifndef _WIN32_WCE
	// CString::Collate is often slower than Compare but is MBSC/Unicode
	//  aware as well as locale-sensitive with respect to sort order.
	int Collate(LPCTSTR lpsz) const   // NLS aware
	{
		return _cstrcoll(m_pchData, lpsz);
	}

	int CollateNoCase(LPCTSTR lpsz) const   // ignore case
	{
		return _cstrcolli(m_pchData, lpsz);
	}
#endif // !_WIN32_WCE

	// simple sub-string extraction
	CString Mid(int nFirst, int nCount) const
	{
		// out-of-bounds requests return sensible things
		if (nFirst < 0)
			nFirst = 0;
		if (nCount < 0)
			nCount = 0;

		if (nFirst + nCount > GetData()->nDataLength)
			nCount = GetData()->nDataLength - nFirst;
		if (nFirst > GetData()->nDataLength)
			nCount = 0;

		CString dest;
		AllocCopy(dest, nCount, nFirst, 0);
		return dest;
	}

	CString Mid(int nFirst) const
	{
		return Mid(nFirst, GetData()->nDataLength - nFirst);
	}

	CString Left(int nCount) const
	{
		if (nCount < 0)
			nCount = 0;
		else if (nCount > GetData()->nDataLength)
			nCount = GetData()->nDataLength;

		CString dest;
		AllocCopy(dest, nCount, 0, 0);
		return dest;
	}

	CString Right(int nCount) const
	{
		if (nCount < 0)
			nCount = 0;
		else if (nCount > GetData()->nDataLength)
			nCount = GetData()->nDataLength;

		CString dest;
		AllocCopy(dest, nCount, GetData()->nDataLength-nCount, 0);
		return dest;
	}

	CString SpanIncluding(LPCTSTR lpszCharSet) const   // strspn equivalent
	{
		ATLASSERT(_IsValidString(lpszCharSet));
		return Left(_cstrspn(m_pchData, lpszCharSet));
	}

	CString SpanExcluding(LPCTSTR lpszCharSet) const   // strcspn equivalent
	{
		ATLASSERT(_IsValidString(lpszCharSet));
		return Left(_cstrcspn(m_pchData, lpszCharSet));
	}

	// upper/lower/reverse conversion
	void MakeUpper()
	{
		CopyBeforeWrite();
		CharUpper(m_pchData);
	}

	void MakeLower()
	{
		CopyBeforeWrite();
		CharLower(m_pchData);
	}

	void MakeReverse()
	{
		CopyBeforeWrite();
		_cstrrev(m_pchData);
	}

	// trimming whitespace (either side)
	void TrimRight()
	{
		CopyBeforeWrite();

		// find beginning of trailing spaces by starting at beginning (DBCS aware)
		LPTSTR lpsz = m_pchData;
		LPTSTR lpszLast = NULL;
		while (*lpsz != _T('\0'))
		{
			if (_cstrisspace(*lpsz))
			{
				if (lpszLast == NULL)
					lpszLast = lpsz;
			}
			else
			{
				lpszLast = NULL;
			}
			lpsz = ::CharNext(lpsz);
		}

		if (lpszLast != NULL)
		{
			// truncate at trailing space start
			*lpszLast = _T('\0');
			GetData()->nDataLength = (int)(DWORD_PTR)(lpszLast - m_pchData);
		}
	}

	void TrimLeft()
	{
		CopyBeforeWrite();

		// find first non-space character
		LPCTSTR lpsz = m_pchData;
		while (_cstrisspace(*lpsz))
			lpsz = ::CharNext(lpsz);

		// fix up data and length
		int nDataLength = GetData()->nDataLength - (int)(DWORD_PTR)(lpsz - m_pchData);
		SecureHelper::memmove_x(m_pchData, (GetData()->nAllocLength + 1) * sizeof(TCHAR), lpsz, (nDataLength + 1) * sizeof(TCHAR));
		GetData()->nDataLength = nDataLength;
	}

	// remove continuous occurrences of chTarget starting from right
	void TrimRight(TCHAR chTarget)
	{
		// find beginning of trailing matches
		// by starting at beginning (DBCS aware)

		CopyBeforeWrite();
		LPTSTR lpsz = m_pchData;
		LPTSTR lpszLast = NULL;

		while (*lpsz != _T('\0'))
		{
			if (*lpsz == chTarget)
			{
				if (lpszLast == NULL)
					lpszLast = lpsz;
			}
			else
				lpszLast = NULL;
			lpsz = ::CharNext(lpsz);
		}

		if (lpszLast != NULL)
		{
			// truncate at left-most matching character
			*lpszLast = _T('\0');
			GetData()->nDataLength = (int)(DWORD_PTR)(lpszLast - m_pchData);
		}
	}

	// remove continuous occcurrences of characters in passed string, starting from right
	void TrimRight(LPCTSTR lpszTargetList)
	{
		// find beginning of trailing matches by starting at beginning (DBCS aware)

		CopyBeforeWrite();
		LPTSTR lpsz = m_pchData;
		LPTSTR lpszLast = NULL;

		while (*lpsz != _T('\0'))
		{
			TCHAR* pNext = ::CharNext(lpsz);
			if(pNext > lpsz + 1)
			{
				if (_cstrchr_db(lpszTargetList, *lpsz, *(lpsz + 1)) != NULL)
				{
					if (lpszLast == NULL)
						lpszLast = lpsz;
				}
				else
				{
					lpszLast = NULL;
				}
			}
			else
			{
				if (_cstrchr(lpszTargetList, *lpsz) != NULL)
				{
					if (lpszLast == NULL)
						lpszLast = lpsz;
				}
				else
				{
					lpszLast = NULL;
				}
			}

			lpsz = pNext;
		}

		if (lpszLast != NULL)
		{
			// truncate at left-most matching character
			*lpszLast = _T('\0');
			GetData()->nDataLength = (int)(DWORD_PTR)(lpszLast - m_pchData);
		}
	}

	// remove continuous occurrences of chTarget starting from left
	void TrimLeft(TCHAR chTarget)
	{
		// find first non-matching character

		CopyBeforeWrite();
		LPCTSTR lpsz = m_pchData;

		while (chTarget == *lpsz)
			lpsz = ::CharNext(lpsz);

		if (lpsz != m_pchData)
		{
			// fix up data and length
			int nDataLength = GetData()->nDataLength - (int)(DWORD_PTR)(lpsz - m_pchData);
			SecureHelper::memmove_x(m_pchData, (GetData()->nAllocLength + 1) * sizeof(TCHAR), lpsz, (nDataLength + 1) * sizeof(TCHAR));
			GetData()->nDataLength = nDataLength;
		}
	}

	// remove continuous occcurrences of characters in passed string, starting from left
	void TrimLeft(LPCTSTR lpszTargets)
	{
		// if we're not trimming anything, we're not doing any work
		if (SafeStrlen(lpszTargets) == 0)
			return;

		CopyBeforeWrite();
		LPCTSTR lpsz = m_pchData;

		while (*lpsz != _T('\0'))
		{
			TCHAR* pNext = ::CharNext(lpsz);
			if(pNext > lpsz + 1)
			{
				if (_cstrchr_db(lpszTargets, *lpsz, *(lpsz + 1)) == NULL)
					break;
			}
			else
			{
				if (_cstrchr(lpszTargets, *lpsz) == NULL)
					break;
			}
			lpsz = pNext;
		}

		if (lpsz != m_pchData)
		{
			// fix up data and length
			int nDataLength = GetData()->nDataLength - (int)(DWORD_PTR)(lpsz - m_pchData);
			SecureHelper::memmove_x(m_pchData, (GetData()->nAllocLength + 1) * sizeof(TCHAR), lpsz, (nDataLength + 1) * sizeof(TCHAR));
			GetData()->nDataLength = nDataLength;
		}
	}

	// advanced manipulation
	// replace occurrences of chOld with chNew
	int Replace(TCHAR chOld, TCHAR chNew)
	{
		int nCount = 0;

		// short-circuit the nop case
		if (chOld != chNew)
		{
			// otherwise modify each character that matches in the string
			CopyBeforeWrite();
			LPTSTR psz = m_pchData;
			LPTSTR pszEnd = psz + GetData()->nDataLength;
			while (psz < pszEnd)
			{
				// replace instances of the specified character only
				if (*psz == chOld)
				{
					*psz = chNew;
					nCount++;
				}
				psz = ::CharNext(psz);
			}
		}
		return nCount;
	}

	// replace occurrences of substring lpszOld with lpszNew;
	// empty lpszNew removes instances of lpszOld
	int Replace(LPCTSTR lpszOld, LPCTSTR lpszNew)
	{
		// can't have empty or NULL lpszOld

		int nSourceLen = SafeStrlen(lpszOld);
		if (nSourceLen == 0)
			return 0;
		int nReplacementLen = SafeStrlen(lpszNew);

		// loop once to figure out the size of the result string
		int nCount = 0;
		LPTSTR lpszStart = m_pchData;
		LPTSTR lpszEnd = m_pchData + GetData()->nDataLength;
		LPTSTR lpszTarget = NULL;
		while (lpszStart < lpszEnd)
		{
			while ((lpszTarget = (TCHAR*)_cstrstr(lpszStart, lpszOld)) != NULL)
			{
				nCount++;
				lpszStart = lpszTarget + nSourceLen;
			}
			lpszStart += lstrlen(lpszStart) + 1;
		}

		// if any changes were made, make them
		if (nCount > 0)
		{
			CopyBeforeWrite();

			// if the buffer is too small, just allocate a new buffer (slow but sure)
			int nOldLength = GetData()->nDataLength;
			int nNewLength =  nOldLength + (nReplacementLen - nSourceLen) * nCount;
			if (GetData()->nAllocLength < nNewLength || GetData()->nRefs > 1)
			{
				CStringData* pOldData = GetData();
				LPTSTR pstr = m_pchData;
				if(!AllocBuffer(nNewLength))
					return -1;
				SecureHelper::memcpy_x(m_pchData, (nNewLength + 1) * sizeof(TCHAR), pstr, pOldData->nDataLength * sizeof(TCHAR));
				CString::Release(pOldData);
			}
			// else, we just do it in-place
			lpszStart = m_pchData;
			lpszEnd = m_pchData + GetData()->nDataLength;

			// loop again to actually do the work
			while (lpszStart < lpszEnd)
			{
				while ((lpszTarget = (TCHAR*)_cstrstr(lpszStart, lpszOld)) != NULL)
				{
					int nBalance = nOldLength - ((int)(DWORD_PTR)(lpszTarget - m_pchData) + nSourceLen);
					int cchBuffLen = GetData()->nAllocLength - (int)(DWORD_PTR)(lpszTarget - m_pchData);
					SecureHelper::memmove_x(lpszTarget + nReplacementLen, (cchBuffLen - nReplacementLen + 1) * sizeof(TCHAR), lpszTarget + nSourceLen, nBalance * sizeof(TCHAR));
					SecureHelper::memcpy_x(lpszTarget, (cchBuffLen + 1) * sizeof(TCHAR), lpszNew, nReplacementLen * sizeof(TCHAR));
					lpszStart = lpszTarget + nReplacementLen;
					lpszStart[nBalance] = _T('\0');
					nOldLength += (nReplacementLen - nSourceLen);
				}
				lpszStart += lstrlen(lpszStart) + 1;
			}
			ATLASSERT(m_pchData[nNewLength] == _T('\0'));
			GetData()->nDataLength = nNewLength;
		}

		return nCount;
	}

	// remove occurrences of chRemove
	int Remove(TCHAR chRemove)
	{
		CopyBeforeWrite();

		LPTSTR pstrSource = m_pchData;
		LPTSTR pstrDest = m_pchData;
		LPTSTR pstrEnd = m_pchData + GetData()->nDataLength;

		while (pstrSource < pstrEnd)
		{
			if (*pstrSource != chRemove)
			{
				*pstrDest = *pstrSource;
				pstrDest = ::CharNext(pstrDest);
			}
			pstrSource = ::CharNext(pstrSource);
		}
		*pstrDest = _T('\0');
		int nCount = (int)(DWORD_PTR)(pstrSource - pstrDest);
		GetData()->nDataLength -= nCount;

		return nCount;
	}

	// insert character at zero-based index; concatenates if index is past end of string
	int Insert(int nIndex, TCHAR ch)
	{
		CopyBeforeWrite();

		if (nIndex < 0)
			nIndex = 0;

		int nNewLength = GetData()->nDataLength;
		if (nIndex > nNewLength)
			nIndex = nNewLength;
		nNewLength++;

		if (GetData()->nAllocLength < nNewLength)
		{
			CStringData* pOldData = GetData();
			LPTSTR pstr = m_pchData;
			if(!AllocBuffer(nNewLength))
				return -1;
			SecureHelper::memcpy_x(m_pchData, (nNewLength + 1) * sizeof(TCHAR), pstr, (pOldData->nDataLength + 1) * sizeof(TCHAR));
			CString::Release(pOldData);
		}

		// move existing bytes down
		SecureHelper::memmove_x(m_pchData + nIndex + 1, (GetData()->nAllocLength - nIndex) * sizeof(TCHAR), m_pchData + nIndex, (nNewLength - nIndex) * sizeof(TCHAR));
		m_pchData[nIndex] = ch;
		GetData()->nDataLength = nNewLength;

		return nNewLength;
	}

	// insert substring at zero-based index; concatenates if index is past end of string
	int Insert(int nIndex, LPCTSTR pstr)
	{
		if (nIndex < 0)
			nIndex = 0;

		int nInsertLength = SafeStrlen(pstr);
		int nNewLength = GetData()->nDataLength;
		if (nInsertLength > 0)
		{
			CopyBeforeWrite();
			if (nIndex > nNewLength)
				nIndex = nNewLength;
			nNewLength += nInsertLength;

			if (GetData()->nAllocLength < nNewLength)
			{
				CStringData* pOldData = GetData();
				LPTSTR pstrTmp = m_pchData;
				if(!AllocBuffer(nNewLength))
					return -1;
				SecureHelper::memcpy_x(m_pchData, (nNewLength + 1) * sizeof(TCHAR), pstrTmp, (pOldData->nDataLength + 1) * sizeof(TCHAR));
				CString::Release(pOldData);
			}

			// move existing bytes down
			SecureHelper::memmove_x(m_pchData + nIndex + nInsertLength, (GetData()->nAllocLength + 1 - nIndex - nInsertLength) * sizeof(TCHAR), m_pchData + nIndex, (nNewLength - nIndex - nInsertLength + 1) * sizeof(TCHAR));
			SecureHelper::memcpy_x(m_pchData + nIndex, (GetData()->nAllocLength + 1 - nIndex) * sizeof(TCHAR), pstr, nInsertLength * sizeof(TCHAR));
			GetData()->nDataLength = nNewLength;
		}

		return nNewLength;
	}

	// delete nCount characters starting at zero-based index
	int Delete(int nIndex, int nCount = 1)
	{
		if (nIndex < 0)
			nIndex = 0;
		int nLength = GetData()->nDataLength;
		if (nCount > 0 && nIndex < nLength)
		{
			if((nIndex + nCount) > nLength)
				nCount = nLength - nIndex;
			CopyBeforeWrite();
			int nBytesToCopy = nLength - (nIndex + nCount) + 1;

			SecureHelper::memmove_x(m_pchData + nIndex, (GetData()->nAllocLength + 1 - nIndex) * sizeof(TCHAR), m_pchData + nIndex + nCount, nBytesToCopy * sizeof(TCHAR));
			nLength -= nCount;
			GetData()->nDataLength = nLength;
		}

		return nLength;
	}

	// searching (return starting index, or -1 if not found)
	// look for a single character match
	int Find(TCHAR ch) const   // like "C" strchr
	{
		return Find(ch, 0);
	}

	int ReverseFind(TCHAR ch) const
	{
		// find last single character
		LPCTSTR lpsz = _cstrrchr(m_pchData, (_TUCHAR)ch);

		// return -1 if not found, distance from beginning otherwise
		return (lpsz == NULL) ? -1 : (int)(lpsz - m_pchData);
	}

	int Find(TCHAR ch, int nStart) const   // starting at index
	{
		int nLength = GetData()->nDataLength;
		if (nStart < 0 || nStart >= nLength)
			return -1;

		// find first single character
		LPCTSTR lpsz = _cstrchr(m_pchData + nStart, (_TUCHAR)ch);

		// return -1 if not found and index otherwise
		return (lpsz == NULL) ? -1 : (int)(lpsz - m_pchData);
	}

	int FindOneOf(LPCTSTR lpszCharSet) const
	{
		ATLASSERT(_IsValidString(lpszCharSet));
		LPCTSTR lpsz = _cstrpbrk(m_pchData, lpszCharSet);
		return (lpsz == NULL) ? -1 : (int)(lpsz - m_pchData);
	}

	// look for a specific sub-string
	// find a sub-string (like strstr)
	int Find(LPCTSTR lpszSub) const   // like "C" strstr
	{
		return Find(lpszSub, 0);
	}

	int Find(LPCTSTR lpszSub, int nStart) const   // starting at index
	{
		ATLASSERT(_IsValidString(lpszSub));

		int nLength = GetData()->nDataLength;
		if (nStart < 0 || nStart > nLength)
			return -1;

		// find first matching substring
		LPCTSTR lpsz = _cstrstr(m_pchData + nStart, lpszSub);

		// return -1 for not found, distance from beginning otherwise
		return (lpsz == NULL) ? -1 : (int)(lpsz - m_pchData);
	}

	// Concatentation for non strings
	CString& Append(int n)
	{
		const int cchBuff = 12;
		TCHAR szBuffer[cchBuff] = { 0 };
		SecureHelper::wsprintf_x(szBuffer, cchBuff, _T("%d"), n);
		ConcatInPlace(SafeStrlen(szBuffer), szBuffer);
		return *this;
	}

	// simple formatting
	// formatting (using wsprintf style formatting)
	BOOL __cdecl Format(LPCTSTR lpszFormat, ...)
	{
		ATLASSERT(_IsValidString(lpszFormat));

		va_list argList;
		va_start(argList, lpszFormat);
		BOOL bRet = FormatV(lpszFormat, argList);
		va_end(argList);
		return bRet;
	}

	BOOL __cdecl Format(UINT nFormatID, ...)
	{
		CString strFormat;
		BOOL bRet = strFormat.LoadString(nFormatID);
		ATLASSERT(bRet != 0);

		va_list argList;
		va_start(argList, nFormatID);
		bRet = FormatV(strFormat, argList);
		va_end(argList);
		return bRet;
	}

	BOOL FormatV(LPCTSTR lpszFormat, va_list argList)
	{
		ATLASSERT(_IsValidString(lpszFormat));

		enum _FormatModifiers
		{
			FORCE_ANSI =	0x10000,
			FORCE_UNICODE =	0x20000,
			FORCE_INT64 =	0x40000
		};

		va_list argListSave = argList;

		// make a guess at the maximum length of the resulting string
		int nMaxLen = 0;
		for (LPCTSTR lpsz = lpszFormat; *lpsz != _T('\0'); lpsz = ::CharNext(lpsz))
		{
			// handle '%' character, but watch out for '%%'
			if (*lpsz != _T('%') || *(lpsz = ::CharNext(lpsz)) == _T('%'))
			{
				nMaxLen += (int)(::CharNext(lpsz) - lpsz);
				continue;
			}

			int nItemLen = 0;

			// handle '%' character with format
			int nWidth = 0;
			for (; *lpsz != _T('\0'); lpsz = ::CharNext(lpsz))
			{
				// check for valid flags
				if (*lpsz == _T('#'))
					nMaxLen += 2;   // for '0x'
				else if (*lpsz == _T('*'))
					nWidth = va_arg(argList, int);
				else if (*lpsz == _T('-') || *lpsz == _T('+') || *lpsz == _T('0') || *lpsz == _T(' '))
					;
				else // hit non-flag character
					break;
			}
			// get width and skip it
			if (nWidth == 0)
			{
				// width indicated by
				nWidth = _cstrtoi(lpsz);
				for (; *lpsz != _T('\0') && _cstrisdigit(*lpsz); lpsz = ::CharNext(lpsz))
					;
			}
			ATLASSERT(nWidth >= 0);

			int nPrecision = 0;
			if (*lpsz == _T('.'))
			{
				// skip past '.' separator (width.precision)
				lpsz = ::CharNext(lpsz);

				// get precision and skip it
				if (*lpsz == _T('*'))
				{
					nPrecision = va_arg(argList, int);
					lpsz = ::CharNext(lpsz);
				}
				else
				{
					nPrecision = _cstrtoi(lpsz);
					for (; *lpsz != _T('\0') && _cstrisdigit(*lpsz); lpsz = ::CharNext(lpsz))
						;
				}
				ATLASSERT(nPrecision >= 0);
			}

			// should be on type modifier or specifier
			int nModifier = 0;
			if(lpsz[0] == _T('I'))
			{
				if((lpsz[1] == _T('6')) && (lpsz[2] == _T('4')))
				{
					lpsz += 3;
					nModifier = FORCE_INT64;
				}
				else if((lpsz[1] == _T('3')) && (lpsz[2] == _T('2')))
				{
					lpsz += 3;
				}
				else
				{
					lpsz++;
					if(sizeof(size_t) == 8)
						nModifier = FORCE_INT64;
				}
			}
			else
			{
				switch (*lpsz)
				{
					// modifiers that affect size
				case _T('h'):
					nModifier = FORCE_ANSI;
					lpsz = ::CharNext(lpsz);
					break;
				case _T('l'):
					nModifier = FORCE_UNICODE;
					lpsz = ::CharNext(lpsz);
					break;

					// modifiers that do not affect size
				case _T('F'):
				case _T('N'):
				case _T('L'):
					lpsz = ::CharNext(lpsz);
					break;
				}
			}

			// now should be on specifier
			switch (*lpsz | nModifier)
			{
				// single characters
			case _T('c'):
			case _T('C'):
				nItemLen = 2;
				va_arg(argList, TCHAR);
				break;
			case _T('c') | FORCE_ANSI:
			case _T('C') | FORCE_ANSI:
				nItemLen = 2;
				va_arg(argList, char);
				break;
			case _T('c') | FORCE_UNICODE:
			case _T('C') | FORCE_UNICODE:
				nItemLen = 2;
				va_arg(argList, WCHAR);
				break;

				// strings
			case _T('s'):
				{
					LPCTSTR pstrNextArg = va_arg(argList, LPCTSTR);
					if (pstrNextArg == NULL)
					{
						nItemLen = 6;  // "(null)"
					}
					else
					{
						nItemLen = lstrlen(pstrNextArg);
						nItemLen = __max(1, nItemLen);
					}
					break;
				}

			case _T('S'):
				{
#ifndef _UNICODE
					LPWSTR pstrNextArg = va_arg(argList, LPWSTR);
					if (pstrNextArg == NULL)
					{
						nItemLen = 6;  // "(null)"
					}
					else
					{
						nItemLen = (int)wcslen(pstrNextArg);
						nItemLen = __max(1, nItemLen);
					}
#else // _UNICODE
					LPCSTR pstrNextArg = va_arg(argList, LPCSTR);
					if (pstrNextArg == NULL)
					{
						nItemLen = 6; // "(null)"
					}
					else
					{
#if defined(_WIN32_WCE) && (_ATL_VER >= 0x0800)
						nItemLen = ATL::lstrlenA(pstrNextArg);
#else
						nItemLen = lstrlenA(pstrNextArg);
#endif
						nItemLen = __max(1, nItemLen);
					}
#endif // _UNICODE
					break;
				}

			case _T('s') | FORCE_ANSI:
			case _T('S') | FORCE_ANSI:
				{
					LPCSTR pstrNextArg = va_arg(argList, LPCSTR);
					if (pstrNextArg == NULL)
					{
						nItemLen = 6; // "(null)"
					}
					else
					{
#if defined(_WIN32_WCE) && (_ATL_VER >= 0x0800)
						nItemLen = ATL::lstrlenA(pstrNextArg);
#else
						nItemLen = lstrlenA(pstrNextArg);
#endif
						nItemLen = __max(1, nItemLen);
					}
					break;
				}

			case _T('s') | FORCE_UNICODE:
			case _T('S') | FORCE_UNICODE:
				{
					LPWSTR pstrNextArg = va_arg(argList, LPWSTR);
					if (pstrNextArg == NULL)
					{
						nItemLen = 6; // "(null)"
					}
					else
					{
						nItemLen = (int)wcslen(pstrNextArg);
						nItemLen = __max(1, nItemLen);
					}
					break;
				}
			}

			// adjust nItemLen for strings
			if (nItemLen != 0)
			{
				nItemLen = __max(nItemLen, nWidth);
				if (nPrecision != 0)
					nItemLen = __min(nItemLen, nPrecision);
			}
			else
			{
				switch (*lpsz)
				{
					// integers
				case _T('d'):
				case _T('i'):
				case _T('u'):
				case _T('x'):
				case _T('X'):
				case _T('o'):
					if (nModifier & FORCE_INT64)
						va_arg(argList, __int64);
					else
						va_arg(argList, int);
					nItemLen = 32;
					nItemLen = __max(nItemLen, nWidth + nPrecision);
					break;

#ifndef _ATL_USE_CSTRING_FLOAT
				case _T('e'):
				case _T('E'):
				case _T('f'):
				case _T('g'):
				case _T('G'):
					ATLASSERT(!"Floating point (%%e, %%E, %%f, %%g, and %%G) is not supported by the WTL::CString class.");
#ifndef _DEBUG
					::OutputDebugString(_T("Floating point (%%e, %%f, %%g, and %%G) is not supported by the WTL::CString class."));
#ifndef _WIN32_WCE
					::DebugBreak();
#else // CE specific
					DebugBreak();
#endif // _WIN32_WCE
#endif // !_DEBUG
					break;
#else // _ATL_USE_CSTRING_FLOAT
				case _T('e'):
				case _T('E'):
				case _T('g'):
				case _T('G'):
					va_arg(argList, double);
					nItemLen = 128;
					nItemLen = __max(nItemLen, nWidth + nPrecision);
					break;
				case _T('f'):
					{
						double f = va_arg(argList, double);
						// 312 == strlen("-1+(309 zeroes).")
						// 309 zeroes == max precision of a double
						// 6 == adjustment in case precision is not specified,
						//   which means that the precision defaults to 6
						int cchLen = __max(nWidth, 312 + nPrecision + 6);
						CTempBuffer<TCHAR, _WTL_STACK_ALLOC_THRESHOLD> buff;
						LPTSTR pszTemp = buff.Allocate(cchLen);
						if(pszTemp != NULL)
						{
							SecureHelper::sprintf_x(pszTemp, cchLen, _T("%*.*f"), nWidth, nPrecision + 6, f);
							nItemLen = (int)_tcslen(pszTemp);
						}
						else
						{
							nItemLen = cchLen;
						}
					}
					break;
#endif // _ATL_USE_CSTRING_FLOAT

				case _T('p'):
					va_arg(argList, void*);
					nItemLen = 32;
					nItemLen = __max(nItemLen, nWidth + nPrecision);
					break;

					// no output
				case _T('n'):
					va_arg(argList, int*);
					break;

				default:
					ATLASSERT(FALSE);  // unknown formatting option
				}
			}

			// adjust nMaxLen for output nItemLen
			nMaxLen += nItemLen;
		}

		if(GetBuffer(nMaxLen) == NULL)
			return FALSE;
#ifndef _ATL_USE_CSTRING_FLOAT
		int nRet = SecureHelper::wvsprintf_x(m_pchData, GetAllocLength() + 1, lpszFormat, argListSave);
#else // _ATL_USE_CSTRING_FLOAT
		int nRet = SecureHelper::vsprintf_x(m_pchData, GetAllocLength() + 1, lpszFormat, argListSave);
#endif // _ATL_USE_CSTRING_FLOAT
		nRet;   // ref
		ATLASSERT(nRet <= GetAllocLength());
		ReleaseBuffer();

		va_end(argListSave);
		return TRUE;
	}

	// formatting for localization (uses FormatMessage API)
	// formatting (using FormatMessage style formatting)
	BOOL __cdecl FormatMessage(LPCTSTR lpszFormat, ...)
	{
		// format message into temporary buffer lpszTemp
		va_list argList;
		va_start(argList, lpszFormat);
		LPTSTR lpszTemp = NULL;
		BOOL bRet = TRUE;

		if ((::FormatMessage(FORMAT_MESSAGE_FROM_STRING | FORMAT_MESSAGE_ALLOCATE_BUFFER,
			lpszFormat, 0, 0, (LPTSTR)&lpszTemp, 0, &argList) == 0) || (lpszTemp == NULL))
			bRet = FALSE;

		// assign lpszTemp into the resulting string and free the temporary
		*this = lpszTemp;
		LocalFree(lpszTemp);
		va_end(argList);
		return bRet;
	}

	BOOL __cdecl FormatMessage(UINT nFormatID, ...)
	{
		// get format string from string table
		CString strFormat;
		BOOL bRetTmp = strFormat.LoadString(nFormatID);
		bRetTmp;   // ref
		ATLASSERT(bRetTmp != 0);

		// format message into temporary buffer lpszTemp
		va_list argList;
		va_start(argList, nFormatID);
		LPTSTR lpszTemp = NULL;
		BOOL bRet = TRUE;

		if ((::FormatMessage(FORMAT_MESSAGE_FROM_STRING | FORMAT_MESSAGE_ALLOCATE_BUFFER,
			strFormat, 0, 0, (LPTSTR)&lpszTemp, 0, &argList) == 0) || (lpszTemp == NULL))
			bRet = FALSE;

		// assign lpszTemp into the resulting string and free lpszTemp
		*this = lpszTemp;
		LocalFree(lpszTemp);
		va_end(argList);
		return bRet;
	}

	// Windows support
	BOOL LoadString(UINT nID)   // load from string resource (255 chars max.)
	{
#ifdef _UNICODE
		const int CHAR_FUDGE = 1;   // one TCHAR unused is good enough
#else
		const int CHAR_FUDGE = 2;   // two BYTES unused for case of DBC last char
#endif

		// try fixed buffer first (to avoid wasting space in the heap)
		TCHAR szTemp[256] = { 0 };
		int nCount =  sizeof(szTemp) / sizeof(szTemp[0]);
		int nLen = _LoadString(nID, szTemp, nCount);
		if (nCount - nLen > CHAR_FUDGE)
		{
			*this = szTemp;
			return (nLen > 0);
		}

		// try buffer size of 512, then larger size until entire string is retrieved
		int nSize = 256;
		do
		{
			nSize += 256;
			LPTSTR lpstr = GetBuffer(nSize - 1);
			if(lpstr == NULL)
			{
				nLen = 0;
				break;
			}
			nLen = _LoadString(nID, lpstr, nSize);
		} while (nSize - nLen <= CHAR_FUDGE);
		ReleaseBuffer();

		return (nLen > 0);
	}

#ifndef _UNICODE
	// ANSI <-> OEM support (convert string in place)
	void AnsiToOem()
	{
		CopyBeforeWrite();
		::AnsiToOem(m_pchData, m_pchData);
	}

	void OemToAnsi()
	{
		CopyBeforeWrite();
		::OemToAnsi(m_pchData, m_pchData);
	}
#endif

#ifndef _ATL_NO_COM
	// OLE BSTR support (use for OLE automation)
	BSTR AllocSysString() const
	{
#if defined(_UNICODE) || defined(OLE2ANSI)
		BSTR bstr = ::SysAllocStringLen(m_pchData, GetData()->nDataLength);
#else
		int nLen = MultiByteToWideChar(CP_ACP, 0, m_pchData,
			GetData()->nDataLength, NULL, NULL);
		BSTR bstr = ::SysAllocStringLen(NULL, nLen);
		if(bstr != NULL)
			MultiByteToWideChar(CP_ACP, 0, m_pchData, GetData()->nDataLength, bstr, nLen);
#endif
		return bstr;
	}

	BSTR SetSysString(BSTR* pbstr) const
	{
#if defined(_UNICODE) || defined(OLE2ANSI)
		::SysReAllocStringLen(pbstr, m_pchData, GetData()->nDataLength);
#else
		int nLen = MultiByteToWideChar(CP_ACP, 0, m_pchData,
			GetData()->nDataLength, NULL, NULL);
		if(::SysReAllocStringLen(pbstr, NULL, nLen))
			MultiByteToWideChar(CP_ACP, 0, m_pchData, GetData()->nDataLength, *pbstr, nLen);
#endif
		ATLASSERT(*pbstr != NULL);
		return *pbstr;
	}
#endif // !_ATL_NO_COM

	// Access to string implementation buffer as "C" character array
	LPTSTR GetBuffer(int nMinBufLength)
	{
		ATLASSERT(nMinBufLength >= 0);

		if (GetData()->nRefs > 1 || nMinBufLength > GetData()->nAllocLength)
		{
			// we have to grow the buffer
			CStringData* pOldData = GetData();
			int nOldLen = GetData()->nDataLength;   // AllocBuffer will tromp it
			if (nMinBufLength < nOldLen)
				nMinBufLength = nOldLen;

			if(!AllocBuffer(nMinBufLength))
				return NULL;

			SecureHelper::memcpy_x(m_pchData, (nMinBufLength + 1) * sizeof(TCHAR), pOldData->data(), (nOldLen + 1) * sizeof(TCHAR));
			GetData()->nDataLength = nOldLen;
			CString::Release(pOldData);
		}
		ATLASSERT(GetData()->nRefs <= 1);

		// return a pointer to the character storage for this string
		ATLASSERT(m_pchData != NULL);
		return m_pchData;
	}

	void ReleaseBuffer(int nNewLength = -1)
	{
		CopyBeforeWrite();   // just in case GetBuffer was not called

		if (nNewLength == -1)
			nNewLength = lstrlen(m_pchData);   // zero terminated

		ATLASSERT(nNewLength <= GetData()->nAllocLength);
		GetData()->nDataLength = nNewLength;
		m_pchData[nNewLength] = _T('\0');
	}

	LPTSTR GetBufferSetLength(int nNewLength)
	{
		ATLASSERT(nNewLength >= 0);

		if(GetBuffer(nNewLength) == NULL)
			return NULL;

		GetData()->nDataLength = nNewLength;
		m_pchData[nNewLength] = _T('\0');
		return m_pchData;
	}

	void FreeExtra()
	{
		ATLASSERT(GetData()->nDataLength <= GetData()->nAllocLength);
		if (GetData()->nDataLength != GetData()->nAllocLength)
		{
			CStringData* pOldData = GetData();
			if(AllocBuffer(GetData()->nDataLength))
			{
				SecureHelper::memcpy_x(m_pchData, (GetData()->nAllocLength + 1) * sizeof(TCHAR), pOldData->data(), pOldData->nDataLength * sizeof(TCHAR));
				ATLASSERT(m_pchData[GetData()->nDataLength] == _T('\0'));
				CString::Release(pOldData);
			}
		}
		ATLASSERT(GetData() != NULL);
	}

	// Use LockBuffer/UnlockBuffer to turn refcounting off
	LPTSTR LockBuffer()
	{
		LPTSTR lpsz = GetBuffer(0);
		if(lpsz != NULL)
			GetData()->nRefs = -1;
		return lpsz;
	}

	void UnlockBuffer()
	{
		ATLASSERT(GetData()->nRefs == -1);
		if (GetData() != _atltmpDataNil)
			GetData()->nRefs = 1;
	}

	// Implementation
public:
	~CString()   //  free any attached data
	{
		if (GetData() != _atltmpDataNil)
		{
			if (InterlockedDecrement(&GetData()->nRefs) <= 0)
				delete[] (BYTE*)GetData();
		}
	}

	int GetAllocLength() const
	{
		return GetData()->nAllocLength;
	}

	static BOOL __stdcall _IsValidString(LPCTSTR lpsz, int /*nLength*/ = -1)
	{
		return (lpsz != NULL) ? TRUE : FALSE;
	}

protected:
	LPTSTR m_pchData;   // pointer to ref counted string data

	// implementation helpers
	CStringData* GetData() const
	{
		ATLASSERT(m_pchData != NULL);
		return ((CStringData*)m_pchData) - 1;
	}

	void Init()
	{
		m_pchData = _GetEmptyString().m_pchData;
	}

	BOOL AllocCopy(CString& dest, int nCopyLen, int nCopyIndex, int nExtraLen) const
	{
		// will clone the data attached to this string
		// allocating 'nExtraLen' characters
		// Places results in uninitialized string 'dest'
		// Will copy the part or all of original data to start of new string

		BOOL bRet = FALSE;
		int nNewLen = nCopyLen + nExtraLen;
		if (nNewLen == 0)
		{
			dest.Init();
			bRet = TRUE;
		}
		else if(nNewLen >= nCopyLen)
		{
			if(dest.AllocBuffer(nNewLen))
			{
				SecureHelper::memcpy_x(dest.m_pchData, (nNewLen + 1) * sizeof(TCHAR), m_pchData + nCopyIndex, nCopyLen * sizeof(TCHAR));
				bRet = TRUE;
			}
		}

		return bRet;
	}

	// always allocate one extra character for '\0' termination
	// assumes [optimistically] that data length will equal allocation length
	BOOL AllocBuffer(int nLen)
	{
		ATLASSERT(nLen >= 0);
		ATLASSERT(nLen <= INT_MAX - 1);   // max size (enough room for 1 extra)

		if (nLen == 0)
		{
			Init();
		}
		else
		{
			CStringData* pData = NULL;
			ATLTRY(pData = (CStringData*)new BYTE[sizeof(CStringData) + (nLen + 1) * sizeof(TCHAR)]);
			if(pData == NULL)
				return FALSE;

			pData->nRefs = 1;
			pData->data()[nLen] = _T('\0');
			pData->nDataLength = nLen;
			pData->nAllocLength = nLen;
			m_pchData = pData->data();
		}

		return TRUE;
	}

	// Assignment operators
	//  All assign a new value to the string
	//      (a) first see if the buffer is big enough
	//      (b) if enough room, copy on top of old buffer, set size and type
	//      (c) otherwise free old string data, and create a new one
	//
	//  All routines return the new string (but as a 'const CString&' so that
	//      assigning it again will cause a copy, eg: s1 = s2 = "hi there".
	//
	void AssignCopy(int nSrcLen, LPCTSTR lpszSrcData)
	{
		if(AllocBeforeWrite(nSrcLen))
		{
			SecureHelper::memcpy_x(m_pchData, (nSrcLen + 1) * sizeof(TCHAR), lpszSrcData, nSrcLen * sizeof(TCHAR));
			GetData()->nDataLength = nSrcLen;
			m_pchData[nSrcLen] = _T('\0');
		}
	}

	// Concatenation
	// NOTE: "operator +" is done as friend functions for simplicity
	//      There are three variants:
	//          CString + CString
	// and for ? = TCHAR, LPCTSTR
	//          CString + ?
	//          ? + CString
	BOOL ConcatCopy(int nSrc1Len, LPCTSTR lpszSrc1Data, int nSrc2Len, LPCTSTR lpszSrc2Data)
	{
		// -- master concatenation routine
		// Concatenate two sources
		// -- assume that 'this' is a new CString object

		BOOL bRet = TRUE;
		int nNewLen = nSrc1Len + nSrc2Len;
		if(nNewLen < nSrc1Len || nNewLen < nSrc2Len)
		{
			bRet = FALSE;
		}
		else if(nNewLen != 0)
		{
			bRet = AllocBuffer(nNewLen);
			if (bRet)
			{
				SecureHelper::memcpy_x(m_pchData, (nNewLen + 1) * sizeof(TCHAR), lpszSrc1Data, nSrc1Len * sizeof(TCHAR));
				SecureHelper::memcpy_x(m_pchData + nSrc1Len, (nNewLen + 1 - nSrc1Len) * sizeof(TCHAR), lpszSrc2Data, nSrc2Len * sizeof(TCHAR));
			}
		}
		return bRet;
	}

	void ConcatInPlace(int nSrcLen, LPCTSTR lpszSrcData)
	{
		//  -- the main routine for += operators

		// concatenating an empty string is a no-op!
		if (nSrcLen == 0)
			return;

		// if the buffer is too small, or we have a width mis-match, just
		//   allocate a new buffer (slow but sure)
		if (GetData()->nRefs > 1 || GetData()->nDataLength + nSrcLen > GetData()->nAllocLength)
		{
			// we have to grow the buffer, use the ConcatCopy routine
			CStringData* pOldData = GetData();
			if (ConcatCopy(GetData()->nDataLength, m_pchData, nSrcLen, lpszSrcData))
			{
				ATLASSERT(pOldData != NULL);
				CString::Release(pOldData);
			}
		}
		else
		{
			// fast concatenation when buffer big enough
			SecureHelper::memcpy_x(m_pchData + GetData()->nDataLength, (GetData()->nAllocLength + 1) * sizeof(TCHAR), lpszSrcData, nSrcLen * sizeof(TCHAR));
			GetData()->nDataLength += nSrcLen;
			ATLASSERT(GetData()->nDataLength <= GetData()->nAllocLength);
			m_pchData[GetData()->nDataLength] = _T('\0');
		}
	}

	void CopyBeforeWrite()
	{
		if (GetData()->nRefs > 1)
		{
			CStringData* pData = GetData();
			Release();
			if(AllocBuffer(pData->nDataLength))
				SecureHelper::memcpy_x(m_pchData, (GetData()->nAllocLength + 1) * sizeof(TCHAR), pData->data(), (pData->nDataLength + 1) * sizeof(TCHAR));
		}
		ATLASSERT(GetData()->nRefs <= 1);
	}

	BOOL AllocBeforeWrite(int nLen)
	{
		BOOL bRet = TRUE;
		if (GetData()->nRefs > 1 || nLen > GetData()->nAllocLength)
		{
			Release();
			bRet = AllocBuffer(nLen);
		}
		ATLASSERT(GetData()->nRefs <= 1);
		return bRet;
	}

	void Release()
	{
		if (GetData() != _atltmpDataNil)
		{
			ATLASSERT(GetData()->nRefs != 0);
			if (InterlockedDecrement(&GetData()->nRefs) <= 0)
				delete[] (BYTE*)GetData();
			Init();
		}
	}

	static void PASCAL Release(CStringData* pData)
	{
		if (pData != _atltmpDataNil)
		{
			ATLASSERT(pData->nRefs != 0);
			if (InterlockedDecrement(&pData->nRefs) <= 0)
				delete[] (BYTE*)pData;
		}
	}

	static int PASCAL SafeStrlen(LPCTSTR lpsz)
	{
		return (lpsz == NULL) ? 0 : lstrlen(lpsz);
	}

	static int __stdcall _LoadString(UINT nID, LPTSTR lpszBuf, UINT nMaxBuf)
	{
#ifdef _DEBUG
		// LoadString without annoying warning from the Debug kernel if the
		//  segment containing the string is not present
		if (::FindResource(ModuleHelper::GetResourceInstance(), MAKEINTRESOURCE((nID >> 4) + 1), RT_STRING) == NULL)
		{
			lpszBuf[0] = _T('\0');
			return 0;   // not found
		}
#endif // _DEBUG

		int nLen = ::LoadString(ModuleHelper::GetResourceInstance(), nID, lpszBuf, nMaxBuf);
		if (nLen == 0)
			lpszBuf[0] = _T('\0');

		return nLen;
	}

	static const CString& __stdcall _GetEmptyString()
	{
		return *(CString*)&_atltmpPchNil;
	}

	// CString conversion helpers
	static int __cdecl _wcstombsz(char* mbstr, const wchar_t* wcstr, size_t count)
	{
		if (count == 0 && mbstr != NULL)
			return 0;

		int result = ::WideCharToMultiByte(CP_ACP, 0, wcstr, -1, mbstr, (int)count, NULL, NULL);
		ATLASSERT(mbstr == NULL || result <= (int)count);
		if ((mbstr != NULL) && (result > 0))
			mbstr[result - 1] = 0;
		return result;
	}

	static int __cdecl _mbstowcsz(wchar_t* wcstr, const char* mbstr, size_t count)
	{
		if (count == 0 && wcstr != NULL)
			return 0;

		int result = ::MultiByteToWideChar(CP_ACP, 0, mbstr, -1, wcstr, (int)count);
		ATLASSERT(wcstr == NULL || result <= (int)count);
		if ((wcstr != NULL) && (result > 0))
			wcstr[result - 1] = 0;
		return result;
	}

	// Helpers to avoid CRT startup code
#ifdef _ATL_MIN_CRT
	static const TCHAR* _cstrchr(const TCHAR* p, TCHAR ch)
	{
		// strchr for '\0' should succeed
		while (*p != 0)
		{
			if (*p == ch)
				break;
			p = ::CharNext(p);
		}
		return (*p == ch) ? p : NULL;
	}

	static TCHAR* _cstrrev(TCHAR* pStr)
	{
		// optimize NULL, zero-length, and single-char case
		if ((pStr == NULL) || (pStr[0] == _T('\0')) || (pStr[1] == _T('\0')))
			return pStr;

		TCHAR* p = pStr;

		while (*p != 0) 
		{
			TCHAR* pNext = ::CharNext(p);
			if(pNext > p + 1)
			{
				char p1 = *(char*)p;
				*(char*)p = *(char*)(p + 1);
				*(char*)(p + 1) = p1;
			}
			p = pNext;
		}

		p--;
		TCHAR* q = pStr;

		while (q < p)
		{
			TCHAR t = *q;
			*q = *p;
			*p = t;
			q++;
			p--;
		}
		return pStr;
	}

	static const TCHAR* _cstrstr(const TCHAR* pStr, const TCHAR* pCharSet)
	{
		int nLen = lstrlen(pCharSet);
		if (nLen == 0)
			return (TCHAR*)pStr;

		const TCHAR* pRet = NULL;
		const TCHAR* pCur = pStr;
		while((pCur = _cstrchr(pCur, *pCharSet)) != NULL)
		{
			if(memcmp(pCur, pCharSet, nLen * sizeof(TCHAR)) == 0)
			{
				pRet = pCur;
				break;
			}
			pCur = ::CharNext(pCur);
		}
		return pRet;
	}

	static int _cstrspn(const TCHAR* pStr, const TCHAR* pCharSet)
	{
		int nRet = 0;
		const TCHAR* p = pStr;
		while (*p != 0)
		{
			const TCHAR* pNext = ::CharNext(p);
			if(pNext > p + 1)
			{
				if(_cstrchr_db(pCharSet, *p, *(p + 1)) == NULL)
					break;
				nRet += 2;
			}
			else
			{
				if(_cstrchr(pCharSet, *p) == NULL)
					break;
				nRet++;
			}
			p = pNext;
		}
		return nRet;
	}

	static int _cstrcspn(const TCHAR* pStr, const TCHAR* pCharSet)
	{
		int nRet = 0;
		TCHAR* p = (TCHAR*)pStr;
		while (*p != 0)
		{
			TCHAR* pNext = ::CharNext(p);
			if(pNext > p + 1)
			{
				if(_cstrchr_db(pCharSet, *p, *(p + 1)) != NULL)
					break;
				nRet += 2;
			}
			else
			{
				if(_cstrchr(pCharSet, *p) != NULL)
					break;
				nRet++;
			}
			p = pNext;
		}
		return nRet;
	}

	static const TCHAR* _cstrpbrk(const TCHAR* p, const TCHAR* lpszCharSet)
	{
		int n = _cstrcspn(p, lpszCharSet);
		return (p[n] != 0) ? &p[n] : NULL;
	}

	static int _cstrcmp(const TCHAR* pstrOne, const TCHAR* pstrOther)
	{
		return lstrcmp(pstrOne, pstrOther);
	}

	static int _cstrcmpi(const TCHAR* pstrOne, const TCHAR* pstrOther)
	{
		return lstrcmpi(pstrOne, pstrOther);
	}

	static int _cstrcoll(const TCHAR* pstrOne, const TCHAR* pstrOther)
	{
		int nRet = CompareString(GetThreadLocale(), 0, pstrOne, -1, pstrOther, -1);
		ATLASSERT(nRet != 0);
		return nRet - 2;   // convert to strcmp convention
	}

	static int _cstrcolli(const TCHAR* pstrOne, const TCHAR* pstrOther)
	{
		int nRet = CompareString(GetThreadLocale(), NORM_IGNORECASE, pstrOne, -1, pstrOther, -1);
		ATLASSERT(nRet != 0);
		return nRet - 2;   // convert to strcmp convention
	}
#else // !_ATL_MIN_CRT
	static const TCHAR* _cstrchr(const TCHAR* p, TCHAR ch)
	{
		return _tcschr(p, ch);
	}

	static TCHAR* _cstrrev(TCHAR* pStr)
	{
		return _tcsrev(pStr);
	}

	static const TCHAR* _cstrstr(const TCHAR* pStr, const TCHAR* pCharSet)
	{
		return _tcsstr(pStr, pCharSet);
	}

	static int _cstrspn(const TCHAR* pStr, const TCHAR* pCharSet)
	{
		return (int)_tcsspn(pStr, pCharSet);
	}

	static int _cstrcspn(const TCHAR* pStr, const TCHAR* pCharSet)
	{
		return (int)_tcscspn(pStr, pCharSet);
	}

	static const TCHAR* _cstrpbrk(const TCHAR* p, const TCHAR* lpszCharSet)
	{
		return _tcspbrk(p, lpszCharSet);
	}

	static int _cstrcmp(const TCHAR* pstrOne, const TCHAR* pstrOther)
	{
		return _tcscmp(pstrOne, pstrOther);
	}

	static int _cstrcmpi(const TCHAR* pstrOne, const TCHAR* pstrOther)
	{
		return _tcsicmp(pstrOne, pstrOther);
	}

#ifndef _WIN32_WCE
	static int _cstrcoll(const TCHAR* pstrOne, const TCHAR* pstrOther)
	{
		return _tcscoll(pstrOne, pstrOther);
	}

	static int _cstrcolli(const TCHAR* pstrOne, const TCHAR* pstrOther)
	{
		return _tcsicoll(pstrOne, pstrOther);
	}
#endif // !_WIN32_WCE
#endif // !_ATL_MIN_CRT

	static const TCHAR* _cstrrchr(const TCHAR* p, TCHAR ch)
	{
		return MinCrtHelper::_strrchr(p, ch);
	}

	static int _cstrisdigit(TCHAR ch)
	{
		return MinCrtHelper::_isdigit(ch);
	}

	static int _cstrisspace(TCHAR ch)
	{
		return MinCrtHelper::_isspace(ch);
	}

	static int _cstrtoi(const TCHAR* nptr)
	{
		return MinCrtHelper::_atoi(nptr);
	}

	static const TCHAR* _cstrchr_db(const TCHAR* p, TCHAR ch1, TCHAR ch2)
	{
		const TCHAR* lpsz = NULL;
		while (*p != 0)
		{
			if (*p == ch1 && *(p + 1) == ch2)
			{
				lpsz = p;
				break;
			}
			p = ::CharNext(p);
		}
		return lpsz;
	}
};


// Compare helpers

inline bool __stdcall operator ==(const CString& s1, const CString& s2)
{ return s1.Compare(s2) == 0; }

inline bool __stdcall operator ==(const CString& s1, LPCTSTR s2)
{ return s1.Compare(s2) == 0; }

inline bool __stdcall operator ==(LPCTSTR s1, const CString& s2)
{ return s2.Compare(s1) == 0; }

inline bool __stdcall operator !=(const CString& s1, const CString& s2)
{ return s1.Compare(s2) != 0; }

inline bool __stdcall operator !=(const CString& s1, LPCTSTR s2)
{ return s1.Compare(s2) != 0; }

inline bool __stdcall operator !=(LPCTSTR s1, const CString& s2)
{ return s2.Compare(s1) != 0; }

inline bool __stdcall operator <(const CString& s1, const CString& s2)
{ return s1.Compare(s2) < 0; }

inline bool __stdcall operator <(const CString& s1, LPCTSTR s2)
{ return s1.Compare(s2) < 0; }

inline bool __stdcall operator <(LPCTSTR s1, const CString& s2)
{ return s2.Compare(s1) > 0; }

inline bool __stdcall operator >(const CString& s1, const CString& s2)
{ return s1.Compare(s2) > 0; }

inline bool __stdcall operator >(const CString& s1, LPCTSTR s2)
{ return s1.Compare(s2) > 0; }

inline bool __stdcall operator >(LPCTSTR s1, const CString& s2)
{ return s2.Compare(s1) < 0; }

inline bool __stdcall operator <=(const CString& s1, const CString& s2)
{ return s1.Compare(s2) <= 0; }

inline bool __stdcall operator <=(const CString& s1, LPCTSTR s2)
{ return s1.Compare(s2) <= 0; }

inline bool __stdcall operator <=(LPCTSTR s1, const CString& s2)
{ return s2.Compare(s1) >= 0; }

inline bool __stdcall operator >=(const CString& s1, const CString& s2)
{ return s1.Compare(s2) >= 0; }

inline bool __stdcall operator >=(const CString& s1, LPCTSTR s2)
{ return s1.Compare(s2) >= 0; }

inline bool __stdcall operator >=(LPCTSTR s1, const CString& s2)
{ return s2.Compare(s1) <= 0; }


// CString "operator +" functions

inline CString __stdcall operator +(const CString& string1, const CString& string2)
{
	CString s;
	s.ConcatCopy(string1.GetData()->nDataLength, string1.m_pchData, string2.GetData()->nDataLength, string2.m_pchData);
	return s;
}

inline CString __stdcall operator +(const CString& string, TCHAR ch)
{
	CString s;
	s.ConcatCopy(string.GetData()->nDataLength, string.m_pchData, 1, &ch);
	return s;
}

inline CString __stdcall operator +(TCHAR ch, const CString& string)
{
	CString s;
	s.ConcatCopy(1, &ch, string.GetData()->nDataLength, string.m_pchData);
	return s;
}

#ifdef _UNICODE
inline CString __stdcall operator +(const CString& string, char ch)
{
	return string + (TCHAR)ch;
}

inline CString __stdcall operator +(char ch, const CString& string)
{
	return (TCHAR)ch + string;
}
#endif // _UNICODE

inline CString __stdcall operator +(const CString& string, LPCTSTR lpsz)
{
	ATLASSERT(lpsz == NULL || CString::_IsValidString(lpsz));
	CString s;
	s.ConcatCopy(string.GetData()->nDataLength, string.m_pchData, CString::SafeStrlen(lpsz), lpsz);
	return s;
}

inline CString __stdcall operator +(LPCTSTR lpsz, const CString& string)
{
	ATLASSERT(lpsz == NULL || CString::_IsValidString(lpsz));
	CString s;
	s.ConcatCopy(CString::SafeStrlen(lpsz), lpsz, string.GetData()->nDataLength, string.m_pchData);
	return s;
}


////////////////////////////////////////////////////////////////////////////////
#endif // !_WTL_NO_CSTRING