#include "stdafx.h"
#include <Shellapi.h>
using namespace std;
class OpenLocation
{
public:
	OpenLocation();
	virtual ~OpenLocation();
	static void Open(const WCHAR * loc);

};
OpenLocation::OpenLocation() {}
OpenLocation::~OpenLocation() {}
void OpenLocation::Open(const WCHAR * loc)
{
	//MessageBox(NULL, L"Control", L"test test test aa", MB_OK);
	
}