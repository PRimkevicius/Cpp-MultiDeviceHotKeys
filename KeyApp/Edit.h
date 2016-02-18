#pragma once
#include "Objects.h"
#include "Resource.h"

class CEdit : public CObject
{
public:
	CEdit();
	virtual ~CEdit();
	BOOL Create(LPCTSTR Text, HWND hWnd, int menu, int width, int height);
private:

};


CEdit::CEdit()
{


}
CEdit::~CEdit()
{


}

BOOL CEdit::Create(LPCTSTR Text, HWND hWnd, int menu, int width, int height)
{
	DWORD dwExStyle = WS_EX_CLIENTEDGE;
	LPCTSTR lpClassName = L"EDIT";
	DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL;
	HMENU hMenu = (HMENU)menu;

	return CreateObject(width, height, dwExStyle, lpClassName, Text, dwStyle, hWnd, hMenu);
}