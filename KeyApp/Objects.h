#ifndef OBJECTS_HEADER
#define OBJECTS_HEADER

#include <commctrl.h>
#include "stdafx.h"

class CObject
{
public:
	CObject();
	virtual ~CObject();
	void Position(int x, int y);
	HWND ReturnObjecthWnd() { return m_hwnd; }
	void SetText(LPCTSTR lpString) { SetWindowText(m_hwnd, lpString); }

private:
	HWND m_hwnd;

protected:
	int X;
	int Y;
	BOOL CreateObject(int Width, int Height, DWORD dwExStyle, LPCTSTR lpClassName, LPCTSTR lpWindowName, DWORD dwStyle, HWND hWndParent, HMENU hMenu);

	LPCTSTR   lpClassName;
	DWORD dwExStyle;
};

CObject::CObject()
{


}

CObject::~CObject()
{


}

BOOL CObject::CreateObject(int Width, int Height,DWORD dwExStyle, LPCTSTR lpClassName, LPCTSTR lpWindowName, DWORD dwStyle, HWND hWndParent, HMENU hMenu)
{
	m_hwnd = CreateWindowEx(dwExStyle, lpClassName, lpWindowName, dwStyle, X, Y, Width, Height, hWndParent, hMenu, GetModuleHandle(NULL), NULL);

	if (m_hwnd == NULL) { MessageBox(NULL, L"Hwnd Error", _T("CWinException thrown"), MB_ICONERROR); return FALSE; }

	return TRUE;
}

void CObject::Position(int x, int y)
{
	X = x;
	Y = y;

}


#endif