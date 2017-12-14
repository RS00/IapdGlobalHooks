#include "KeyHook.h"


KeyHook::KeyHook()
{
}


KeyHook::~KeyHook()
{
}


string KeyHook::getNameFromCode(long code)
{
	CHAR name[1024];
	UINT scanCode = MapVirtualKeyW(code, MAPVK_VK_TO_VSC);
	long long lParamValue = (scanCode << 16);
	GetKeyNameTextA(lParamValue, name, 1024);
	string result(name);
	return result;
}