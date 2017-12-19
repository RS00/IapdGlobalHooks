#include "ConfigFile.h"

ConfigFile::ConfigFile()
{
	ifstream file(FILE_NAME, ios_base::in);
	if (!file.good())
	{
		createConfig(DEFAULT_MODE, DEFAULT_EMAIL, DEFAULT_MAX_LENGTH);
	}
	parseValues();
}

ConfigFile::~ConfigFile()
{
}

void ConfigFile::encryptFile()
{
	fstream file, temp;
	char ch;
	file.open(FILE_NAME);
	temp.open("temp.cfg", ios_base::out);
	if (!file | !temp)
		return;
	stringstream buffer;
	buffer << file.rdbuf();
	string contents(buffer.str());
	for (int i = 0; i < contents.size(); i++)
	{
		ch = contents[i];
		if (ch != '\n')
			ch += ENCRYPT_KEY;
		temp << ch;
	}
	file.close();
	temp.close();
	DeleteFile(TEXT(FILE_NAME));
	MoveFile(L"temp.cfg", TEXT(FILE_NAME));
	return;
}

string ConfigFile::decryptFile()
{
	fstream file;
	char ch;
	file.open(FILE_NAME, ios_base::in);
	if (!file)
		return string();
	stringstream buffer;
	buffer << file.rdbuf();
	string contents(buffer.str());
	for (int i = 0; i < contents.size(); i++)
	{
		ch = contents[i];
		if (ch != '\n')
			ch -= ENCRYPT_KEY;
		contents[i] = ch;
	}
	file.close();
	return contents;
}

void ConfigFile::parseValues()
{
	string contents = decryptFile();
	for (int i = 0; i < 3; i++)
	{
		int start = contents.find('=');
		int end = contents.find('\n');
		if (!start | !end)
		{
			DeleteFile(TEXT(FILE_NAME));
			createConfig(DEFAULT_MODE, DEFAULT_EMAIL, DEFAULT_MAX_LENGTH);
			return;
		}
		string value = contents.substr(start + 1, end - start - 1);
 	if (i == MODE && (value != "Spy" && value != "Default"))
		{
			DeleteFile(TEXT(FILE_NAME));
			createConfig(DEFAULT_MODE, DEFAULT_EMAIL, DEFAULT_MAX_LENGTH);
			return;
		}
		contents = contents.erase(0, end + 1);
		switch (i)
		{
			case MODE:
			{	
				this->mode = value;
				break;
			}
			case EMAIL:
			{
				this->email = value;
				break;
			}
			case MAX_VALUE:
			{
				this->maxLength = value;
				break;
			}
		}
	}
	return;
}

void ConfigFile::setNewValues(string mode, string email, string maxLength)
{
	size_t value;
	stoi(maxLength, &value, 10);
	if (mode != "Spy" && mode != "Default" || value < 0)
		return;
	this->mode = mode;
	this->email = email;
	this->maxLength = maxLength;
	DeleteFile(TEXT(FILE_NAME));
	createConfig(mode, email, maxLength);
}

void ConfigFile::createConfig(string mode, string email, string maxLength)
{
	ofstream f(FILE_NAME, ios_base::out);
	string fileContent("StartMode=");
	fileContent.append(mode);
	fileContent.append("\nemailForLogs=");
	fileContent.append(email);
	fileContent.append("\nmaxFileLength=");
	fileContent.append(maxLength);
	f << fileContent;
	f.close();
	encryptFile();
}

string ConfigFile::getMode()
{
	return this->mode;
}

string ConfigFile::getEmail()
{
	return this->email;
}

string ConfigFile::getMaxLength()
{
	return this->maxLength;
}