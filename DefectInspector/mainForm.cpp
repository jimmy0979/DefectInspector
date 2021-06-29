#include "mainForm.h"

#include <time.h>

#include <random>
#include <sstream>
#include <string>
#include <vector>
using namespace System::Windows::Forms::DataVisualization::Charting;

using namespace DefectInspector;
using namespace System::Threading;

//===============================
// Global Variables Declartion

// OpenCV MAT Image
ROI* roi;
Map* die_map;
DataControlUnit* data_controller;
vector<paint_unit> paint_buffer;

// SQL Server Connect
SqlCommunicator* sql = nullptr;

// Map
int yCurrent = 0, xCurrent = 0;

//===============================

//---------------------------------------------------------------------

System::Drawing::Bitmap^ MatToBitmap(cv::Mat img) {
	// covert OpenCV::Mat to Bitmap^
	// in order to show on clr::pictureBox simply

	System::IntPtr ptr(img.ptr());
	System::Drawing::Bitmap^ b = gcnew System::Drawing::Bitmap(img.cols, img.rows, img.step, System::Drawing::Imaging::PixelFormat::Format24bppRgb, ptr);
	return b;
}

//---------------------------------------------------------------------

System::Void mainForm::mainForm_Load(System::Object^ sender, System::EventArgs^ e) {
	// mainForm_load
	try {
		// connect to database with its ConnectString
		sql = new SqlCommunicator(L"Driver={ODBC Driver 17 for SQL Server};server=localhost;database=test;trusted_connection=Yes;");

		// initial Painter, Mapper
		roi = new ROI(1000, 1000);
		die_map = new Map();
		data_controller = new DataControlUnit;

		// updatePlot();
		this->chart1->Series->Clear();
		this->chart1->Series->Add("DefectType");
	}
	catch (System::Exception^ e) {
		lblInfo->Text = e->Message;
	}
}

//---------------------------------------------------------------------

System::Void mainForm::btnConnectSql_Click(System::Object^ sender, System::EventArgs^ e) {
	// click to connect
	// connect to dataBase
	// connectToDb();

	// use another thread to deal with connectToDb(),
	//      or it will cause GUI react like "no response"

	Thread^ thr1 = gcnew ::Thread(gcnew ThreadStart(this, &mainForm::connectToDb));
	thr1->Start();
	// thr1->Join();
}

System::Void mainForm::btnUpdate_Click(System::Object^ sender, System::EventArgs^ e) {
	try {
		// make a query and get the statmentHandle
		// TODO : assoicate the update with amplify function
		SQLHSTMT hstmt = sql->sqlCommand(L"UPDATE [test].[dbo].[2274_DefectData_TEST_PartALL] SET[LOT_ID] = 131 WHERE[DieX] = 131 AND[DieY] = 31; ");
	}
	catch (System::Exception^ e) {
		lblInfo->Text = e->Message;
	}
}

System::Void mainForm::imgMap_MouseDown(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {
	// trigger function
	// whenever selected region on Mapper changing, Painter should refresh to correspond die infor.

	// TRANSFER : TODO : amplify function
	// only consider drop & moving
	if (e->Button == System::Windows::Forms::MouseButtons::Left) {
		int yPosition = (e->Y / 48), xPosition = (e->X / 24);
		// if (48 * yPosition < e->Y)	yPosition++;
		// if (24 * xPosition < e->X)	xPosition++;
		this->lblInfo->Text = "";
		lblInfo->Text = "xPosition=" + xPosition + "\nyPosition=" + yPosition + "\n";
		int yDisplace = yPosition - yCurrent, xDisplace = xPosition - xCurrent;

		// directions : {0: left, 1: right, 2: up, 3: down}
		// yDisplace movement
		for (int i = 0; i < abs(yDisplace); i++) {
			int dir = (yDisplace > 0) ? 3 : 2;
			Drop(dir);
		}

		// xDisplace movement
		for (int i = 0; i < abs(xDisplace); i++) {
			int dir = (xDisplace > 0) ? 1 : 0;
			Drop(dir);
		}

		//
		updatePlot();

		xCurrent = xPosition;
		yCurrent = yPosition;
	}
}

System::Void mainForm::mainForm_KeyDown(System::Object^ sender, System::Windows::Forms::KeyEventArgs^ e) {
	// trigger function
	// whenever selected region on Mapper changing, Painter should refresh to correspond die infor.

	// TODO : a section to check whether info are loaded.
	// tp prevent null section

	// only consider drop & moving

	// directions : {0: left, 1: right, 2: up, 3: down}
	bool isDrop = false, isAmplify = false;
	int dir = -1;              // for Drop
	bool amplifyFlag = true;   // for Amplify

	// get the direction from keypad input
	switch (e->KeyCode) {
	case Keys::W:
	case Keys::Up:
		isDrop = true;
		dir = 2;
		break;
	case Keys::S:
	case Keys::Down:
		isDrop = true;
		dir = 3;
		break;
	case Keys::A:
	case Keys::Left:
		isDrop = true;
		dir = 0;
		break;
	case Keys::D:
	case Keys::Right:
		isDrop = true;
		dir = 1;
		break;

	case Keys::K:
		isAmplify = true;
		amplifyFlag = true;   // amplify
		break;
	case Keys::L:
		isAmplify = true;
		amplifyFlag = false;   // shrink
		break;

	default:
		dir = -1;
		break;
	}

	// if nothing is done, just return
	if (!(isDrop || isAmplify)) return;

	if (isDrop) {
		Drop(dir);
	}

	if (isAmplify) {
		Amplify(amplifyFlag);
	}

	//
	updatePlot();
}

//---------------------------------------------------------------------
// Datbase related
System::Void mainForm::connectToDb(System::Void) {
	// TODO : warp the statement here in <Class> SqlCommunicator
	//                  to make it more simply to operate the database

	// collects info. that needs to update to a element in ui
	// will later call delegate to help display the info. on the element
	String^ resInfo;

	// make a query and get the statmentHandle
	// WARNING : can only divide infos into 10*10 regions
	SQLHSTMT hstmt = sql->sqlCommand(L"SELECT [Region], [DieX], [DieY] FROM [test].[dbo].[2274_DefectData_TEST_PartALL] WHERE [Region] = 1");

	// SQL ODBC Connect
	SQLBIGINT region, diex, diey;
	SQLLEN cbRegion = 0, cbDiex = 0, cbDiey = 0;

	// seperate each column from HSTMT
	SQLBindCol(hstmt, 1, SQL_C_SBIGINT, &region, 0, &cbRegion);
	SQLBindCol(hstmt, 2, SQL_C_SBIGINT, &diex, 0, &cbDiex);
	SQLBindCol(hstmt, 3, SQL_C_SBIGINT, &diey, 0, &cbDiey);

	// put each info into dataStructure
	clock_t start = clock();

	while (true) {
		// the return state of each ODBC command
		SQLRETURN retCode = SQLFetch(hstmt);

		if (retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO) {
			int abs_diex = 1000 * ((region -1)% 10) + diex;
			int abs_diey = 1000 * ((region -1)/ 10) + diey;
			data_controller->put_data(abs_diex, abs_diey, 255);
		}
		else {
			break;
		}
	}
	sql->close();

	// the consuming time used to load data
	float cost = (float)(clock() - start) / CLOCKS_PER_SEC;

	resInfo = cost.ToString() + "\n";

	// default area = 0 in Painter
	start = clock();
	/*
	int area = 0;
	for (int i = 0; i < Dies[area].size(); i++) {
		ROI->paint(Dies[area][i].first, Dies[area][i].second);
	}
	*/

	if (this->InvokeRequired) {
		UpdateImage^ uiMapper = gcnew UpdateImage(this, &mainForm::UpdateMapperBitmap);
		UpdateImage^ uiPainter = gcnew UpdateImage(this, &mainForm::UpdatePainterBitmap);

		uiMapper->Invoke(MatToBitmap(die_map->show(data_controller->pull_data(false), data_controller->return_level(), data_controller->return_locat_x(), data_controller->return_locat_y())));
		uiPainter->Invoke(MatToBitmap(roi->show(data_controller->pull_data(true), data_controller->return_level())));
	}
	else {
		imgMap->Image = MatToBitmap(die_map->show(data_controller->pull_data(false), data_controller->return_level(), data_controller->return_locat_x(), data_controller->return_locat_y()));
		imgROI->Image = MatToBitmap(roi->show(data_controller->pull_data(true), data_controller->return_level()));
	}

	// TODO : temp solution to avoid black Map occurs, should be deleted later and find the real bug
	Drop(1);
	Drop(0);

	// TODO : wait for delegate
	// updatePlot();

	// the consuming time that used to paint die on region 0
	cost = (float)(clock() - start) / CLOCKS_PER_SEC;
	resInfo += cost.ToString() + "\n";

	//// TODO : make String^ in other thread can be used in main thread's ui lblInfo
	//// BUG  : System.InvalidOperationException: '跨執行緒作業無效: 存取控制項 'lblInfo' 時所使用的執行緒與建立控制項的執行緒不同。'
	//if (this->InvokeRequired) {
	//    UpdateText^ uiInfo = gcnew UpdateText(this, &mainForm::UpdateInfoText);
	//    uiInfo->Invoke(resInfo, 0);
	//}
	//else {
	//    // convert string to String^ in CLR
	//    // lblInfo->Text = gcnew String(text.c_str());
	//    lblInfo->Text = resInfo;
	//}
}

//---------------------------------------------------------------------

System::Void mainForm::UpdateMapperBitmap(Bitmap^ uImage) {
	imgMap->Image = uImage;
}

System::Void mainForm::UpdatePainterBitmap(Bitmap^ uImage) {
	imgROI->Image = uImage;
}

System::Void mainForm::UpdateInfoText(String^ uText, int mode) {
	// update text in different way by its mode
	// mode (0, 1) = (overwrite, append)

	switch (mode) {
	case 0:
		lblInfo->Text = uText;
		break;
	case 1:
		lblInfo->Text += uText;
		break;
	default:
		lblInfo->Text = uText;
		break;
	}
}

//---------------------------------------------------------------------

System::Void mainForm::Drop(int dir) {
	// directions : {0: left, 1: right, 2: up, 3: down}
	// then move the block by the direction
	if (data_controller->change_block(dir)) {
		imgROI->Image = MatToBitmap(roi->show(data_controller->pull_data(true), data_controller->return_level()));
		imgMap->Image = MatToBitmap(die_map->relocate(data_controller->return_locat_x(), data_controller->return_locat_y(), data_controller->return_level()));
	}
	if (data_controller->return_map_change()) {
		imgMap->Image = MatToBitmap(die_map->show(data_controller->pull_data(false), data_controller->return_level(), data_controller->return_locat_x(), data_controller->return_locat_y()));
	}

	xCurrent = data_controller->return_locat_x();
	yCurrent = data_controller->return_locat_y();
	// lblInfo->Text = "xCurrent=" + xCurrent + "\nyCurrent=" + yCurrent;
}

System::Void mainForm::Amplify(bool amplifyFlag) {
	if (data_controller->change_level(amplifyFlag)) {
		imgMap->Image = MatToBitmap(die_map->show(data_controller->pull_data(false), data_controller->return_level(), data_controller->return_locat_x(), data_controller->return_locat_y()));
		imgROI->Image = MatToBitmap(roi->show(data_controller->pull_data(true), data_controller->return_level()));
	}

	xCurrent = data_controller->return_locat_x();
	yCurrent = data_controller->return_locat_y();

	// TODO : temp solution to avoid black Map occurs, should be deleted later and find the real bug
	Drop(0);
	Drop(1);
	// lblInfo->Text = "xCurrent=" + xCurrent + "\nyCurrent=" + yCurrent;
}

//---------------------------------------------------------------------

System::Void mainForm::updatePlot() {
	this->chart1->Series["DefectType"]->Points->Clear();
	this->lblInfo->Text = "";

	map<int, int> res = data_controller->return_defect_count();
	for (pair<int, int> i : res) {
		this->lblInfo->Text += i.first + " -> " + i.second + "\n";
		//this->chart1->Series["DefectType"]->XValueType = ChartValueType::String;
		//this->chart1->Series["DefectType"]->YValueType = ChartValueType::Int64;
		this->chart1->Series["DefectType"]->Points->AddXY(i.first, i.second);
	}
}

//---------------------------------------------------------------------