#pragma once
#include <Windows.h>
#include <string>
using namespace std;

#pragma comment (lib, "User32.lib")

class KeyHook
{
private:
public:
	KeyHook();
	string getNameFromCode(long code);
	~KeyHook();
};

