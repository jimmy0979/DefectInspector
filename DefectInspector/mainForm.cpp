#include "mainForm.h"

#include <time.h>
#include <string>
#include <sstream>
#include <vector>

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
		roi = new ROI(900, 900);
		die_map = new Map();
		data_controller = new DataControlUnit;
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

	// TODO : amplify function
	if (e->Button == System::Windows::Forms::MouseButtons::Left) {
		int region = (e->Y / 30) * 8 + (e->X / 30);
		cout << "Mouse move over the window - Region (" << region << ")" << endl;
		System::Console::WriteLine("Mouse move over the window - Region (" + region + " )");

		ROI->clear();
		for (int i = 0; i < Dies[region].size(); i++) {
			ROI->paint(Dies[region][i].first, Dies[region][i].second);
		}

		imgROI->Image = MatToBitmap(ROI->returnMat());
	}
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
	SQLHSTMT hstmt = sql->sqlCommand(L"SELECT [Region], [DieX], [DieY] FROM [test].[dbo].[2274_DefectData_TEST_PartALL]");

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

		if (retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO) 
		{
			data_controller->put_data(diex, diey, 255);
		}
		else 
		{
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

	if (this->InvokeRequired) 
	{
		UpdateImage^ uiMapper = gcnew UpdateImage(this, &mainForm::UpdateMapperBitmap);
		UpdateImage^ uiPainter = gcnew UpdateImage(this, &mainForm::UpdatePainterBitmap);
		paint_buffer = data_controller->pulldata(false);
		uiMapper->Invoke(MatToBitmap(die_map->returnMat(paint_buffer,data_controller->return)));
		uiPainter->Invoke(MatToBitmap(ROI->returnMat()));
	}
	else {
		imgROI->Image = MatToBitmap(ROI->returnMat());
		imgMap->Image = MatToBitmap(Map->returnMat());
	}

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