#include "windows.h"
#include <string>
#include <list>
#include <algorithm>

#include "Edit.h"
#include "Button.h"
#include "Static.h"
#include "GroupBox.h"
#include "TreeView.h"
#include "VirtualKeys.h"
#include "TxtFiles.h"
#include <wchar.h>
#include "KeyDataBase.h"
#include <shlobj.h>
#include <Commdlg.h>
#include "Action.h"

using namespace std;

class CCore
{
public:
	CCore();
	virtual ~CCore();

	CAction ExeAction;

	BOOL CreateFormObjects(HWND hWnd, HINSTANCE hinst);

	HBRUSH SetStaticObjectColor(HDC hdcStatic);

	HWND ReturnTreeViewHWND() { return TreeViewBox.ReturnObjecthWnd(); }
	BOOL ProcessCommand(int wmId);
	WCHAR* VirtualKey(USHORT VKEY) { return VirtualKeys::ReturnKeyValue(VKEY); }


	wstring ReturnAltDeviceName(WCHAR* DID);

	void OnExitSaveData(){ CData::SaveData(LDevices, SaveFileName); }

	int CheckSelection(HTREEITEM& item);
	BOOL SetTextToRenemaDialog(HTREEITEM& item);

	BOOL CheckForDialog();
	BOOL OnAddKeyToDataBase();
	BOOL OnCreateDialog(HWND dhwnd);
	BOOL OnTreeViewRClick();
	void OnDialogClose() { IsDialogOpen = FALSE;  OpenDialoghWnd = NULL; }
	BOOL OnAltKeyboardNameChange(HWND dhwnd);
	BOOL OnAddEditDialogChange(USHORT VKEY, USHORT KID, WCHAR* DID);
	BOOL OnInputDialogChange(USHORT VKEY, USHORT KID, WCHAR* DID);
	void OnTreeViewSelectionChange(LPARAM lParam);
	BOOL OnRemoveItem();

	void OnShowAllActiveWindows(WCHAR* buf);
		
	BOOL OnRenameDialogUpdateDevice(WCHAR *devname);

	BOOL CheckForAction(USHORT &VKey, WCHAR* DId, list<KeyNode*>::iterator &it);
	BOOL OnEditItemDialog();
	BOOL OnEditItem();
	void OnRenameObject();

	void OnHookBlockAction(list<KeyNode*>::iterator &ActionIt);
	
	void OnAddEditDialogComboBoxChange(HWND dialog,int id);
	void OnAddEditDialogComboBoxInitialize(HWND dialog, int id);
	
	BOOL ReturnAddEditPushButtonState();
	void DisableAddEditDlgControls(BOOL value);
	BOOL OnButtonFolderFilePath();

private:
	BOOL AddKeyToDataBase();
	BOOL AddKeyToList();
	void RedreshTreeView() { TreeViewBox.AddItemsToTV(LDevices); }

	BOOL SetSelectedTreeViewItem(int TVGN);

	BOOL ReturnIterators(list<Keys*>::iterator &dev, list<KeyNode*>::iterator &node);

	BOOL TestLOOPfoo();
	BOOL DeleteObjects();

	BOOL IsDialogOpen;
	HWND OpenDialoghWnd;
	HTREEITEM SelectedTreeViewItem;

	void CloseDialog() { SendMessage(OpenDialoghWnd, WM_COMMAND, IDCANCEL, 0); IsDialogOpen = FALSE; }

	BOOL SaveXML();

	HWND Maindhwnd;
	HINSTANCE hInst;

	int LastVirtualKey;
	WCHAR LastDeviceName[90];

	CEdit Edit1;
	CButton ButtonAdd;
	CButton ButtonDelete;
	CButton ButtonEdit;
	CButton ButtonImputs;

	std::list<Keys*> LDevices;

	CStatic LabelVKey;
	CStatic LabelKeyState;
	CStatic LabelDevName;
	CStatic LabelKeyValue;

	CTreeView TreeViewBox;

	string SaveFileName = "Data.txt";

	HTREEITEM root;



	void AddEditDialogHideAllControls(HWND dialog);

	
	void SetText(HWND dialog, int Control, WCHAR* text) { SetWindowText(GetDlgItem(dialog, Control), text); }
};


CCore::CCore()
{


}

CCore::~CCore()
{
	DeleteObjects();
}
BOOL CALLBACK		EnumWindowsProc(HWND hwnd, LPARAM lParam);

BOOL	CCore::OnButtonFolderFilePath()
{
	switch (SendMessage(GetDlgItem(OpenDialoghWnd, IDC_ADD_COMBOBOX_ACTION), CB_GETCURSEL, 0, 0) + ACTIONS + 1)
	{
	case ACTION_OPENFILE:
	{
			OPENFILENAME ofn;
			wchar_t szFile[100];
			// open a file name
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = NULL;
			ofn.lpstrFile = szFile;
			ofn.lpstrFile[0] = '\0';
			ofn.nMaxFile = sizeof(szFile);
			ofn.lpstrFilter = L"All\0*.*\0Text\0*.TXT\0";
			ofn.nFilterIndex = 1;
			ofn.lpstrFileTitle = NULL;
			ofn.nMaxFileTitle = 0;
			ofn.lpstrInitialDir = NULL;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
			GetOpenFileName(&ofn);
			wstring buf = ofn.lpstrFile;
			replace(buf.begin(), buf.end(), '\\', '/');
			
			SetWindowText(GetDlgItem(OpenDialoghWnd, IDC_ADD_EDIT_PATH), buf.c_str());
	}
		break;
	case ACTION_OPENFOLDER:
	{
			BROWSEINFO bi = { 0 };
			bi.lpszTitle = L"Pick a Directory";
			LPITEMIDLIST pidl = SHBrowseForFolder(&bi);
			if (pidl != 0)
			{
				// get the name of the folder
				TCHAR path[MAX_PATH];
				if (SHGetPathFromIDList(pidl, path))
				{
					SetWindowText(GetDlgItem(OpenDialoghWnd, IDC_ADD_EDIT_PATH), path);
				}

				// free memory used
				IMalloc * imalloc = 0;
				if (SUCCEEDED(SHGetMalloc(&imalloc)))
				{
					imalloc->Free(pidl);
					imalloc->Release();
				}
			}
	}
		break;
	default:
		return FALSE;
		break;
	}

	return TRUE;
}
void	CCore::OnHookBlockAction(list<KeyNode*>::iterator &ActionIt)
{


	switch ((*ActionIt)->ActionID)
	{
	case ACTION_SUPPRESS: return;
	case ACTION_PASTE: ExeAction.PasteText((*ActionIt)->Action.c_str()); return;
	case ACTION_OPENFILE:  ExeAction.OpenLocation((*ActionIt)->Action.c_str()); return;
	case ACTION_OPENFOLDER: ExeAction.OpenLocation((*ActionIt)->Action.c_str()); return;
	case ACTION_SHORTCUT: ExeAction.ShortCut((*ActionIt)->Action); return;
	case ACTION_WINDOWFOCUS: ExeAction.FocusWindow((*ActionIt)->Action.c_str()); return;
	case ACTION_WINDOWMINIMIZE: ExeAction.WindowComand(SW_MINIMIZE); return;
	case ACTION_WINDOWMAXIMIZE: ExeAction.WindowComand(SW_MAXIMIZE);  return;
	case ACTION_WINDOWRESTORE: ExeAction.WindowComand(SW_RESTORE);  return;
	case ACTION_WINDOWCLOSE: ExeAction.WindowClose();  return;
	case ACTION_WINDOWTOPMOST: ExeAction.WindowTopMost(); return;

	default: break;
		
	}
}
BOOL	CCore::ReturnAddEditPushButtonState()
{
	switch (IsDlgButtonChecked(OpenDialoghWnd, IDC_ADD_BUTTON_INPUT))
	{
	case BST_CHECKED: return TRUE;
	default: return FALSE;
	}
}

void	CCore::OnShowAllActiveWindows(WCHAR* buf)
{
	HWND cbhwnd = GetDlgItem(OpenDialoghWnd, IDC_ADD_CONBOBOX_WIN);
	SendMessage(cbhwnd, CB_ADDSTRING, 0, (LPARAM)buf);
	SendMessage(cbhwnd, CB_SETCURSEL, 0, 0);
}
void	CCore::OnAddEditDialogComboBoxInitialize(HWND dialog, int id)
{

	AddEditDialogHideAllControls(dialog);

	TCHAR buf[160];

	HWND cbhwnd = GetDlgItem(dialog, IDC_ADD_COMBOBOX_ACTION);
	for (size_t i = 1; i < 13; i++)
	{
		LoadString(hInst, ACTIONS+i, buf, sizeof(buf) / sizeof(TCHAR));
		SendMessage(cbhwnd, CB_ADDSTRING, 0, (LPARAM)buf);
	}

	SendMessage(cbhwnd, CB_SETCURSEL, 0, 0);

	//OnAddEditDialogComboBoxChange(id);
}
void	CCore::OnAddEditDialogComboBoxChange(HWND dialog,int id)
{
	AddEditDialogHideAllControls(dialog);

	switch (ACTIONS+1+id)
	{
	case ACTION_PASTE:
	{
		ShowWindow(GetDlgItem(dialog, IDC_ADD_EDIT_ACTION), SW_SHOW);
	}
		break;
	case ACTION_OPENFILE:
	{
		ShowWindow(GetDlgItem(dialog, IDC_ADD_BUTTON_SELECT), SW_SHOW);
		ShowWindow(GetDlgItem(dialog, IDC_ADD_EDIT_PATH), SW_SHOW);
	}
		break;
	case ACTION_OPENFOLDER:
	{
		ShowWindow(GetDlgItem(dialog, IDC_ADD_EDIT_PATH), SW_SHOW);
		ShowWindow(GetDlgItem(dialog, IDC_ADD_BUTTON_SELECT), SW_SHOW);
	}
		break;
	case ACTION_SHORTCUT:
	{
		ShowWindow(GetDlgItem(dialog, IDC_ADD_EDIT_SHORTCUT), SW_SHOW);
		ShowWindow(GetDlgItem(dialog, IDC_ADD_LABEL_SCINFO), SW_SHOW);
	}
		break;
	case ACTION_WINDOWFOCUS:
	{
		ShowWindow(GetDlgItem(dialog, IDC_ADD_CONBOBOX_WIN), SW_SHOW);
		EnumWindows(EnumWindowsProc, NULL);
	}
		break;
	default:
		break;
	}

	

}
void	CCore::AddEditDialogHideAllControls(HWND dialog)
{
	ShowWindow(GetDlgItem(dialog, IDC_ADD_CONBOBOX_WIN), SW_HIDE);
	ShowWindow(GetDlgItem(dialog, IDC_ADD_EDIT_ACTION), SW_HIDE);
	ShowWindow(GetDlgItem(dialog, IDC_ADD_EDIT_PATH), SW_HIDE);
	ShowWindow(GetDlgItem(dialog, IDC_ADD_EDIT_SHORTCUT), SW_HIDE);
	ShowWindow(GetDlgItem(dialog, IDC_ADD_LABEL_SCINFO), SW_HIDE);
	ShowWindow(GetDlgItem(dialog, IDC_ADD_BUTTON_SELECT), SW_HIDE);
	
	//SetWindowText(GetDlgItem(OpenDialoghWnd, IDC_AD_TEXTBOXSHORTCUT), L"");
	//SetWindowText(GetDlgItem(OpenDialoghWnd, IDC_AD_TEXTBOXPATH), L"");
	//SetWindowText(GetDlgItem(OpenDialoghWnd, IDC_AD_TEXTBOXACTION), L"");

}
BOOL	CCore::SetSelectedTreeViewItem(int TVGN)
{

	SelectedTreeViewItem = (HTREEITEM)SendDlgItemMessage(Maindhwnd, IDC_MAIN_TREEVIEW, TVM_GETNEXTITEM, TVGN, 0);
	SendDlgItemMessage(Maindhwnd, IDC_MAIN_TREEVIEW, TVM_SELECTITEM, TVGN_CARET, (LPARAM)SelectedTreeViewItem);

	if (SelectedTreeViewItem == NULL) return FALSE;

	TreeView_EnsureVisible(Maindhwnd, SelectedTreeViewItem);
	SendDlgItemMessage(Maindhwnd, IDC_MAIN_TREEVIEW, TVM_SELECTITEM, TVGN_CARET, (LPARAM)SelectedTreeViewItem);

	CheckSelection(SelectedTreeViewItem);

	return TRUE;
}
BOOL	CCore::CreateFormObjects(HWND hWnd, HINSTANCE hinst)
{
	Maindhwnd = hWnd;
	hInst = hinst;

	int x = 10;
	int y = 50;

	ButtonAdd.Position(5, 2);
	if (!ButtonAdd.Create(L"Add", Maindhwnd, IDC_MAIN_ADD, 50, 20)) return FALSE;
	ButtonAdd.SetFont(12, L"MS Sans Serif");

	ButtonEdit.Position(56, 2);
	if (!ButtonEdit.Create(L"Edit", Maindhwnd, IDC_MAIN_EDIT, 50, 20)) return FALSE;
	ButtonEdit.SetFont(12, L"MS Sans Serif");
	ButtonEdit.Enabled(FALSE);

	ButtonDelete.Position(107, 2);
	if (!ButtonDelete.Create(L"Delete", Maindhwnd, IDC_MAIN_DELETE, 50, 20)) return FALSE;
	ButtonDelete.SetFont(12, L"MS Sans Serif");
	ButtonDelete.Enabled(FALSE);

	ButtonImputs.Position(158, 2);
	if (!ButtonImputs.Create(L"Inputs", Maindhwnd, IDC_MAIN_INPUTS, 50, 20)) return FALSE;
	ButtonImputs.SetFont(12, L"MS Sans Serif");

	TreeViewBox.Position(5, 25);
	if (!TreeViewBox.Create(L"", Maindhwnd, IDC_MAIN_TREEVIEW, 400, 250, hInst)) return FALSE;


	HMENU menu = GetMenu(Maindhwnd);
	EnableMenuItem(menu, IDM_EDIT, MF_GRAYED);
	EnableMenuItem(menu, IDM_DELETE, MF_GRAYED);
	EnableMenuItem(menu, IDM_RENAME, MF_GRAYED);

	CData::LoadData(LDevices, SaveFileName);

	TreeViewBox.AddItemsToTV(LDevices);
	return TRUE;
}

BOOL	CCore::CheckForAction(USHORT &VKey, WCHAR* DId, list<KeyNode*>::iterator &actionit)
{
	if (LDevices.size() != 0)
	{
		list<Keys*>::iterator it;
		for (it = LDevices.begin(); it != LDevices.end(); it++)
		{
			if (wcscmp((*it)->Device, DId) == 0)
			{
				list<KeyNode*>::iterator kit;
				for (kit = (*it)->KeyList.begin(); kit != (*it)->KeyList.end(); kit++)
				{
					if ((*kit)->VirtualKey == VKey)
					{
						actionit = kit;
						//MessageBox(NULL, L"suppresing", L"Error", MB_ICONERROR);
						return TRUE;
					}
				}
			}

		}
	}

	return FALSE;
}
void	CCore::OnTreeViewSelectionChange(LPARAM lParam)
{
	if (((LPNMTREEVIEW)lParam)->action == TVC_BYKEYBOARD)
	{
		if (!SetSelectedTreeViewItem(TVGN_CARET)) return;
		//SelectedTreeViewItem = (HTREEITEM)SendDlgItemMessage(Maindhwnd, IDC_MAIN_TREEVIEWBOX, TVM_GETNEXTITEM, TVGN_CARET, (LPARAM)SelectedTreeViewItem);
		//if (SelectedTreeViewItem == NULL) return;
		//TreeView_EnsureVisible(Maindhwnd, SelectedTreeViewItem);
		//SendDlgItemMessage(Maindhwnd, IDC_MAIN_TREEVIEWBOX, TVM_SELECTITEM, TVGN_CARET, (LPARAM)SelectedTreeViewItem);
	}
	if (((LPNMTREEVIEW)lParam)->action == TVC_BYMOUSE)
	{
		if (!SetSelectedTreeViewItem(TVGN_CARET)) return;
	}
}
BOOL	CCore::ReturnIterators(list<Keys*>::iterator &devIt, list<KeyNode*>::iterator &nodeIt)
{
	
	for (devIt = LDevices.begin(); devIt != LDevices.end(); devIt++)
	{
		if ((*devIt)->tviroot == SelectedTreeViewItem)
		{
			return TRUE;
		}
		for (nodeIt = (*devIt)->KeyList.begin(); nodeIt != (*devIt)->KeyList.end(); nodeIt++)
		{
			if ((*nodeIt)->tvikey == SelectedTreeViewItem) return TRUE;
		}
	}
	CloseDialog();
	return FALSE;
}
BOOL	CCore::OnEditItemDialog()
{
	if (LDevices.size() != 0 && SelectedTreeViewItem != NULL)
	{
		list<Keys*>::iterator devIt;
		list<KeyNode*>::iterator nodeIt;
		if (!ReturnIterators(devIt, nodeIt)) return FALSE;

		SetWindowText(GetDlgItem(OpenDialoghWnd, IDC_ADD_TEXT_DEVICE), (*devIt)->Device);
		SetWindowText(GetDlgItem(OpenDialoghWnd, IDC_ADD_TEXT_ALTNAME), (*devIt)->AltName.c_str());
		SetWindowText(GetDlgItem(OpenDialoghWnd, IDC_ADD_TEXT_KEYCODE), std::to_wstring((*nodeIt)->VirtualKey).c_str());
		SetWindowText(GetDlgItem(OpenDialoghWnd, IDC_ADD_TEXT_STATE), L"0");
		SetWindowText(GetDlgItem(OpenDialoghWnd, IDC_ADD_TEXT_VKEY), VirtualKey((*nodeIt)->VirtualKey));

		LastVirtualKey = (*nodeIt)->VirtualKey;
		wcsncpy_s(LastDeviceName,(*devIt)->Device, 90);
		int id = (*nodeIt)->ActionID - ACTIONS - 1;
		SendMessage(GetDlgItem(OpenDialoghWnd, IDC_ADD_COMBOBOX_ACTION), CB_SETCURSEL, id, 0);

		switch (ACTIONS + 1 + id)
		{
		case ACTION_PASTE: SetWindowText(GetDlgItem(OpenDialoghWnd, IDC_ADD_EDIT_ACTION), (*nodeIt)->Action.c_str()); break;
		case ACTION_OPENFILE: SetWindowText(GetDlgItem(OpenDialoghWnd, IDC_ADD_EDIT_PATH), (*nodeIt)->Action.c_str()); break;
		case ACTION_OPENFOLDER: SetWindowText(GetDlgItem(OpenDialoghWnd, IDC_ADD_EDIT_PATH), (*nodeIt)->Action.c_str()); break;
		case ACTION_SHORTCUT: SetWindowText(GetDlgItem(OpenDialoghWnd, IDC_ADD_EDIT_SHORTCUT), (*nodeIt)->Action.c_str()); break;
		default: break;
		}

		OnAddEditDialogComboBoxChange(OpenDialoghWnd, id);
	}
	return TRUE;
}
void	CCore::DisableAddEditDlgControls(BOOL value)
{
	EnableWindow(GetDlgItem(OpenDialoghWnd, IDC_ADD_TEXT_DEVICE), value);
	EnableWindow(GetDlgItem(OpenDialoghWnd, IDC_ADD_TEXT_ALTNAME), value);
	EnableWindow(GetDlgItem(OpenDialoghWnd, IDC_ADD_TEXT_KEYCODE), value);
	EnableWindow(GetDlgItem(OpenDialoghWnd, IDC_ADD_TEXT_STATE), value);
	EnableWindow(GetDlgItem(OpenDialoghWnd, IDC_ADD_TEXT_VKEY), value);
}
BOOL	CCore::OnEditItem()
{

	if (LDevices.size() != 0)
	{

		list<Keys*>::iterator devIt;
		list<KeyNode*>::iterator nodeIt;
		if (!ReturnIterators(devIt, nodeIt)) return FALSE;

		int msg = MessageBox(NULL, L"overrite data?", L"Error", MB_YESNO | MB_ICONERROR);
		if (msg == IDYES)
		{
			(*nodeIt)->VirtualKey = LastVirtualKey;
			(*nodeIt)->Action = L"some kind of action";
			RedreshTreeView();
		}
		else return FALSE;
	}


	return TRUE;
}

BOOL	CCore::OnRemoveItem()
{
	TV_ITEM tvi;

	WCHAR Text[255];
	memset(&tvi, 0, sizeof(tvi));

	tvi.mask = TVIF_TEXT;
	tvi.pszText = Text;
	tvi.cchTextMax = 256;
	tvi.hItem = SelectedTreeViewItem;


	SendDlgItemMessage(Maindhwnd, IDC_MAIN_TREEVIEW, TVM_GETITEM, TVGN_CARET, (LPARAM)&tvi);

	//SendDlgItemMessage(Maindhwnd, IDC_MAIN_TREEVIEWBOX, TVM_DELETEITEM, TVGN_CARET, (LPARAM)tvi.hItem);


	list<Keys*>::iterator it;
	for (it = LDevices.begin(); it != LDevices.end(); it++)
	{
		if ((*it)->tviroot == SelectedTreeViewItem)
		{

			WCHAR buf[64];
			swprintf_s(buf, 64, L"Do you want to remove all \"%s \" items?", (*it)->AltName.c_str());
			int msg = MessageBox(NULL, buf, L"?", MB_YESNOCANCEL | MB_ICONINFORMATION);
			if (msg == IDYES)
			{
				delete (*it);
				LDevices.erase(it);
			}
			RedreshTreeView();
			return TRUE;
		}
		list<KeyNode*>::iterator kit;
		for (kit = (*it)->KeyList.begin(); kit != (*it)->KeyList.end(); kit++)
		{
			


			if ((*kit)->tvikey == SelectedTreeViewItem)
			{
				WCHAR buf[64];
				swprintf_s(buf, 64, L"Do you want to remove: %s - %s ?", VirtualKey((*kit)->VirtualKey), (*kit)->Action.c_str());
				int msg = MessageBox(NULL, buf, L"?", MB_YESNOCANCEL | MB_ICONINFORMATION);
				if (msg == IDYES)
				{
					delete (*kit);
					(*it)->KeyList.erase(kit);
					
				}
				RedreshTreeView();
				return TRUE;
			}
		}
	}
	
	return TRUE;
}
BOOL	CCore::CheckForDialog()
{
	if (IsDialogOpen)  CloseDialog(); 
	return IsDialogOpen;
}
BOOL	CCore::OnInputDialogChange(USHORT VKEY, USHORT KID, WCHAR* DID)
{
	SetWindowText(GetDlgItem(OpenDialoghWnd, IDC_INPUT_TEXT_DEVICE), DID);
	SetWindowText(GetDlgItem(OpenDialoghWnd, IDC_INPUT_TEXT_KEYCODE), std::to_wstring(VKEY).c_str());
	SetWindowText(GetDlgItem(OpenDialoghWnd, IDC_INPUT_TEXT_STATE), std::to_wstring(KID).c_str());
	SetWindowText(GetDlgItem(OpenDialoghWnd, IDC_INPUT_TEXT_VKEY), VirtualKey(VKEY));
	return TRUE;
}
BOOL	CCore::OnCreateDialog(HWND dhwnd)
{

	IsDialogOpen = TRUE;
	OpenDialoghWnd = dhwnd;
	LastVirtualKey = NULL;
	return TRUE;
}
BOOL	CCore::OnTreeViewRClick()
{
	if (!SetSelectedTreeViewItem(TVGN_DROPHILITE)) return FALSE;


	POINT point;
	GetCursorPos(&point);

	HMENU hMenu = CreatePopupMenu();

	switch (CheckSelection(SelectedTreeViewItem))
	{
	case 1:
	{
		AppendMenuW(hMenu, MF_STRING, IDCM_RENAME, L"&Rename");
		AppendMenuW(hMenu, MF_STRING, IDCM_DELETE, L"&Delete");
		break;
	}
	case 2:
	{
		AppendMenuW(hMenu, MF_STRING, IDCM_EDIT, L"&Edit");
		AppendMenuW(hMenu, MF_STRING, IDCM_DELETE, L"&Delete");
		break;
	}
	}
	TrackPopupMenu(hMenu, TPM_RIGHTBUTTON, point.x, point.y, 0, Maindhwnd, NULL);
	DestroyMenu(hMenu);
	return TRUE;
}
BOOL	CCore::OnAltKeyboardNameChange(HWND dhwnd)
{
	int length = GetWindowTextLength(GetDlgItem(dhwnd, IDC_RENAME_EDIT));
	if (length == 0) return FALSE;
	wstring altname;
	WCHAR* buf = new WCHAR[length+1];
	GetDlgItemText(dhwnd, IDC_RENAME_EDIT, buf, length+1);
	altname.append(buf);


	list<Keys*>::iterator it;
	for (it = LDevices.begin(); it != LDevices.end(); it++)
	{
		if (SelectedTreeViewItem == (*it)->tviroot) break;
	}

	(*it)->AltName.assign(altname);
	delete buf;

	length = GetWindowTextLength(GetDlgItem(dhwnd, IDC_RENAME_BUTTON_DEVN));

	WCHAR* buf2 = new WCHAR[length + 1];
	GetDlgItemText(dhwnd, IDC_RENAME_BUTTON_DEVN, buf2, length + 1);


	if (wcscmp((*it)->Device, buf2) != 0) wcsncpy_s((*it)->Device, buf2, 90);

	TreeViewBox.AddItemsToTV(LDevices);
	
	delete buf2;

	CData::SaveData(LDevices, SaveFileName);
	return TRUE;
}
int		CCore::CheckSelection(HTREEITEM& item)
{

	HMENU menu = GetMenu(Maindhwnd);

	list<Keys*>::iterator it;
	for (it = LDevices.begin(); it != LDevices.end(); it++)
	{
		if (item == (*it)->tviroot)
		{
			ButtonEdit.Enabled(FALSE);
			ButtonDelete.Enabled(TRUE);
			EnableMenuItem(menu, IDM_EDIT, MF_GRAYED);
			EnableMenuItem(menu, IDM_DELETE, MF_ENABLED);
			EnableMenuItem(menu, IDM_RENAME, MF_ENABLED);
			return 1;
		}
		list<KeyNode*>::iterator kit;

		for (kit = (*it)->KeyList.begin(); kit != (*it)->KeyList.end(); kit++)
		{
			if ((*kit)->tvikey == item)
			{
				ButtonDelete.Enabled(TRUE);
				ButtonEdit.Enabled(TRUE);
				EnableMenuItem(menu, IDM_EDIT, MF_ENABLED);
				EnableMenuItem(menu, IDM_DELETE, MF_ENABLED);
				EnableMenuItem(menu, IDM_RENAME, MF_GRAYED);
				return 2;
			}
		}
	}
	ButtonDelete.Enabled(FALSE);
	ButtonEdit.Enabled(FALSE);
	EnableMenuItem(menu, IDM_EDIT, MF_GRAYED);
	EnableMenuItem(menu, IDM_DELETE, MF_GRAYED);
	EnableMenuItem(menu, IDM_RENAME, MF_GRAYED);
	return 0;
}

HBRUSH	CCore::SetStaticObjectColor(HDC hdcStatic)
{
	//HBRUSH BackgroundBrush = CreateSolidBrush(RGB(190,190,190));

	SetTextColor(hdcStatic, RGB(255, 0, 0));
	SetBkMode(hdcStatic, TRANSPARENT);

	return 0;
}
BOOL	CCore::ProcessCommand(int wmId)
{
	switch (wmId)
	{
	case IDM_SAVEDATA: CData::SaveData(LDevices, SaveFileName);  break;
	case IDM_LOADDATA: DeleteObjects(); CData::LoadData(LDevices, SaveFileName); RedreshTreeView(); break;
	case IDM_CEAR_ALL:
	{
		int msgid = MessageBox(NULL, L"Delete all data?", L"!", MB_YESNO | MB_ICONINFORMATION);
		if (msgid == IDYES)
		{
			DeleteObjects();
			RedreshTreeView();
		}
		break;
	}

	default: return FALSE;
	}

	return TRUE;
}
BOOL	CCore::AddKeyToList()
{
	WCHAR text[256];
	//std::wstring test(LastDeviceName);
	//LDevices.push_back(test);
	if (LDevices.size() != 0)
	{
		list<Keys*>::iterator it;
		for (it = LDevices.begin(); it != LDevices.end(); it++)
		{
			swprintf_s(text, 256, L"%s %s", (*it)->Device, LastDeviceName);
			if (wcscmp((*it)->Device, LastDeviceName) == 0)
			{

				list<KeyNode*>::iterator kit;
				for (kit = (*it)->KeyList.begin(); kit != (*it)->KeyList.end(); kit++)
				{
					if ((*kit)->VirtualKey == LastVirtualKey)
					{
						MessageBox(NULL, L"This device ant key already exist in database", L"Error", MB_ICONERROR);
						return FALSE;
					}
				}

				KeyNode *node = new KeyNode;
				node->VirtualKey = LastVirtualKey;

				node->Action = L"Keyboard";
				node->Action.append(std::to_wstring(LDevices.size()).c_str());

				(*it)->KeyList.push_back(node);
				return TRUE;
			}
		}
	}


	

	KeyNode *node = new KeyNode;
	node->VirtualKey = LastVirtualKey;
	node->Action = L"Keyboard";
	node->Action.append(std::to_wstring(LDevices.size()).c_str());
		

	Keys *key = new Keys;

	wcsncpy_s(key->Device, LastDeviceName, 90);

	key->KeyList.push_back(node);
	LDevices.push_back(key);


	return TRUE;
}
BOOL	CCore::TestLOOPfoo()
{
	WCHAR text[128];

	if (LDevices.size() != 0)
	{ 
		list<Keys*>::iterator it;
		for (it = LDevices.begin(); it != LDevices.end(); it++)
		{
			list<KeyNode*>::iterator kit;
			for (kit = (*it)->KeyList.begin(); kit != (*it)->KeyList.end(); kit++)
			{
				swprintf_s(text, 128, L"%s %X | %s", (*it)->Device,(*kit)->VirtualKey,(*kit)->Action.c_str());
				MessageBox(NULL, text, L"teste", MB_ICONERROR);
			}
		}
	}
	
	return TRUE;
}
BOOL	CCore::DeleteObjects()
{
	
	if (LDevices.size() == 0) return TRUE;
	list<Keys*>::iterator it;
	for (it = LDevices.begin(); it != LDevices.end(); it++)
	{
		delete (*it);
	}
	LDevices.clear();
	return TRUE;
}
BOOL	CCore::OnAddEditDialogChange(USHORT VKEY, USHORT KID, WCHAR* DID)
{
	LastVirtualKey = VKEY;
	wcsncpy_s(LastDeviceName, DID, 90);

	SetWindowText(GetDlgItem(OpenDialoghWnd, IDC_ADD_TEXT_DEVICE), DID);
	SetWindowText(GetDlgItem(OpenDialoghWnd, IDC_ADD_TEXT_ALTNAME), ReturnAltDeviceName(DID).c_str());
	SetWindowText(GetDlgItem(OpenDialoghWnd, IDC_ADD_TEXT_KEYCODE), std::to_wstring(VKEY).c_str());
	SetWindowText(GetDlgItem(OpenDialoghWnd, IDC_ADD_TEXT_STATE), std::to_wstring(KID).c_str());
	SetWindowText(GetDlgItem(OpenDialoghWnd, IDC_ADD_TEXT_VKEY), VirtualKey(VKEY));

	CheckDlgButton(OpenDialoghWnd, IDC_ADD_BUTTON_INPUT, BST_UNCHECKED);
	return TRUE;
}
wstring CCore::ReturnAltDeviceName(WCHAR* DID)
{
	if (LDevices.size() != 0)
	{
		list<Keys*>::iterator it;
		for (it = LDevices.begin(); it != LDevices.end(); it++)
		{
			if (wcscmp((*it)->Device, DID) == 0)
			{
				return (*it)->AltName;
			}
		}

	}

	wstring text = L"Keyboard";
	
	return  text.append(std::to_wstring(LDevices.size()+1));
}
BOOL	CCore::OnAddKeyToDataBase()
{
	BOOL rez = AddKeyToDataBase();

	RedreshTreeView();
	CData::SaveData(LDevices, SaveFileName);
	return rez;
}
BOOL	CCore::AddKeyToDataBase()
{
	if (LastVirtualKey == 0) 
	{ 
		MessageBox(NULL, L"Please, choose a prefered key", L"Error", MB_OK | MB_ICONINFORMATION); 
		return FALSE;
	}
	if (!CDB_Actions::CheckActionTextBox(OpenDialoghWnd))  return FALSE;

	list<Keys*>::iterator kit;
	list<KeyNode*>::iterator knit;
	if (LDevices.size() != 0)
	{
		for (kit = LDevices.begin(); kit != LDevices.end(); kit++)
		{
			if (wcscmp((*kit)->Device, LastDeviceName) == 0)
			{
				
				for (knit = (*kit)->KeyList.begin(); knit != (*kit)->KeyList.end(); knit++)
				{
					if ((*knit)->VirtualKey == LastVirtualKey)
					{
						int msgid = MessageBox(NULL, L"This device ant key already exist in database, overrite?", L"Error", MB_YESNO|MB_ICONERROR);

						if (msgid == IDYES)
						{
							CDB_Actions::UpdateKeyNodeItem(OpenDialoghWnd, LastVirtualKey, knit); 
							return TRUE;
						}
						else return FALSE;
					}
				}
				KeyNode *node = CDB_Actions::CreateNewKeyNodeItem(OpenDialoghWnd, LastVirtualKey);
				(*kit)->KeyList.push_back(node);
				return TRUE;
			}
		}
	}
	
	KeyNode *node = CDB_Actions::CreateNewKeyNodeItem(OpenDialoghWnd, LastVirtualKey);
	Keys * dev = CreateNewDeviceItem(LastDeviceName, LDevices.size());

	dev->KeyList.push_back(node);
	LDevices.push_back(dev);
	return TRUE;

}
BOOL	CCore::OnRenameDialogUpdateDevice(WCHAR *devname)
{
	if (IsDlgButtonChecked(OpenDialoghWnd, IDC_RENAME_BUTTON_DEVN) == BST_CHECKED)
	{
		wcsncpy_s(LastDeviceName, devname, 90);
		SetWindowText(GetDlgItem(OpenDialoghWnd, IDC_RENAME_BUTTON_DEVN), devname);
		CheckDlgButton(OpenDialoghWnd, IDC_RENAME_BUTTON_DEVN, BST_UNCHECKED);
		return TRUE;
	}
	else return FALSE;

}
void	CCore::OnRenameObject()
{ 

	list<Keys*>::iterator it;
	for (it = LDevices.begin(); it != LDevices.end(); it++)
	{
		if (SelectedTreeViewItem == (*it)->tviroot) break;
	}
	
	SetWindowText(GetDlgItem(OpenDialoghWnd, IDC_RENAME_EDIT), (*it)->AltName.c_str());
	SetWindowText(GetDlgItem(OpenDialoghWnd, IDC_RENAME_BUTTON_DEVN), (*it)->Device);
}





















//int length = GetWindowTextLength(GetDlgItem(dhwnd, IDC_AD_EDITTEXTALTNAME));
//wstring altname;
//if (length == 0)
//{
//	altname.append(L"Keyboard").append(std::to_wstring(LDevices.size() + 1));
//}
//else
//{
//	WCHAR* buf = new WCHAR[length];
//	GetDlgItemText(dhwnd, IDC_AD_EDITTEXTALTNAME, buf, length + 1);
//	altname.append(buf);
//	delete buf;
//}




//std::wstring test(LastDeviceName);
//LDevices.push_back(test);





/*


case WM_INPUT:
{
UINT bufferSize;

// Prepare buffer for the data
GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &bufferSize, sizeof(RAWINPUTHEADER));
LPBYTE dataBuffer = new BYTE[bufferSize];
// Load data into the buffer
GetRawInputData((HRAWINPUT)lParam, RID_INPUT, dataBuffer, &bufferSize, sizeof(RAWINPUTHEADER));

RAWINPUT* raw = (RAWINPUT*)dataBuffer;

// Get the virtual key code of the key and report it
USHORT virtualKeyCode = raw->data.keyboard.VKey;
USHORT keyPressed = raw->data.keyboard.Flags & RI_KEY_BREAK ? 0 : 1;
WCHAR text[128];
swprintf_s(text, 128, L"Raw Input: %X (%d)\n", virtualKeyCode, keyPressed);
OutputDebugString(text);




// Prepare string buffer for the device name
GetRawInputDeviceInfo(raw->header.hDevice, RIDI_DEVICENAME, NULL, &bufferSize);
WCHAR* stringBuffer = new WCHAR[bufferSize];




// Load the device name into the buffer
GetRawInputDeviceInfo(raw->header.hDevice, RIDI_DEVICENAME, stringBuffer, &bufferSize);

swprintf_s(text, 128, L"string buffer  %s \n", stringBuffer);
OutputDebugString(text);

swprintf_s(text, 128, L"device name    %s \n", numericKeyboardDeviceName);
OutputDebugString(text);



if (IsInputDialogOpen)  Core.OnInputDialogChange(virtualKeyCode, keyPressed, stringBuffer);
if (IsAddEditDialogOpen) Core.OnAddEditDialogChange(virtualKeyCode, keyPressed, stringBuffer);


// Check whether the key struck was a L"7" on a numeric keyboard, and remember the decision whether to block the input
if (virtualKeyCode == 0x67 && wcscmp(stringBuffer, numericKeyboardDeviceName) == 0)
{
decisionBuffer.push_back(DecisionRecord(virtualKeyCode, TRUE));

swprintf_s(text, 128, L"true \n");
OutputDebugString(text);
}
else
{
decisionBuffer.push_back(DecisionRecord(virtualKeyCode, FALSE));
swprintf_s(text, 128, L"false \n");
OutputDebugString(text);
}

delete[] stringBuffer;
delete[] dataBuffer;
return 0;
}
break;
case WM_HOOK:
	{
	USHORT virtualKeyCode = (USHORT)wParam;
	USHORT keyPressed = lParam & 0x80000000 ? 0 : 1;
	WCHAR text[128];
	swprintf_s(text, 128, L"Hook: %X (%d)\n", virtualKeyCode, keyPressed);
	OutputDebugString(text);

	// Check the buffer if this Hook message is supposed to be blocked; return 1 if it is
	BOOL blockThisHook = FALSE;
	BOOL recordFound = FALSE;
	int index = 1;
	if (!decisionBuffer.empty())
	{
		// Search the buffer for the matching record
		std::deque<DecisionRecord>::iterator iterator = decisionBuffer.begin();
		while (iterator != decisionBuffer.end())
		{
			if (iterator->virtualKeyCode == virtualKeyCode)
			{
				blockThisHook = iterator->decision;
				recordFound = TRUE;
				// Remove this and all preceding messages from the buffer
				for (int i = 0; i < index; ++i)
				{
				decisionBuffer.pop_front();
				}
				// Stop looking
			break;
			}
		++iterator;
		++index;
		}
	}

	
	DWORD currentTime, startTime;
	startTime = GetTickCount();
while (!recordFound)
{
MSG rawMessage;
while (!PeekMessage(&rawMessage, mainHwnd, WM_INPUT, WM_INPUT, PM_REMOVE))
{
// Test for the maxWaitingTime
currentTime = GetTickCount();
// If current time is less than start, the time rolled over to 0
if ((currentTime < startTime ? ULONG_MAX - startTime + currentTime : currentTime - startTime) > maxWaitingTime)
{
// Ignore the Hook message, if it exceeded the limit
WCHAR text[128];
swprintf_s(text, 128, L"Hook TIMED OUT: %X (%d)\n", virtualKeyCode, keyPressed);
OutputDebugString(text);
return 0;
}
}

// The Raw Input message has arrived; decide whether to block the input
UINT bufferSize;

// Prepare buffer for the data
GetRawInputData((HRAWINPUT)rawMessage.lParam, RID_INPUT, NULL, &bufferSize, sizeof(RAWINPUTHEADER));
LPBYTE dataBuffer = new BYTE[bufferSize];
// Load data into the buffer
GetRawInputData((HRAWINPUT)rawMessage.lParam, RID_INPUT, dataBuffer, &bufferSize, sizeof(RAWINPUTHEADER));

RAWINPUT* raw = (RAWINPUT*)dataBuffer;

// Get the virtual key code of the key and report it
USHORT rawVirtualKeyCode = raw->data.keyboard.VKey;
USHORT rawKeyPressed = raw->data.keyboard.Flags & RI_KEY_BREAK ? 0 : 1;
WCHAR text[128];
swprintf_s(text, 128, L"Raw Input WAITING: %X (%d)\n", rawVirtualKeyCode, rawKeyPressed);
OutputDebugString(text);

// Prepare string buffer for the device name
GetRawInputDeviceInfo(raw->header.hDevice, RIDI_DEVICENAME, NULL, &bufferSize);
WCHAR* stringBuffer = new WCHAR[bufferSize];

// Load the device name into the buffer
GetRawInputDeviceInfo(raw->header.hDevice, RIDI_DEVICENAME, stringBuffer, &bufferSize);

// If the Raw Input message doesn't match the Hook, push it into the buffer and continue waiting
if (virtualKeyCode != rawVirtualKeyCode)
{
// Check whether the key struck was a L"7" on a numeric keyboard, and decide whether to block the input
if (rawVirtualKeyCode == 0x67 && wcscmp(stringBuffer, numericKeyboardDeviceName) == 0)
{
decisionBuffer.push_back(DecisionRecord(rawVirtualKeyCode, TRUE));
}
else
{
decisionBuffer.push_back(DecisionRecord(rawVirtualKeyCode, FALSE));
}
}
else
{
// This is correct Raw Input message
recordFound = TRUE;
//	blockThisHook = Core.CheckForAction(rawVirtualKeyCode, stringBuffer);
// Check whether the key struck was a L"7" on a numeric keyboard, and decide whether to block the input
if (rawVirtualKeyCode == 0x67 && wcscmp(stringBuffer, numericKeyboardDeviceName) == 0)
{
blockThisHook = TRUE;
}
else
{
blockThisHook = FALSE;
}
}
delete[] stringBuffer;
delete[] dataBuffer;
}
// Apply the decision
if (blockThisHook)
{
swprintf_s(text, 128, L"Keyboard event: %X (%d) is being blocked!\n", virtualKeyCode, keyPressed);
OutputDebugString(text);
return 1;
}
return 0;
}














*/