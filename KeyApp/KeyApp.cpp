// KeyApp.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "KeyApp.h"
#include "HookDll.h"
#include "Core.h"
#include <commctrl.h>
#include <deque>



#define MAX_LOADSTRING 100
// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

BOOL IsInputDialogOpen;
BOOL IsAddEditDialogOpen;
BOOL IsRenameDialogOpen;
BOOL CatchHOOKs = FALSE;

UINT const WM_KBHOOK = WM_APP + 1;



struct KeyboardHooks
{
	USHORT KeyCode;
	list<KeyNode*>::iterator ActionNodeIt;

	KeyboardHooks(USHORT keycode, list<KeyNode*>::iterator it) : KeyCode(keycode), ActionNodeIt(it) {}
};

std::deque<KeyboardHooks> KeyHooks;
list<KeyNode*>::iterator ActionNIt;

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);



LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	TestInputs(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	AddEditItems(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	RenameDialog(HWND, UINT, WPARAM, LPARAM);



void				OnTestInputs(HWND hWnd);
void				OnAdd(HWND hWnd);
void				OnRename(HWND hWnd);
void				OnEdit(HWND hWnd);
void				OnDetele();

CCore Core;
NOTIFYICONDATA niData;
int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// register TreeView from comctl32.dll before creating windows
	INITCOMMONCONTROLSEX commonCtrls;
	commonCtrls.dwSize = sizeof(commonCtrls);
	commonCtrls.dwICC = ICC_TREEVIEW_CLASSES;   // TreeView class name
	InitCommonControlsEx(&commonCtrls);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_KEYAPP, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_KEYAPP));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_KEYAPP));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_KEYAPP);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable




   hWnd =  CreateWindowEx(
	   0,
	   szWindowClass,
	   L"Keyboard Hooks",
	  WS_EX_TOOLWINDOW  | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_EX_TRANSPARENT,
	
	   CW_USEDEFAULT, CW_USEDEFAULT, 415, 400,
	   NULL, NULL, hInstance, NULL);
   
   if (!hWnd)
   {
      return FALSE;
   }




   ZeroMemory(&niData, sizeof(NOTIFYICONDATA));

  // niData.uID = 147;
   niData.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
   niData.hIcon =
	   (HICON)LoadImage(hInstance,
	   MAKEINTRESOURCE(IDI_SMALL),
	   IMAGE_ICON,
	   GetSystemMetrics(SM_CXSMICON),
	   GetSystemMetrics(SM_CYSMICON),
	   LR_DEFAULTCOLOR);
   niData.hWnd = hWnd;
   niData.uCallbackMessage = ID_TRAYICON;

   Shell_NotifyIcon(NIM_ADD, &niData);


   ShowWindow(hWnd, nCmdShow);
  // ShowWindow(hWnd, SW_HIDE);
   UpdateWindow(hWnd);

   // Register for receiving Raw Input for keyboards
   RAWINPUTDEVICE rawInputDevice[1];
   rawInputDevice[0].usUsagePage = 1;
   rawInputDevice[0].usUsage = 6;
   rawInputDevice[0].dwFlags = RIDEV_INPUTSINK;
   rawInputDevice[0].hwndTarget = hWnd;
   RegisterRawInputDevices(rawInputDevice, 1, sizeof(rawInputDevice[0]));

   // Set up the keyboard Hook
   InstallHook(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	if (CatchHOOKs && (message == WM_INPUT || message == WM_KBHOOK))
	{
		switch (message)
		{
		case WM_INPUT:
		{
			UINT bsize;
			GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &bsize, sizeof(RAWINPUTHEADER));
			LPBYTE dataBuffer = new BYTE[bsize];
			GetRawInputData((HRAWINPUT)lParam, RID_INPUT, dataBuffer, &bsize, sizeof(RAWINPUTHEADER));

			RAWINPUT* raw = (RAWINPUT*)dataBuffer;
			USHORT KeyState = raw->data.keyboard.Flags & RI_KEY_BREAK ? 0 : 1;
			USHORT VKeyCode = raw->data.keyboard.VKey;

			GetRawInputDeviceInfo(raw->header.hDevice, RIDI_DEVICENAME, NULL, &bsize);
			WCHAR* stringBuffer = new WCHAR[bsize];

			GetRawInputDeviceInfo(raw->header.hDevice, RIDI_DEVICENAME, stringBuffer, &bsize);

			if (KeyState && Core.CheckForAction(VKeyCode, stringBuffer, ActionNIt))
			{
				KeyHooks.push_back(KeyboardHooks(VKeyCode, ActionNIt));
			}
			delete[] stringBuffer;
			delete[] dataBuffer;
			return 0;
		}
		case WM_KBHOOK:
		{
			USHORT VKeyCode = (USHORT)wParam;
			USHORT KeyState = lParam & 0x80000000 ? 0 : 1;

			BOOL Block = FALSE;
			list<KeyNode*>::iterator NodeIterator;
			int index = 1;
			if (!KeyHooks.empty())
			{
				std::deque<KeyboardHooks>::iterator iterator = KeyHooks.begin();
				while (iterator != KeyHooks.end())
				{

					if (iterator->KeyCode == VKeyCode)
					{
						if (!KeyState)
						{
							Core.OnHookBlockAction(iterator->ActionNodeIt);
							for (int i = 0; i < index; ++i) { KeyHooks.pop_front(); }
						}
						return 1;
					}
					++iterator;
					++index;
				}
			}
			//OutputDebugString(L"No record found\n");
			return 0;
		}
		default: break;
		}
	}

	if (message == WM_SYSCOMMAND && (wParam & 0xFFF0) == SC_MINIMIZE)
	{
		CatchHOOKs = TRUE;
		ShowWindow(hWnd, SW_HIDE);
		return 0;
	}
	switch (message)
	{
	case WM_NOTIFY:
	{
		switch (LOWORD(wParam))
		{
		case IDC_MAIN_TREEVIEW:
			{
				switch (((LPNMHDR)lParam)->code)
				{
				case NM_RCLICK:			Core.OnTreeViewRClick(); break;
				case TVN_SELCHANGED:	Core.OnTreeViewSelectionChange(lParam); break;
				default: break;
				}
			}

		}
	}
	
	break;
	case WM_INPUT:
	{
		UINT bsize;
		// Prepare buffer for the data
		GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &bsize, sizeof(RAWINPUTHEADER));
		LPBYTE dataBuffer = new BYTE[bsize];
		// Load data into the buffer
		GetRawInputData((HRAWINPUT)lParam, RID_INPUT, dataBuffer, &bsize, sizeof(RAWINPUTHEADER));

		RAWINPUT* raw = (RAWINPUT*)dataBuffer;
		USHORT KeyState = raw->data.keyboard.Flags & RI_KEY_BREAK ? 0 : 1;
		USHORT VKeyCode = raw->data.keyboard.VKey;
		// Prepare string buffer for the device name
		GetRawInputDeviceInfo(raw->header.hDevice, RIDI_DEVICENAME, NULL, &bsize);
		WCHAR* stringBuffer = new WCHAR[bsize];

		GetRawInputDeviceInfo(raw->header.hDevice, RIDI_DEVICENAME, stringBuffer, &bsize);

		if (IsRenameDialogOpen && Core.OnRenameDialogUpdateDevice(stringBuffer)) {}
		if (IsInputDialogOpen)  Core.OnInputDialogChange(VKeyCode, KeyState, stringBuffer);
		if (IsAddEditDialogOpen && Core.ReturnAddEditPushButtonState()) Core.OnAddEditDialogChange(VKeyCode, KeyState, stringBuffer);

		if (!IsWindowVisible(hWnd) && KeyState && Core.CheckForAction(VKeyCode, stringBuffer, ActionNIt))
		{
			KeyHooks.push_back(KeyboardHooks(VKeyCode, ActionNIt));
		}
		delete[] stringBuffer;
		delete[] dataBuffer;
		return 0;
	}
		break;
	case WM_KBHOOK:
	{
		USHORT VKeyCode = (USHORT)wParam;
		USHORT KeyState = lParam & 0x80000000 ? 0 : 1;

		BOOL Block = FALSE;
		list<KeyNode*>::iterator NodeIterator;
		int index = 1;
		if (KeyState && !KeyHooks.empty())
		{
			// Search the buffer for the matching record
			std::deque<KeyboardHooks>::iterator iterator = KeyHooks.begin();
			while (iterator != KeyHooks.end())
			{

				if (iterator->KeyCode == VKeyCode)
				{
					Core.OnHookBlockAction(iterator->ActionNodeIt);
					for (int i = 0; i < index; ++i) { KeyHooks.pop_front();  }
					return 1;
				}
				++iterator;
				++index;
			}
		}
	
		OutputDebugString(L"No record found\n");
		
		return 0;

	}
		break;
	case WM_CREATE:
	{
		if (!Core.CreateFormObjects(hWnd,hInst))
		{
			MessageBox(hWnd, L"Could not create main form objects.", L"Error", MB_OK | MB_ICONERROR);
			break;
		}


		RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_INTERNALPAINT);
	}
		break;
	case WM_CTLCOLORSTATIC:
	{
		return (LONG)Core.SetStaticObjectColor((HDC)wParam);
	}
		break;
	case ID_TRAYICON:
	{
		switch (lParam)
		{
		case WM_LBUTTONDBLCLK: ShowWindow(hWnd, SW_RESTORE); break;
		case WM_RBUTTONUP: 
		//case WM_CONTEXTMENU:
		{
			HMENU hMenu = CreatePopupMenu();
			AppendMenuW(hMenu, MF_STRING, IDCM_CLOSE, L"&Exit");
			AppendMenuW(hMenu, MF_STRING, IDCM_SHOW, L"&Show");

			POINT point;
			GetCursorPos(&point);
			SetForegroundWindow(hWnd);
			TrackPopupMenu(hMenu, TPM_RIGHTBUTTON, point.x, point.y, 0, hWnd, NULL);
			PostMessage(hWnd, WM_NULL, 0, 0);
			DestroyMenu(hMenu);
		}
			break;
		}
		

	}
		
		break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);

		if (!Core.ProcessCommand(wmId))
		{
			switch (wmId)
			{
			case IDM_ABOUT:						DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About); break;
			case IDC_MAIN_ADD:					OnAdd(hWnd); break; //button add
			case IDC_MAIN_DELETE:				OnDetele(); break;  //button delete
			case IDC_MAIN_EDIT:					OnEdit(hWnd); break; // button edit
			case IDC_MAIN_INPUTS:				OnTestInputs(hWnd); break; //button inputs

			case IDM_EXIT:						DestroyWindow(hWnd); break;
			case IDM_ADD:						OnAdd(hWnd); break; //menu itme
			case IDM_DELETE:					OnDetele(); break;  //menu item
			case IDM_INPUTS:					OnTestInputs(hWnd); break; //menu item
			case IDM_RENAME:					OnRename(hWnd); break; // menu item


			case IDCM_RENAME:					OnRename(hWnd); break; // context menu item
			case IDCM_DELETE:					OnDetele(); break;  //context menu item
			case IDCM_EDIT:						OnEdit(hWnd); break; // context menu item

			case IDCM_CLOSE:					DestroyWindow(hWnd); break;
			case IDCM_SHOW:						ShowWindow(hWnd, SW_RESTORE); break;
			
			
			default:
				return DefWindowProc(hWnd, message, wParam, lParam);
			}
		}
		break;

		
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		UninstallHook();
		Core.OnExitSaveData();
		Shell_NotifyIcon(NIM_DELETE, &niData);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

void OnTestInputs(HWND hWnd)
{
	if (!Core.CheckForDialog())
	{
		Core.OnCreateDialog(CreateDialog(hInst, MAKEINTRESOURCE(IDD_INPUT_DIALOG), hWnd, TestInputs));
		IsInputDialogOpen = TRUE;
	}
}
void OnAdd(HWND hWnd)
{
	if (!Core.CheckForDialog())
	{
		Core.OnCreateDialog(CreateDialog(hInst, MAKEINTRESOURCE(IDD_ADD_ITEMS_DIALOG), hWnd, AddEditItems));
		IsAddEditDialogOpen = TRUE;
	}
}
void OnRename(HWND hWnd)
{
	if (!Core.CheckForDialog())
	{
		Core.OnCreateDialog(CreateDialog(hInst, MAKEINTRESOURCE(IDD_RENAME_DIALOG), hWnd, RenameDialog));
		Core.OnRenameObject();
		IsRenameDialogOpen = TRUE;
	}
}
void OnEdit(HWND hWnd)
{
	Core.OnCreateDialog(CreateDialog(hInst, MAKEINTRESOURCE(IDD_ADD_ITEMS_DIALOG), hWnd, AddEditItems));
	Core.OnEditItemDialog();
	IsAddEditDialogOpen = TRUE;
}
void OnDetele()
{
	Core.OnRemoveItem();
}

// Message handler for about box.
TCHAR* SelectedPath;

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
INT_PTR CALLBACK TestInputs(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	
	switch (message)
	{
		case WM_INITDIALOG:
		return (INT_PTR)TRUE;

		case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			
			IsInputDialogOpen = FALSE;
			Core.OnDialogClose();
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
INT_PTR CALLBACK AddEditItems(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	switch (message)
	{
	case WM_INITDIALOG:
	{
		//HFONT hFont = CreateFont(12, 0, 0, 0, FALSE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"MS Sans Serif");
		//SendMessage(GetDlgItem(hDlg, IDC_GROUPBOX), WM_SETFONT, (WPARAM)hFont, MAKELPARAM(FALSE, 0));
		Core.OnAddEditDialogComboBoxInitialize(hDlg,0);


		return (INT_PTR)TRUE; 
	}
	case WM_COMMAND:
		if (LOWORD(wParam) == IDC_ADD_COMBOBOX_ACTION)
		{
			switch (HIWORD(wParam))
			{
			//case CBN_DROPDOWN: // This means that the list is about to display
			////	MessageBox(NULL, L"A request to display the list has been made",L"Display Notification", MB_OK);
			//	break;
			case CBN_CLOSEUP:
			{
				int id = SendMessage(GetDlgItem(hDlg, IDC_ADD_COMBOBOX_ACTION), CB_GETCURSEL, 0, 0);
				Core.OnAddEditDialogComboBoxChange(hDlg,id);
			}
				break;

			default: break;
				
			}
		}
		if (LOWORD(wParam) == IDC_ADD_BUTTON_SELECT)
		{
			Core.OnButtonFolderFilePath();
		}
		if (LOWORD(wParam) == IDC_ADD_BUTTON_INPUT)
		{
			Core.DisableAddEditDlgControls(Core.ReturnAddEditPushButtonState());
		}
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
		{
			IsAddEditDialogOpen = FALSE;
			Core.OnDialogClose();
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		if (LOWORD(wParam) == IDC_ADD_BUTTON_ADD)
		{
			if (!Core.OnAddKeyToDataBase()) return (INT_PTR)TRUE;
			IsAddEditDialogOpen = FALSE;
			Core.OnDialogClose();
			EndDialog(hDlg, LOWORD(wParam));
		}

		break;
	}
	return (INT_PTR)FALSE;
}
INT_PTR CALLBACK RenameDialog(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	switch (message)
	{
	case WM_INITDIALOG:
	{
		return (INT_PTR)TRUE;
	}
	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK)
		{
			Core.OnAltKeyboardNameChange(hDlg);
			Core.OnDialogClose();
			
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		if (LOWORD(wParam) == IDCANCEL)
		{
			Core.OnDialogClose();
			IsRenameDialogOpen = FALSE;
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}


BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
	int length = ::GetWindowTextLength(hwnd);
	if (length == 0) return TRUE;

	if (GetWindowLong(hwnd, GWL_HWNDPARENT) || !IsWindow(hwnd) || !IsWindowVisible(hwnd) || (GetWindowLong(hwnd, GWL_EXSTYLE) & WS_EX_TOOLWINDOW))  return TRUE;

	WCHAR* buf = new WCHAR[length + 1];
	GetWindowText(hwnd, buf, length + 1);
	//	MessageBox(NULL, buf, L"As", MB_OK);
	Core.OnShowAllActiveWindows(buf);
	return TRUE;
}
BOOL CALLBACK EnumWindowsProcForWinFocus(HWND hwnd, LPARAM lParam)
{
	int length = ::GetWindowTextLength(hwnd);
	if (length == 0) return TRUE;

	if (GetWindowLong(hwnd, GWL_HWNDPARENT) || !IsWindow(hwnd) || !IsWindowVisible(hwnd) || (GetWindowLong(hwnd, GWL_EXSTYLE) & WS_EX_TOOLWINDOW))  return TRUE;
	
	WCHAR* buf = new WCHAR[length + 1];
	GetWindowText(hwnd, buf, length + 1);

	if (wcscmp(buf, (WCHAR*)lParam)==0)
	{
		if (IsIconic(hwnd)) ShowWindow(hwnd, SW_RESTORE);
		//MessageBox(NULL, buf, L"Focus", MB_OK);

		INPUT input;
		input.type = INPUT_KEYBOARD;
		input.ki.wScan = MapVirtualKey(VK_MENU, MAPVK_VK_TO_VSC);
		input.ki.time = 0;
		input.ki.dwExtraInfo = 0;
		input.ki.wVk = VK_MENU;
		input.ki.dwFlags = 0; 

		SendInput(1, &input, sizeof(INPUT));

		SetForegroundWindow(hwnd);

		input.ki.dwFlags = KEYEVENTF_KEYUP;
		SendInput(1, &input, sizeof(INPUT));

	}

	//MessageBox(NULL, buf, (WCHAR*)lParam, MB_OK);

	return TRUE;
}