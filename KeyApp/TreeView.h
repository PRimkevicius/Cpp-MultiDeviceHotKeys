
#include "Objects.h"
#include"Resource.h"
#include <commctrl.h>
#include "KeyDataBase.h"
#include "VirtualKeys.h"
class CTreeView : public CObject
{
public:
	CTreeView();
	virtual ~CTreeView();
	BOOL Create(LPCTSTR Text, HWND hWnd, int menu, int width, int height, HINSTANCE hInst);
	HTREEITEM ItemAdd(HTREEITEM hParent, LPCTSTR Text, int image);
	BOOL ItemsDeleteAll();
	BOOL AddItemsToTV(std::list<Keys*>& LDev);
private:
	HTREEITEM tviRoot;
	HINSTANCE hInst;
private:

};


CTreeView::CTreeView()
{

}
CTreeView::~CTreeView()
{


}

BOOL CTreeView::Create(LPCTSTR Text, HWND hWnd, int menu, int width, int height, HINSTANCE hI)
{
	hInst = hI;
	DWORD dwExStyle = WS_EX_CLIENTEDGE;
	LPCTSTR lpClassName = WC_TREEVIEW;
	DWORD dwStyle = WS_VISIBLE | WS_CHILD | WS_BORDER | TVS_HASLINES | TVS_HASBUTTONS | TVS_HASBUTTONS | TVS_LINESATROOT;
	HMENU hMenu = (HMENU)menu;

	if (!CreateObject(width, height, dwExStyle, lpClassName, Text, dwStyle, hWnd, hMenu)) return FALSE;
	


	HIMAGELIST hImageList = ImageList_Create(16, 16, ILC_COLOR16, 2, 10);
	HBITMAP hBitMap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_TREE));
	ImageList_Add(hImageList, hBitMap, NULL);
	DeleteObject(hBitMap);
	SendMessage(ReturnObjecthWnd(), TVM_SETIMAGELIST, 0, (LPARAM)hImageList);

	//DWORD dwStyle = (DWORD)GetWindowLongPtr(ReturnObjecthWnd(), GWL_STYLE);
	//dwStyle |= TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT;
	//SetWindowLongPtr(ReturnObjecthWnd(), GWL_STYLE, dwStyle);
	return TRUE;
}
HTREEITEM CTreeView::ItemAdd(HTREEITEM hParent, LPCTSTR Text, int image)
{

	TVITEM tvi;
	ZeroMemory(&tvi, sizeof(TVITEM));
	tvi.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVN_SELCHANGED;
	tvi.iImage = 0;
	tvi.iSelectedImage = 1;
	tvi.pszText = (LPWSTR)Text;

	TVINSERTSTRUCT tvis;
	ZeroMemory(&tvis, sizeof(TVINSERTSTRUCT));
	tvis.hParent = hParent;
	tvis.item = tvi;

	return TreeView_InsertItem(ReturnObjecthWnd(), &tvis);
}
BOOL CTreeView::ItemsDeleteAll()
{
	return TreeView_DeleteAllItems(ReturnObjecthWnd());
}
BOOL CTreeView::AddItemsToTV(std::list<Keys*>& LDev)
{
	TreeView_DeleteAllItems(ReturnObjecthWnd());

	if (LDev.size() == 0) return TRUE;
	tviRoot = ItemAdd(NULL, L"Device", 1);
	
	VirtualKeys VK;
	list<Keys*>::iterator it;
	for (it = LDev.begin(); it != LDev.end(); it++)
	{
		(*it)->tviroot = ItemAdd(tviRoot, (*it)->AltName.c_str(), 1);
		

		//wstring buf = L"Id - ";
		//ItemAdd((*it)->tviroot, buf.append((*it)->Device).c_str(), 1);
		list<KeyNode*>::iterator kit;
		for (kit = (*it)->KeyList.begin(); kit != (*it)->KeyList.end(); kit++)
		{
			int size = (*kit)->Action.size()+60;
			
			WCHAR *buf = new WCHAR[size];
			TCHAR act[160];
			LoadString(hInst, (*kit)->ActionID, act, sizeof(act) / sizeof(TCHAR));

			swprintf_s(buf, size, L"%s - %s %s", VK.ReturnKeyValue((*kit)->VirtualKey), act, (*kit)->Action.c_str());
			(*kit)->tvikey = ItemAdd((*it)->tviroot, buf, 1);
			delete buf;
		}
		//TreeView_Expand(ReturnObjecthWnd(), (*it)->tviroot, TVE_EXPAND);
	}

	TreeView_Expand(ReturnObjecthWnd(), tviRoot, TVE_EXPAND);
	return TRUE;
}