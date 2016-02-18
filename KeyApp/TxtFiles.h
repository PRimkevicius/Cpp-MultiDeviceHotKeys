#include <stdio.h>
#include "KeyDataBase.h"
#include <iostream>
#include <fstream>



class CData
{
public:
	CData();
	virtual ~CData();
	static BOOL SaveData(std::list<Keys*>& LDev, string& filename);
	static BOOL LoadData(std::list<Keys*>& LDev, string& filename);


};
CData::CData() {}
CData::~CData() {}

BOOL CData::SaveData(std::list<Keys*>& LDev, string& filename)
{
	
	if (LDev.size() == 0) return TRUE;
	WCHAR text[128];

	wofstream file(filename);
	
	if (file.is_open())
	{
		list<Keys*>::iterator it;
		for (it = LDev.begin(); it != LDev.end(); it++)
		{
			file << L"<Device>" << endl;
			file << L"\t<Name>" << (*it)->Device << endl;
			file << L"\t<AltName>" << (*it)->AltName << endl;
			list<KeyNode*>::iterator kit;
			for (kit = (*it)->KeyList.begin(); kit != (*it)->KeyList.end(); kit++)
			{
				file << L"\t<Node>" << endl;

				swprintf_s(text, 128, L"\t\t<Key>%u",(*kit)->VirtualKey);
				file << text << endl;
				//file << L"\t\t<Key>%u" << std::to_wstring((*kit)->VirtualKey).c_str() << endl;
				file << L"\t\t<ActionId>" << std::to_wstring((*kit)->ActionID).c_str() << endl;
				file << L"\t\t<Action>" << (*kit)->Action << endl;
				file << L"\t</Node>" << endl;
			}
			file << L"</Device>" << endl;
			
		}		
		file.close();
	}
	else
	{
		MessageBox(NULL, L"Cannot open the file", L"teste", MB_ICONERROR);
	}


	return TRUE;
}
BOOL CData::LoadData(std::list<Keys*>& LDev, string &filename)
{

	wifstream file(filename);
	WCHAR text[128];
	WCHAR buffer[128];
	WCHAR *wp1, *wp2;
	WCHAR* res;


	if (file.is_open())
	{
		while (file.getline(text,128))
		{

			wp1 = wcstok_s(text,L">",&res);
			wp2 = wcstok_s(NULL, L">", &res);

			swprintf_s(buffer, 128, L"%ls", wp1);
				if (wcscmp(buffer, L"<Device") == 0)
				{
					Keys *key = new Keys;
					
					while (file.getline(text, 128))
					{
						wp1 = wcstok_s(text, L">", &res);
						wp2 = wcstok_s(NULL, L">", &res);

						swprintf_s(buffer, 128, L"%ls", wp1);
						if (wcscmp(buffer, L"\t<Name") == 0)
						{
							wcsncpy_s(key->Device, wp2, 90);
						}
						if (wcscmp(buffer, L"\t<AltName") == 0)
						{
							key->AltName = wp2;
						}
						if (wcscmp(buffer, L"\t<Node") == 0)
						{
							KeyNode *node = new KeyNode;
							while (file.getline(text, 128))
							{
								wp1 = wcstok_s(text, L">", &res);
								wp2 = wcstok_s(NULL, L">", &res);

								swprintf_s(buffer, 128, L"%ls", wp1);
								if (wcscmp(buffer, L"\t\t<Key") == 0)
								{
									USHORT key = std::stoi(wp2);
									node->VirtualKey = key;
									continue;
								}
								if (wcscmp(buffer, L"\t\t<Action") == 0)
								{
									if (wp2 != NULL)  node->Action = wp2;
									continue;
								}
								if (wcscmp(buffer, L"\t\t<ActionId") == 0)
								{
									node->ActionID = std::stoi(wp2);
									continue;
								}
								if (wcscmp(buffer, L"\t</Node") == 0)	break;
							}
							if (node->ActionID <= ACTIONS || node->ActionID > ACTIONS + 15) node->ActionID = ACTIONS + 1;
							key->KeyList.push_back(node);
							continue;
						}
						if (wcscmp(buffer, L"</Device") == 0) break;

					}
					
					LDev.push_back(key);
				
			}

			//if (wp1 != NULL && wp2 != NULL)
			//{
			//	swprintf_s(buffer, 128, L"%ls", wp1);
			//	if (wcscmp(buffer, L"\t<Name") == 0)
			//	{
			//		//swprintf_s(buffer, 128, L"Device name %ls", wp2);
			//		//MessageBox(NULL, buffer, L"teste", MB_ICONERROR);
			//	}

			//	if (wcscmp(buffer, L"\t\t<Key") == 0)
			//	{
			//		swprintf_s(buffer, 128, L"%ls", wp2);
			//		

			//		USHORT key = std::stoi(buffer);

			//		MessageBox(NULL, buffer, L"teste", MB_ICONERROR);
			//		
			//		swprintf_s(buffer, 128, L"Key %u", key);
			//		MessageBox(NULL, buffer, L"teste", MB_ICONERROR);
			//	}
			//	if (wcscmp(buffer, L"\t\t<Action") == 0)
			//	{
			//		//swprintf_s(buffer, 128, L"Action %ls", wp2);
			//		//MessageBox(NULL, buffer, L"teste", MB_ICONERROR);
			//	}

			//}

		}



		file.close();

	}
	else
	{
		MessageBox(NULL, L"Cannot open the file", L"teste", MB_ICONERROR);
	}
	return TRUE;
}