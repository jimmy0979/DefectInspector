/*******************************************************************************
 * Entry Point of the whole Project
 * Will new a mainForm to start a windows
 *******************************************************************************/

#pragma once
#include "mainForm.h"

namespace DefectInspector {

	using namespace System;
	using namespace System::Windows::Forms;

	[STAThreadAttribute]
	void main(cli::array<String^>^ args) {
		Application::EnableVisualStyles();
		Application::SetCompatibleTextRenderingDefault(false);
		mainForm form;
		Application::Run(% form);
	}
}