#pragma once
#include <string>
using namespace std;
class ILogger
{
public:
	virtual void addMessage(string mes) = 0;
	virtual void sendFile(wstring fileName) = 0;
};

