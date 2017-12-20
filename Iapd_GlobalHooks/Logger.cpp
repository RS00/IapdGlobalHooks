#include "Logger.h"

string Logger::emailReceiver;

Logger::Logger(long long maxLength, string emailReceiver, wstring fileName)
{
	this->emailReceiver = emailReceiver;
	this->fileName = fileName;
	this->maxLength = maxLength;
	this->currentLength = this->getCurrentFileSize();
	if (currentLength > maxLength)
	{
		DeleteFile(fileName.c_str());
		currentLength = 0;
	}
	hFile = CreateFile(fileName.c_str(),
		GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		hFile = CreateFile(fileName.c_str(),
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


Logger::~Logger()
{
	CloseHandle(hFile);
}

string Logger::getCurrentTime()
{
	auto end = std::chrono::system_clock::now();
	time_t currentTime = std::chrono::system_clock::to_time_t(end);
	string time = ctime(&currentTime);
	time.resize(time.size() - 1);
	return time;
}

void Logger::sendFile(wstring fileName)
{
	Email email(this->emailReceiver);
	email.send(fileName);
	return;
}

void Logger::clearFile(wstring fileName)
{
	ofstream file(fileName, ios_base::out | ios_base::trunc);
	file.close();
	return;
}

void Logger::sendFileNewThread(LPVOID arg)
{
	wstring *fileName = (wstring *)arg;
	Email email(emailReceiver);
	email.send(*fileName);
	DeleteFile(fileName->c_str());
	delete fileName;
}

void Logger::addMessage(string mes)
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
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&this->sendFileNewThread, (LPVOID)name, NULL, &id);
		hFile = CreateFile(fileName.c_str(),
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


wstring Logger::renameCurrentFile()
{
	int num = 0;
	wstring name(fileName.c_str());
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
	MoveFile(fileName.c_str() , newName.c_str());
	return newName;
}


long long Logger::getCurrentFileSize()
{
	ifstream ifs(fileName.c_str(), ios_base::in);
	stringstream buffer;
	buffer << ifs.rdbuf();
	string content(buffer.str());
	long long size = 0;
	for (std::size_t pos = 0; pos < content.size(); pos ++)
	{
		pos = content.find('\n', pos);
		if (pos != std::string::npos)
		{
			size++;
		}
		else
		{
			break;
		}
	}
	ifs.close();
	return size;
}