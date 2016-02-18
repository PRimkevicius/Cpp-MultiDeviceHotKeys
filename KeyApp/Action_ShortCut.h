#include "stdafx.h"
#include <string>
#include <vector>
using namespace std;

class SKeySimulation
{
public:
	SKeySimulation();
	virtual ~SKeySimulation();
	std::vector<INPUT*> KeyHold;
	std::vector<INPUT*> KeyPress;

};
SKeySimulation::SKeySimulation() {}
SKeySimulation::~SKeySimulation() 
{
	for (unsigned int i = 0; i < KeyHold.size(); i++)
	{
		delete KeyHold[i];
	}
	for (unsigned int i = 0; i < KeyPress.size(); i++)
	{
		delete KeyPress[i];
	}
	KeyHold.clear();
	KeyPress.clear();
}


class AShortCut
{
public:
	AShortCut();
	virtual ~AShortCut();
	static BOOL CheckIfShortCutIsValid(wstring sc);
	static BOOL ExecuteShortCut(wstring sc);
	static INPUT *SetInput(USHORT KEY);

};
AShortCut::AShortCut() {}
AShortCut::~AShortCut() {}
INPUT *AShortCut::SetInput(USHORT KEY)
{
	INPUT *input = new INPUT();

	input->type = INPUT_KEYBOARD;
	input->ki.wScan = MapVirtualKey(KEY, MAPVK_VK_TO_VSC);
	input->ki.time = 0;
	input->ki.dwExtraInfo = 0;
	input->ki.wVk = KEY;
	input->ki.dwFlags = 0; // there is no KEYEVENTF_KEYDOWN
	return input;

}
BOOL AShortCut::ExecuteShortCut(wstring sc)
{
	if (sc.size() < 1) return FALSE;
	SKeySimulation *as = new SKeySimulation();


	unsigned int i = 0;
	while (sc.length() > i)
	{
		if (sc[i] == L'^' || sc[i] == L'!' || sc[i] == L'+' || sc[i] == L'#')
		{
			//	MessageBox(NULL, L"Control", L"test test test aa", MB_OK);
			if (sc[i] == L'^')  as->KeyHold.push_back(SetInput(VK_CONTROL));
			if (sc[i] == L'!')  as->KeyHold.push_back(SetInput(VK_MENU));
			if (sc[i] == L'+')  as->KeyHold.push_back(SetInput(VK_SHIFT));
			if (sc[i] == L'#')  as->KeyHold.push_back(SetInput(VK_LWIN));
			i++;
			continue;
		}

		if (sc[i] == L'(')
		{
			int istart = i + 1;

			while (sc.length() > i)
			{
				if (sc[i] == L')')
				{
					wstring test = sc.substr(istart, i - istart);

					USHORT keyCode = VirtualKeys::ReturnKeyCode(test);

					as->KeyPress.push_back(SetInput(keyCode));
					break;
				}
				else i++;
			}

		}

		i++;
	}

	for (auto i = as->KeyHold.begin(); i != as->KeyHold.end(); i++)
	{
		SendInput(1, (*i), sizeof(INPUT));
	}
	for (auto i = as->KeyPress.begin(); i != as->KeyPress.end(); i++)
	{
		SendInput(1, (*i), sizeof(INPUT));
	}

	for (auto i = as->KeyPress.begin(); i != as->KeyPress.end(); i++)
	{
		(*i)->ki.dwFlags = KEYEVENTF_KEYUP;
		SendInput(1, (*i), sizeof(INPUT));
	}
	for (auto i = as->KeyHold.begin(); i != as->KeyHold.end(); i++)
	{
		(*i)->ki.dwFlags = KEYEVENTF_KEYUP;
		SendInput(1, (*i), sizeof(INPUT));
	}

	delete as;
	return TRUE;
}
BOOL AShortCut::CheckIfShortCutIsValid(wstring sc)
{
	MessageBox(NULL, sc.c_str(), L"aa", MB_OK);
	
	if (sc.size() < 1) return FALSE;
	SKeySimulation *as = new SKeySimulation();
	

	unsigned int i = 0;
	
	while (sc.length() > i)
	{
		

		if (sc[i] == L'^' || sc[i] == L'!' || sc[i] == L'+' || sc[i] == L'#')
		{
		//	MessageBox(NULL, L"Control", L"test test test aa", MB_OK);
			INPUT *input = new INPUT();

			input->type = INPUT_KEYBOARD;
			input->ki.wScan = MapVirtualKey(VK_LWIN, MAPVK_VK_TO_VSC);
			input->ki.time = 0;
			input->ki.dwExtraInfo = 0;
			input->ki.wVk = VK_LWIN;
			input->ki.dwFlags = 0; // there is no KEYEVENTF_KEYDOWN

			as->KeyHold.push_back(input);

			i++;
			continue;
		}

		if (sc[i] == L'(')
		{
			int istart = i+1;
			
			while (sc.length() > i)
			{
				if (sc[i] == L')')
				{
					wstring test = sc.substr(istart, i - istart);

					USHORT keyCode = VirtualKeys::ReturnKeyCode(test);

					INPUT *input = new INPUT();

					input->type = INPUT_KEYBOARD;
					input->ki.wScan = MapVirtualKey(keyCode, MAPVK_VK_TO_VSC);
					input->ki.time = 0;
					input->ki.dwExtraInfo = 0;
					input->ki.wVk = keyCode;
					input->ki.dwFlags = 0; // there is no KEYEVENTF_KEYDOWN

					as->KeyPress.push_back(input);
					break;
				}
				else i++;
			}
			
		}

		i++;
	}


	
		for (auto i = as->KeyHold.begin(); i != as->KeyHold.end(); i++)
		{
			SendInput(1, (*i), sizeof(INPUT));
		}
	for (auto i = as->KeyPress.begin(); i != as->KeyPress.end(); i++)
	{
		SendInput(1, (*i), sizeof(INPUT));
	}

	for (auto i = as->KeyPress.begin(); i != as->KeyPress.end(); i++)
	{
		(*i)->ki.dwFlags = KEYEVENTF_KEYUP;
		SendInput(1, (*i), sizeof(INPUT));
	}
	for (auto i = as->KeyHold.begin(); i != as->KeyHold.end(); i++)
	{
		(*i)->ki.dwFlags = KEYEVENTF_KEYUP;
		SendInput(1, (*i), sizeof(INPUT));
	}
	
	delete as;

	return TRUE;
}
