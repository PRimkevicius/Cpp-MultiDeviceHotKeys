#include "Objects.h"
#include"Resource.h"


class CStatusBar : public CObject
{
	CStatusBar();
	virtual ~CStatusBar();
	BOOL Create(LPCTSTR Text, HWND hWnd, int menu, int width, int height);

};
CStatusBar::CStatusBar() {}
CStatusBar::~CStatusBar() {}

BOOL CStatusBar::Create(LPCTSTR Text, HWND hWnd, int menu, int width, int height)
{
	DWORD dwExStyle = 0;
	LPCTSTR lpClassName = L"BUTTON";
	DWORD dwStyle = BS_GROUPBOX | WS_CHILD | WS_VISIBLE;
	HMENU hMenu = (HMENU)menu;


	if (!CreateObject(width, height, dwExStyle, lpClassName, Text, dwStyle, hWnd, hMenu)) return FALSE;

	int FontSize = 15;
	LPCTSTR FontName = L"Arial";

	HFONT hFont = CreateFont(FontSize, 0, 0, 0, FALSE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, FontName);
	SendMessage(ReturnObjecthWnd(), WM_SETFONT, WPARAM(hFont), TRUE);

	return TRUE;
}