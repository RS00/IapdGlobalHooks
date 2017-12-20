#pragma once
#include <Windows.h>
#include <set>
#include <string>
using namespace std;

#pragma comment (lib, "User32.lib")

class KeyHook
{
private:
	set<DWORD> blockList;
public:
	KeyHook();
	string getNameFromStr(KBDLLHOOKSTRUCT code);
	void addBlock(DWORD key);
	bool checkBlockForKey(DWORD key);
	void deleteBlock(DWORD key);
	~KeyHook();
};

