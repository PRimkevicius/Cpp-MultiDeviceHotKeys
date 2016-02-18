


#include "Objects.h"
#include "Resource.h"


class CButton : public CObject
{
public:
	CButton();
	virtual ~CButton();
	BOOL Create(LPCTSTR Text, HWND hWnd, int menu, int width, int height);
	BOOL SetFont(int FontSize, WCHAR* text);
	BOOL Enabled(BOOL value);
private:

};


CButton::CButton()
{

}
CButton::~CButton()
{


}
BOOL CButton::Enabled(BOOL value)
{
	return EnableWindow(ReturnObjecthWnd(), value);
}
BOOL CButton::Create(LPCTSTR Text, HWND hWnd, int menu, int width, int height)
{

	DWORD dwExStyle = 0;
	LPCTSTR lpClassName = L"BUTTON";
	DWORD dwStyle = WS_TABSTOP | WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON;
	HMENU hMenu = (HMENU)menu;

	return CreateObject(width, height, dwExStyle, lpClassName, Text, dwStyle, hWnd, hMenu);
} 
BOOL CButton::SetFont(int FontSize, WCHAR* font)
{
	HFONT hFont = CreateFont(FontSize, 0, 0, 0, FALSE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, font);
	SendMessage(ReturnObjecthWnd(), WM_SETFONT, WPARAM(hFont), TRUE);
	return TRUE;
}