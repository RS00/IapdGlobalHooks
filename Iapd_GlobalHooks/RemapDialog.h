#pragma once

namespace Iapd_GlobalHooks {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	public ref class RemapDialog : public System::Windows::Forms::Form
	{
	public:
		RemapDialog(void)
		{
			InitializeComponent();
		}

		bool Show()
		{
			this->ShowDialog();
			return success;
		}

		Keys GetKey()
		{
			return key;
		}
	protected:
		~RemapDialog()
		{
			if (components)
			{
				delete components;
			}
		}

	private:
		bool success = false;
		ComboBox ^box;
		Keys key;
		System::ComponentModel::Container ^components;
		void InitializeComponent(void)
		{
			this->components = gcnew System::ComponentModel::Container();
			this->Text = L"Remap";
			this->Padding = System::Windows::Forms::Padding(0);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->Width = 150;
			this->Height = 120;
			this->StartPosition = FormStartPosition::CenterScreen;
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
			box = gcnew ComboBox();
			box->Items->AddRange(Enum::GetNames(Keys::typeid));
			box->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			box->SelectedIndex = 0;
			box->Top = 5;
			box->Left = 5;
			Button ^buttonOk = gcnew Button();
			buttonOk->Text = "Ok";
			buttonOk->Top = 45;
			buttonOk->Left = 30;
			buttonOk->Click += gcnew EventHandler(this, &RemapDialog::ButtonSetRemapPressed);
			this->Controls->Add(box);
			this->Controls->Add(buttonOk);
			this->AcceptButton = buttonOk;
		}

		void ButtonSetRemapPressed(System::Object^  sender, EventArgs^ e)
		{
			success = true;
			key = (Keys)Enum::Parse(Keys::typeid, (String ^)box->SelectedItem);
			this->Close();
		}
	};
}
