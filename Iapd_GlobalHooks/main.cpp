#include "MainWindow.h"
using namespace System;
using namespace System::Windows::Forms;

[STAThread]
int main() {
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	Application::Run(GlobalHooks::MainWindow::Instance);
	return 0;
}