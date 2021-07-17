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
		// 程式初始入口
		Application::EnableVisualStyles();
		Application::SetCompatibleTextRenderingDefault(false);
		
		// 建立 mainForm 物件，並執行 mainForm 視窗
		mainForm form;
		Application::Run(% form);
	}
}