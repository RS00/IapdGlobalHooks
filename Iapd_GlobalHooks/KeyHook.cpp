#include "KeyHook.h"


KeyHook::KeyHook()
{
}


KeyHook::~KeyHook()
{
}


string KeyHook::getNameFromStr(KBDLLHOOKSTRUCT str)
{
	char lpszKeyName[1024] = { 0 };
	LONG lParamVal = (byte)str.scanCode << 16;
	GetKeyNameTextA(lParamVal, lpszKeyName, 1024);
	return string(lpszKeyName);
}

void KeyHook::addBlock(DWORD key)
{
	blockList.insert(key);
}

bool KeyHook::checkBlockForKey(DWORD key)
{
	return blockList.find(key) != blockList.end();
}

void KeyHook::deleteBlock(DWORD key)
{
	blockList.erase(key);
}