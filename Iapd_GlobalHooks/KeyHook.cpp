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

void KeyHook::addRemap(DWORD key1, DWORD key2)
{
	remapList.insert(pair<DWORD, DWORD>(key1, key2));
}

DWORD KeyHook::checkRemapForKey(DWORD key1)
{
	for (map<DWORD, DWORD>::iterator it = remapList.begin(); it != remapList.end(); it++)
	{
		if ((*it).first == key1)
			return (*it).second;
	}
	return NO_REMAP;
}

void KeyHook::deleteRemap(DWORD key)
{
	for (map<DWORD, DWORD>::iterator it = remapList.begin(); it != remapList.end(); )
	{
		if ((*it).first == key)
			remapList.erase(it++);
		else
			it++;
	}
}