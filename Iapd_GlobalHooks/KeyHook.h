#pragma once
#include <Windows.h>
#include <set>
#include <map>
#include <string>
#define NO_REMAP -1
using namespace std;

#pragma comment (lib, "User32.lib")

class KeyHook
{
private:
	set<DWORD> blockList;
	map<DWORD, DWORD> remapList;
public:
	KeyHook();
	string getNameFromStr(KBDLLHOOKSTRUCT code);
	void addBlock(DWORD key);
	bool checkBlockForKey(DWORD key);
	void deleteBlock(DWORD key);
	void addRemap(DWORD key1, DWORD key2);
	DWORD checkRemapForKey(DWORD key1);
	void deleteRemap(DWORD key);
	~KeyHook();
};

