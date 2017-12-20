#pragma once
#include <string>
#include "ConfigFile.h"
using namespace std;

namespace Iapd_GlobalHooks {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	public ref class ConfigForm : public System::Windows::Forms::Form
	{
	public:
		ConfigForm(void)
		{
			InitializeComponent();
		}

		string GetEmail()
		{
			return msclr::interop::marshal_as<std::string>(emailBox->Text);
		}

		string GetMode()
		{
			return msclr::interop::marshal_as<std::string>(modeBox->Text);
		}

		string GetMaxLength()
		{
			return msclr::interop::marshal_as<std::string>(maxLengthBox->Text);
		}

		void SetMail(String ^mail)
		{
			emailBox->Text = mail;
		}

		void SetMaxLength(String ^maxLength)
		{
			maxLengthBox->Text = maxLength;
		}

		void SetMode(String ^mode)
		{
			modeBox->Text = mode;
		}

		bool Show()
		{
			this->ShowDialog();
			return success;
		}

	protected:
		~ConfigForm()
		{
			if (components)
			{
				delete components;
			}
		}

	private:
		bool success = false;
		TextBox ^emailBox = gcnew TextBox();
		TextBox ^maxLengthBox = gcnew TextBox();
		TextBox ^modeBox = gcnew TextBox();
		System::ComponentModel::Container ^components;
		void InitializeComponent(void)
		{
			this->components = gcnew System::ComponentModel::Container();
			this->Text = L"Config file";
			this->Padding = System::Windows::Forms::Padding(0);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->Width = 300;
			this->Height = 200;
			this->StartPosition = FormStartPosition::CenterScreen;
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
			Label ^emailLabel = gcnew Label();
			emailLabel->Top = 20;
			emailLabel->Left = 30;
			emailLabel->Text = "Email:";
			emailBox->Top = 20;
			emailBox->Left = 140;
			emailBox->Width = 115;
			Label ^maxLengthLabel = gcnew Label();
			maxLengthLabel->Top = 50;
			maxLengthLabel->Left = 30;
			maxLengthLabel->Text = "Max file length:";
			maxLengthBox->Top = 50;
			maxLengthBox->Left = 140;
			maxLengthBox->Width = 115;
			Label ^modeLabel = gcnew Label();
			modeLabel->Top = 80;
			modeLabel->Left = 30;
			modeLabel->Text = "Start mode:";
			modeBox->Top = 80;
			modeBox->Left = 140;
			modeBox->Width = 115;
			Button ^buttonOk = gcnew Button();
			buttonOk->Text = "Ok";
			buttonOk->Top = 120;
			buttonOk->Left = 100;
			buttonOk->Click += gcnew EventHandler(this, &ConfigForm::ButtonSetConfigPressed);
			this->Controls->Add(emailLabel);
			this->Controls->Add(emailBox);
			this->Controls->Add(maxLengthLabel);
			this->Controls->Add(maxLengthBox);
			this->Controls->Add(modeLabel);
			this->Controls->Add(modeBox);
			this->Controls->Add(buttonOk);
			this->AcceptButton = buttonOk;
		}

		void ButtonSetConfigPressed(System::Object^  sender, EventArgs^ e)
		{
			success = true;
			this->Close();
		}
	};
}
