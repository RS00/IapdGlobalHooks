#pragma once
#include <string>
using namespace std;
class ILogger
{
public:
	virtual void addMessage(string mes) = 0;
	virtual void clearFile() = 0;
	virtual void sendFile() = 0;
};

