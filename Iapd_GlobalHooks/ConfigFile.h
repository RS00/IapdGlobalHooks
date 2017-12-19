#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <Windows.h>
using namespace std;
#define FILE_NAME "Settings.cfg"
#define DEFAULT_EMAIL "hazy142@gmail.com"
#define DEFAULT_MAX_LENGTH "100"
#define DEFAULT_MODE "Default"
#define MODE 0
#define EMAIL 1
#define MAX_VALUE 2
#define ENCRYPT_KEY 87
class ConfigFile
{
private:
	string mode;
	string email;
	string maxLength;
	void encryptFile();
	string decryptFile();
	void parseValues();
	void createConfig(string mode, string email, string maxLength);
public:
	ConfigFile();
	void setNewValues(string mode, string email, string maxLength);
	string getMode();
	string getEmail();
	string getMaxLength();
	~ConfigFile();
};

