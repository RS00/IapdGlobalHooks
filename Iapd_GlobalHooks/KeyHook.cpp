#include "KeyHook.h"


KeyHook::KeyHook()
{
}


KeyHook::~KeyHook()
{
}


string KeyHook::getNameFromStr(KBDLLHOOKSTRUCT str)
{
	DWORD dwMsg = 1;
	char lpszKeyName[1024] = { 0 };
	LONG lParamVal = (byte)str.scanCode << 16;
	GetKeyNameTextA(lParamVal, lpszKeyName, 1024);
	return string(lpszKeyName);
}