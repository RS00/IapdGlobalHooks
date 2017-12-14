#pragma once
#include <windows.h>
#include "KeyHook.h"
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
			InitializeComponent();
		}

	protected:
		~MainWindow()
		{
			UnhookWindowsHookEx(keyboardHook);
			if (components)
			{
				delete components;
			}
		}

		static void KeyboardProc(int nCode, WPARAM wParam,
			LPARAM lParam)
		{
			KeyHook hook;
			hook.getNameFromCode(wParam);
			z++;
			return;
		}
		virtual void WndProc(Message %m) override
		{
			switch (m.Msg)
			{
			case WM_KILLFOCUS:
			{

				return;
			}
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
		static int z = 0;
		HHOOK keyboardHook;
		System::ComponentModel::Container ^components;

		void InitializeComponent(void)
		{
			this->SuspendLayout();
			keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)KeyboardProc, 0, 0);
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(284, 261);
			this->Name = L"Form1";
			this->Text = L"Form1";
			this->ResumeLayout(false);
		}
	};
}
