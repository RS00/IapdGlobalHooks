#pragma once
#include <string>
#include <Windows.h>
#include <chrono>
#include <fstream>
#include "Email.h"
using namespace std;
class Logger
{
private:
	HANDLE hFile;
	wstring fileName;
	long long maxLength;
	long long currentLength;
	static string emailReceiver;
	string getCurrentTime();
	static void sendFileNewThread(LPVOID arg);
	static void clearFile(wstring fileName);
	wstring renameCurrentFile();
public:
	Logger(long long maxLength, string emailReceiver, wstring fileName);
	~Logger();
	void sendFile(wstring fileName);
	void addMessage(string mes);
};

