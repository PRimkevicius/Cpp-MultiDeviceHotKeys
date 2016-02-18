#pragma once
#include "stdafx.h"
#include <string>
#include <algorithm>
using namespace std;


struct KeyNode
{
	USHORT VirtualKey;
	int ActionID;
	wstring Action;
	HTREEITEM tvikey;
	HTREEITEM tviaction;
};

class Keys
{
public:
	Keys();
	virtual ~Keys();
	std::list<KeyNode*> KeyList;
    WCHAR Device[90];
	HTREEITEM tviroot;
	wstring AltName;


};
Keys::Keys() {}
Keys::~Keys() 
{
	if (KeyList.size() == 0) return;

	list<KeyNode*>::iterator it;
	for (it = KeyList.begin(); it != KeyList.end(); it++)
	{
		delete (*it);
	}
	KeyList.clear();
}

class CDB_Actions
{
public:
	CDB_Actions();
	virtual ~CDB_Actions();
	static int ReturIdAndAction(HWND hdlg, wstring &altname);
	static KeyNode* CreateNewKeyNodeItem(HWND hdlg, int LastVirtualKey);
	static BOOL UpdateKeyNodeItem(HWND hdlg, int LastVirtualKey, list<KeyNode*>::iterator knit);

	static Keys* CreateNewDeviceItem(WCHAR *LastDeviceName, int size);
	static BOOL CheckActionTextBox(HWND hdlg);
};

CDB_Actions::CDB_Actions() {}
CDB_Actions::~CDB_Actions() {}
int CDB_Actions::ReturIdAndAction(HWND hdlg, wstring &action)
{
	int actionId = ACTIONS + 1 + SendMessage(GetDlgItem(hdlg, IDC_ADD_COMBOBOX_ACTION), CB_GETCURSEL, 0, 0);

	switch (actionId)
	{
	case ACTION_PASTE:
	{
		int length = GetWindowTextLength(GetDlgItem(hdlg, IDC_ADD_EDIT_ACTION));
		if (length != 0)
		{
			WCHAR* buf = new WCHAR[256];
			GetDlgItemText(hdlg, IDC_ADD_EDIT_ACTION, buf, 256);
			action.assign(buf);
			delete buf;
		}
	}
		break;
	case ACTION_OPENFILE:
	{
		int length = GetWindowTextLength(GetDlgItem(hdlg, IDC_ADD_EDIT_PATH));
		if (length != 0)
		{
			int size = GetWindowTextLength(GetDlgItem(hdlg, IDC_ADD_EDIT_PATH)) + 1;
			WCHAR* buf = new WCHAR[size];
			GetDlgItemText(hdlg, IDC_ADD_EDIT_PATH, buf, size);
			action.assign(buf);
			delete buf;
		}
	}
		break;
	case ACTION_OPENFOLDER:
	{
		int length = GetWindowTextLength(GetDlgItem(hdlg, IDC_ADD_EDIT_PATH));
		if (length != 0)
		{
			int size = GetWindowTextLength(GetDlgItem(hdlg, IDC_ADD_EDIT_PATH)) + 1;
			WCHAR* buf = new WCHAR[size];
			GetDlgItemText(hdlg, IDC_ADD_EDIT_PATH, buf, size);
			action.assign(buf);
			delete buf;
		}
	}
		break;

	case ACTION_SHORTCUT:
	{
		int length = GetWindowTextLength(GetDlgItem(hdlg, IDC_ADD_EDIT_SHORTCUT));
		if (length != 0)
		{
			int size = GetWindowTextLength(GetDlgItem(hdlg, IDC_ADD_EDIT_SHORTCUT)) + 1;
			WCHAR* buf = new WCHAR[size];
			GetDlgItemText(hdlg, IDC_ADD_EDIT_SHORTCUT, buf, size);
			action.append(buf);
			delete buf;
		}
	}
		break;
	case ACTION_WINDOWFOCUS:
	{
		int length = GetWindowTextLength(GetDlgItem(hdlg, IDC_ADD_CONBOBOX_WIN));
		if (length != 0)
		{
			int size = GetWindowTextLength(GetDlgItem(hdlg, IDC_ADD_CONBOBOX_WIN)) + 1;
			WCHAR* buf = new WCHAR[size];
			GetDlgItemText(hdlg, IDC_ADD_CONBOBOX_WIN, buf, size);
			action.append(buf);
			delete buf;
		}
	}
		break;
	default:
		break;
	}
	return actionId;
} 


BOOL CDB_Actions::CheckActionTextBox(HWND hdlg)
{
	switch (ACTIONS + 1 + SendMessage(GetDlgItem(hdlg, IDC_ADD_COMBOBOX_ACTION), CB_GETCURSEL, 0, 0))
	{
	case ACTION_PASTE:
	{
		int length = GetWindowTextLength(GetDlgItem(hdlg, IDC_ADD_EDIT_ACTION));
		if (length == 0) { MessageBox(NULL, L"TextBox is empy", L"Chose a file", MB_OK); return FALSE; }
	}
		break;
	case ACTION_OPENFILE:
	{
		int length = GetWindowTextLength(GetDlgItem(hdlg, IDC_ADD_EDIT_PATH));
		if (length == 0) { MessageBox(NULL, L"Please, select a file", L"Chose a file", MB_OK); return FALSE;  }
	}
		break;
	case ACTION_OPENFOLDER:
	{
		int length = GetWindowTextLength(GetDlgItem(hdlg, IDC_ADD_EDIT_PATH));
		if (length == 0) { MessageBox(NULL, L"Please, select a folder", L"Chose a folder", MB_OK); return FALSE;  }
	}
		break;
	case ACTION_SHORTCUT:
	{
		int length = GetWindowTextLength(GetDlgItem(hdlg, IDC_ADD_EDIT_SHORTCUT));
		if (length == 0) { MessageBox(NULL, L"Please, set a shortcut", L"Shortcut", MB_OK); return FALSE;  }
	}
		break;
	default:
		break;
	}

	return TRUE;
}
KeyNode* CDB_Actions::CreateNewKeyNodeItem(HWND hdlg, int LastVirtualKey)
{
	KeyNode *node = new KeyNode();
	wstring action;
	node->ActionID = ReturIdAndAction(hdlg, action);
	node->VirtualKey = LastVirtualKey;
	node->Action = action;
	return node;
}
BOOL CDB_Actions::UpdateKeyNodeItem(HWND hdlg, int LastVirtualKey, list<KeyNode*>::iterator knit)
{
	wstring action;
	(*knit)->ActionID = ReturIdAndAction(hdlg, action);
	(*knit)->VirtualKey = LastVirtualKey;
	(*knit)->Action = action;
	return TRUE;
}
Keys* CreateNewDeviceItem(WCHAR *LastDeviceName,int size)
{
	Keys *key = new Keys;
	wcsncpy_s(key->Device, LastDeviceName, 90);
	key->AltName.assign(L"Keyboard").append(std::to_wstring(size + 1).c_str());

	return key;

}