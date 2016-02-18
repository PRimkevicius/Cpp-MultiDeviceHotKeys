#pragma once
#include "stdafx.h"

class VirtualKeys
{
public:
	VirtualKeys();
	virtual ~VirtualKeys();
	static WCHAR* ReturnKeyValue(USHORT VKEY);
	static USHORT ReturnKeyCode(wstring value);

};
VirtualKeys::VirtualKeys()
{


}
VirtualKeys::~VirtualKeys()
{


}
WCHAR* VirtualKeys::ReturnKeyValue(USHORT VKEY)
{
	switch (VKEY)
	{
	case 0x41: return L"A";
	case 0x6b: return L"Add";
	case 0x40000: return L"Alt";
	case 0x5d: return L"Apps";
	case 0xf6: return L"Attn";
	case 0x42: return L"B";
	case 8: return L"Back";
	case 0xa6: return L"BrowserBack";
	case 0xab: return L"BrowserFavorites";
	case 0xa7: return L"BrowserForward";
	case 0xac: return L"BrowserHome";
	case 0xa8: return L"BrowserRefresh";
	case 170: return L"BrowserSearch";
	case 0xa9: return L"BrowserStop";
	case 0x43: return L"C";
	case 3: return L"Cancel";
	//case 20: return L"Capital";
	case 20: return L"CapsLock";
	case 12: return L"Clear";
	case 0x20000: return L"Control";
	case 0x11: return L"ControlKey";
	case 0xf7: return L"Crsel";
	case 0x44: return L"D";
	case 0x30: return L"D0";
	case 0x31: return L"D1";
	case 50: return L"D2";
	case 0x33: return L"D3";
	case 0x34: return L"D4";
	case 0x35: return L"D5";
	case 0x36: return L"D6";
	case 0x37: return L"D7";
	case 0x38: return L"D8";
	case 0x39: return L"D9";
	case 110: return L"Decimal";
	case 0x2e: return L"Delete";
	case 0x6f: return L"Divide";
	case 40: return L"Down";
	case 0x45: return L"E";
	case 0x23: return L"End";
	case 13: return L"Enter";
	case 0xf9: return L"EraseEof";
	case 0x1b: return L"Escape";
	case 0x2b: return L"Execute";
	case 0xf8: return L"Exsel";
	case 70: return L"F";
	case 0x70: return L"F1";
	case 0x79: return L"F10";
	case 0x7a: return L"F11";
	case 0x7b: return L"F12";
	case 0x7c: return L"F13";
	case 0x7d: return L"F14";
	case 0x7e: return L"F15";
	case 0x7f: return L"F16";
	case 0x80: return L"F17";
	case 0x81: return L"F18";
	case 130: return L"F19";
	case 0x71: return L"F2";
	case 0x83: return L"F20";
	case 0x84: return L"F21";
	case 0x85: return L"F22";
	case 0x86: return L"F23";
	case 0x87: return L"F24";
	case 0x72: return L"F3";
	case 0x73: return L"F4";
	case 0x74: return L"F5";
	case 0x75: return L"F6";
	case 0x76: return L"F7";
	case 0x77: return L"F8";
	case 120: return L"F9";
	case 0x18: return L"FinalMode";
	case 0x47: return L"G";
	case 0x48: return L"H";
	case 0x15: return L"HanguelMode";
	//case 0x15: return L"HangulMode";
	case 0x19: return L"HanjaMode";
	case 0x2f: return L"Help";
	case 0x24: return L"Home";
	case 0x49: return L"I";
	case 30: return L"IMEAceept";
	case 0x1c: return L"IMEConvert";
	case 0x1f: return L"IMEModeChange";
	case 0x1d: return L"IMENonconvert";
	case 0x2d: return L"Insert";
	case 0x4a: return L"J";
	case 0x17: return L"JunjaMode";
	case 0x4b: return L"K";
		//case 0x15:    return L"KanaMode";
		//case 0x19:    return L"KanjiMode";
	case 0xffff: return L"KeyCode";
	case 0x4c: return L"L";
	case 0xb6: return L"LaunchApplication1";
	case 0xb7: return L"LaunchApplication2";
	case 180: return L"LaunchMail";
	case 1: return L"LButton";
	case 0xa2: return L"LControl";
	case 0x25: return L"Left";
	case 10: return L"LineFeed";
	case 0xa4: return L"LMenu";
	case 160: return L"LShift";
	case 0x5b: return L"LWin";
	case 0x4d: return L"M";
	case 4: return L"MButton";
	case 0xb0: return L"MediaNextTrack";
	case 0xb3: return L"MediaPlayPause";
	case 0xb1: return L"MediaPreviousTrack";
	case 0xb2: return L"MediaStop";
	case 0x12: return L"Menu";
		// case 65536:  return L"Modifiers";
	case 0x6a: return L"Multiply";
	case 0x4e: return L"N";
	case 0x22: return L"Next";
	case 0xfc: return L"NoName";
	case 0: return L"None";
	case 0x90: return L"NumLock";
	case 0x60: return L"NumPad0";
	case 0x61: return L"NumPad1";
	case 0x62: return L"NumPad2";
	case 0x63: return L"NumPad3";
	case 100: return L"NumPad4";
	case 0x65: return L"NumPad5";
	case 0x66: return L"NumPad6";
	case 0x67: return L"NumPad7";
	case 0x68: return L"NumPad8";
	case 0x69: return L"NumPad9";
	case 0x4f: return L"O";
	case 0xdf: return L"Oem8";
	case 0xe2: return L"OemBackslash";
	case 0xfe: return L"OemClear";
	case 0xdd: return L"OemCloseBrackets";
	case 0xbc: return L"OemComma";
	case 0xbd: return L"OemMinus";
	case 0xdb: return L"OemOpenBrackets";
	case 190: return L"OemPeriod";
	case 220: return L"OemPipe";
	case 0xbb: return L"Oemplus";
	case 0xbf: return L"OemQuestion";
	case 0xde: return L"OemQuotes";
	case 0xba: return L"OemSemicolon";
	case 0xc0: return L"Oemtilde";
	case 80: return L"P";
	case 0xfd: return L"Pa1";
		// case 0x22:   return L"PageDown";
		// case 0x21:   return L"PageUp";
	case 0x13: return L"Pause";
	case 250: return L"Play";
	case 0x2a: return L"Print";
	case 0x2c: return L"PrintScreen";
	case 0x21: return L"Prior";
	case 0xe5: return L"ProcessKey";
	case 0x51: return L"Q";
	case 0x52: return L"R";
	case 2: return L"RButton";
	case 0xa3: return L"RControl";
		//case 13:      return L"Return";
	case 0x27: return L"Right";
	case 0xa5: return L"RMenu";
	case 0xa1: return L"RShift";
	case 0x5c: return L"RWin";
	case 0x53: return L"S";
	case 0x91: return L"Scroll";
	case 0x29: return L"Select";
	case 0xb5: return L"SelectMedia";
	case 0x6c: return L"Separator";
	case 0x10000: return L"Shift";
	case 0x10: return L"ShiftKey";
		//case 0x2c:    return L"Snapshot";
	case 0x20: return L"Space";
	case 0x6d: return L"Subtract";
	case 0x54: return L"T";
	case 9: return L"Tab";
	case 0x55: return L"U";
	case 0x26: return L"Up";
	case 0x56: return L"V";
	case 0xae: return L"VolumeDown";
	case 0xad: return L"VolumeMute";
	case 0xaf: return L"VolumeUp";
	case 0x57: return L"W";
	case 0x58: return L"X";
	case 5: return L"XButton1";
	case 6: return L"XButton2";
	case 0x59: return L"Y";
	case 90: return L"Z";
	case 0xfb: return L"Zoom";
	default: return L"Unknown";
	}
}
USHORT VirtualKeys::ReturnKeyCode(wstring value)
{
	transform(value.begin(), value.end(), value.begin(), towupper);
	if (value == L"A") return 0x41;
	if (value == L"ADD") return 0x6b;
	if (value == L"APPS") return 0x5d;
	if (value == L"ATTN") return 0xf6;
	if (value == L"B") return 0x42;
	if (value == L"BACK") return 8;
	if (value == L"BROWSERBACK") return 0xa6;
	if (value == L"BROWSERFAVORITES") return 0xab;
	if (value == L"BROWSERFORWARD") return 0xa7;
	if (value == L"BROWSERHOME") return 0xac;
	if (value == L"BROWSERREFRESH") return 0xa8;
	if (value == L"BROWSERSEARCH") return 170;
	if (value == L"BROWSERSTOP") return 0xa9;
	if (value == L"C") return 0x43;
	if (value == L"CANCEL") return 3;
	if (value == L"CAPSLOCK") return 20;
	if (value == L"CLEAR") return 12;
	if (value == L"CRSEL") return 0xf7;
	if (value == L"D") return 0x44;
	if (value == L"D0") return 0x30;
	if (value == L"D1") return 0x31;
	if (value == L"D2") return 50;
	if (value == L"D3") return 0x33;
	if (value == L"D4") return 0x34;
	if (value == L"D5") return 0x35;
	if (value == L"D6") return 0x36;
	if (value == L"D7") return 0x37;
	if (value == L"D8") return 0x38;
	if (value == L"D9") return 0x39;
	if (value == L"DECIMAL") return 110;
	if (value == L"DELETE") return 0x2e;
	if (value == L"DIVIDE") return 0x6f;
	if (value == L"DOWN") return 40;
	if (value == L"E") return 0x45;
	if (value == L"END") return 0x23;
	if (value == L"ENTER") return 13;
	if (value == L"ERASEEOF") return 0xf9;
	if (value == L"ESCAPE") return 0x1b;
	if (value == L"EXECUTE") return 0x2b;
	if (value == L"EXSEL") return 0xf8;
	if (value == L"F") return 70;
	if (value == L"F1") return 0x70;
	if (value == L"F10") return 0x79;
	if (value == L"F11") return 0x7a;
	if (value == L"F12") return 0x7b;
	if (value == L"F13") return 0x7c;
	if (value == L"F14") return 0x7d;
	if (value == L"F15") return 0x7e;
	if (value == L"F16") return 0x7f;
	if (value == L"F17") return 0x80;
	if (value == L"F18") return 0x81;
	if (value == L"F19") return 130;
	if (value == L"F2") return 0x71;
	if (value == L"F20") return 0x83;
	if (value == L"F21") return 0x84;
	if (value == L"F22") return 0x85;
	if (value == L"F23") return 0x86;
	if (value == L"F24") return 0x87;
	if (value == L"F3") return 0x72;
	if (value == L"F4") return 0x73;
	if (value == L"F5") return 0x74;
	if (value == L"F6") return 0x75;
	if (value == L"F7") return 0x76;
	if (value == L"F8") return 0x77;
	if (value == L"F9") return 120;
	if (value == L"FINALMODE") return 0x18;
	if (value == L"G") return 0x47;
	if (value == L"H") return 0x48;
	if (value == L"HANGUELMODE") return 0x15;
	if (value == L"HANJAMODE") return 0x19;
	if (value == L"HELP") return 0x2f;
	if (value == L"HOME") return 0x24;
	if (value == L"I") return 0x49;
	if (value == L"IMEACEEPT") return 30;
	if (value == L"IMECONVERT") return 0x1c;
	if (value == L"IMEMODECHANGE") return 0x1f;
	if (value == L"IMENONCONVERT") return 0x1d;
	if (value == L"INSERT") return 0x2d;
	if (value == L"J") return 0x4a;
	if (value == L"JUNJAMODE") return 0x17;
	if (value == L"K") return 0x4b;
	if (value == L"KEYCODE") return 0xffff;
	if (value == L"L") return 0x4c;
	if (value == L"LAUNCHAPPLICATION1") return 0xb6;
	if (value == L"LAUNCHAPPLICATION2") return 0xb7;
	if (value == L"LAUNCHMAIL") return 180;
	if (value == L"LBUTTON") return 1;
	if (value == L"LCONTROL") return 0xa2;
	if (value == L"LEFT") return 0x25;
	if (value == L"LINEFEED") return 10;
	if (value == L"LMENU") return 0xa4;
	if (value == L"LSHIFT") return 160;
	if (value == L"LWIN") return 0x5b;
	if (value == L"M") return 0x4d;
	if (value == L"MBUTTON") return 4;
	if (value == L"MEDIANEXTTRACK") return 0xb0;
	if (value == L"MEDIAPLAYPAUSE") return 0xb3;
	if (value == L"MEDIAPREVIOUSTRACK") return 0xb1;
	if (value == L"MEDIASTOP") return 0xb2;
	if (value == L"MENU") return 0x12;
	if (value == L"MULTIPLY") return 0x6a;
	if (value == L"N") return 0x4e;
	if (value == L"NEXT") return 0x22;
	if (value == L"NONAME") return 0xfc;
	if (value == L"NONE") return 0;
	if (value == L"NUMLOCK") return 0x90;
	if (value == L"NUMPAD0") return 0x60;
	if (value == L"NUMPAD1") return 0x61;
	if (value == L"NUMPAD2") return 0x62;
	if (value == L"NUMPAD3") return 0x63;
	if (value == L"NUMPAD4") return 100;
	if (value == L"NUMPAD5") return 0x65;
	if (value == L"NUMPAD6") return 0x66;
	if (value == L"NUMPAD7") return 0x67;
	if (value == L"NUMPAD8") return 0x68;
	if (value == L"NUMPAD9") return 0x69;
	if (value == L"O") return 0x4f;
	if (value == L"OEM8") return 0xdf;
	if (value == L"OEMBACKSLASH") return 0xe2;
	if (value == L"OEMCLEAR") return 0xfe;
	if (value == L"OEMCLOSEBRACKETS") return 0xdd;
	if (value == L"OEMCOMMA") return 0xbc;
	if (value == L"OEMMINUS") return 0xbd;
	if (value == L"OEMOPENBRACKETS") return 0xdb;
	if (value == L"OEMPERIOD") return 190;
	if (value == L"OEMPIPE") return 220;
	if (value == L"OEMPLUS") return 0xbb;
	if (value == L"OEMQUESTION") return 0xbf;
	if (value == L"OEMQUOTES") return 0xde;
	if (value == L"OEMSEMICOLON") return 0xba;
	if (value == L"OEMTILDE") return 0xc0;
	if (value == L"P") return 80;
	if (value == L"PA1") return 0xfd;
	if (value == L"PAUSE") return 0x13;
	if (value == L"PLAY") return 250;
	if (value == L"PRINT") return 0x2a;
	if (value == L"PRINTSCREEN") return 0x2c;
	if (value == L"PRIOR") return 0x21;
	if (value == L"PROCESSKEY") return 0xe5;
	if (value == L"Q") return 0x51;
	if (value == L"R") return 0x52;
	if (value == L"RBUTTON") return 2;
	if (value == L"RCONTROL") return 0xa3;
	if (value == L"RIGHT") return 0x27;
	if (value == L"RMENU") return 0xa5;
	if (value == L"RSHIFT") return 0xa1;
	if (value == L"RWIN") return 0x5c;
	if (value == L"S") return 0x53;
	if (value == L"SCROLL") return 0x91;
	if (value == L"SELECT") return 0x29;
	if (value == L"SELECTMEDIA") return 0xb5;
	if (value == L"SEPARATOR") return 0x6c;
	if (value == L"SPACE") return 0x20;
	if (value == L"SUBTRACT") return 0x6d;
	if (value == L"T") return 0x54;
	if (value == L"TAB") return 9;
	if (value == L"U") return 0x55;
	if (value == L"UP") return 0x26;
	if (value == L"V") return 0x56;
	if (value == L"VOLUMEDOWN") return 0xae;
	if (value == L"VOLUMEMUTE") return 0xad;
	if (value == L"VOLUMEUP") return 0xaf;
	if (value == L"W") return 0x57;
	if (value == L"X") return 0x58;
	if (value == L"XBUTTON1") return 5;
	if (value == L"XBUTTON2") return 6;
	if (value == L"Y") return 0x59;
	if (value == L"Z") return 90;
	if (value == L"ZOOM") return 0xfb;


	return -1;
}



//INPUT input;

//input.type = INPUT_KEYBOARD;
//input.ki.wScan = MapVirtualKey(0x52, MAPVK_VK_TO_VSC);
//input.ki.time = 0;
//input.ki.dwExtraInfo = 0;
//input.ki.wVk = 0x52;
//input.ki.dwFlags = 0; // there is no KEYEVENTF_KEYDOWN

//INPUT input2;

//input2.type = INPUT_KEYBOARD;
//input2.ki.wScan = MapVirtualKey(VK_LWIN, MAPVK_VK_TO_VSC);
//input2.ki.time = 0;
//input2.ki.dwExtraInfo = 0;
//input2.ki.wVk = VK_LWIN;
//input2.ki.dwFlags = 0; // there is no KEYEVENTF_KEYDOWN


//SendInput(1, &input2, sizeof(INPUT));
//SendInput(1, &input, sizeof(INPUT));


//input.ki.dwFlags = KEYEVENTF_KEYUP;
//input2.ki.dwFlags = KEYEVENTF_KEYUP;
//SendInput(1, &input, sizeof(INPUT));
//SendInput(1, &input2, sizeof(INPUT));

//return TRUE;
//SKeySimulation *as = new SKeySimulation();
//
//INPUT input;

//input.type = INPUT_KEYBOARD;
//input.ki.wScan = MapVirtualKey(0x52, MAPVK_VK_TO_VSC);
//input.ki.time = 0;
//input.ki.dwExtraInfo = 0;
//input.ki.wVk = 0x52;
//input.ki.dwFlags = 0; // there is no KEYEVENTF_KEYDOWN
//
//as->KeyPress.push_back(input);

//INPUT input2;

//input2.type = INPUT_KEYBOARD;
//input2.ki.wScan = MapVirtualKey(VK_LWIN, MAPVK_VK_TO_VSC);
//input2.ki.time = 0;
//input2.ki.dwExtraInfo = 0;
//input2.ki.wVk = VK_LWIN;
//input2.ki.dwFlags = 0; // there is no KEYEVENTF_KEYDOWN

//as->KeyHold.push_back(input2);



//for (auto i = as->KeyHold.begin(); i != as->KeyHold.end(); i++)
//{
//	SendInput(1, &(*i), sizeof(INPUT));
//}
//for (auto i = as->KeyPress.begin(); i != as->KeyPress.end(); i++)
//{
//	SendInput(1, &(*i), sizeof(INPUT));
//}

//for (auto i = as->KeyPress.begin(); i != as->KeyPress.end(); i++)
//{
//	i->ki.dwFlags = KEYEVENTF_KEYUP;
//	SendInput(1, &(*i), sizeof(INPUT));
//}
//for (auto i = as->KeyHold.begin(); i != as->KeyHold.end(); i++)
//{
//	i->ki.dwFlags = KEYEVENTF_KEYUP;
//	SendInput(1, &(*i), sizeof(INPUT));
//}

