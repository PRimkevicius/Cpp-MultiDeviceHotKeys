#include "Action_ShortCut.h"
#include "Action_OpenFileorFolderr.h"

BOOL CALLBACK		EnumWindowsProcForWinFocus(HWND hwnd, LPARAM lParam);
class CAction
{
public:

	CAction();
	virtual ~CAction();
	void OpenLocation(const WCHAR *loc) { ShellExecute(NULL, L"open", loc, NULL, NULL, SW_SHOW);}
	void PasteText(const WCHAR *text) { MessageBox(NULL, L"paste", L"event", MB_OK); }
	void ShortCut(wstring scut) { AShortCut::ExecuteShortCut(scut); }
	void FocusWindow(const WCHAR *title) { EnumWindows(EnumWindowsProcForWinFocus, (LPARAM)title);  }
	void WindowComand(int comand) { ShowWindow(GetForegroundWindow(), comand); }
	void WindowClose() { PostMessage(GetForegroundWindow(), WM_CLOSE, 0, 0); }
	void WindowTopMost();
};
CAction::CAction() {}
CAction::~CAction() {}
void CAction::WindowTopMost()
{
	if ((GetWindowLong(GetForegroundWindow(), GWL_EXSTYLE) & WS_EX_TOPMOST) != 0)
	{
		MessageBox(NULL, L"disable", L"Focus window", MB_OK);
		SetWindowPos(GetForegroundWindow(), HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	}
		
	else
	{
		MessageBox(NULL, L"Enable", L"Focus window", MB_OK);
		SetWindowPos(GetForegroundWindow(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

	}

}
 