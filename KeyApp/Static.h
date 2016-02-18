#include "Objects.h"
#include"Resource.h"

class CStatic : public CObject
{
public:
	CStatic();
	virtual ~CStatic();
	BOOL Create(LPCTSTR Text, HWND hWnd, int menu, int width, int height);
	HFONT SetStaticFont(int Height, int Weight, DWORD Italic, DWORD Underline, DWORD StrikeOut, LPCTSTR FontName);
	DWORD dwStyle = WS_CHILD | WS_VISIBLE;
private:

};


CStatic::CStatic()
{


}
CStatic::~CStatic()
{


}

BOOL CStatic::Create(LPCTSTR Text, HWND hWnd, int menu, int width, int height)
{
	DWORD dwExStyle = 0;
	LPCTSTR lpClassName = L"STATIC";

	HMENU hMenu = (HMENU)menu;
	if (!CreateObject(width, height, dwExStyle, lpClassName, Text, dwStyle, hWnd, hMenu)) return FALSE;
	SetStaticFont(16, FW_NORMAL, FALSE, FALSE, FALSE, L"Arial");

	return TRUE;
}
HFONT CStatic::SetStaticFont(int Height, int Weight, DWORD Italic, DWORD Underline, DWORD StrikeOut, LPCTSTR FontName)
{

	HFONT hFont = CreateFont(Height, 0, 0, 0, Weight, Italic, Underline, StrikeOut, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, FontName);
	SendMessage(ReturnObjecthWnd(), WM_SETFONT, WPARAM(hFont), TRUE);

	return hFont;
}