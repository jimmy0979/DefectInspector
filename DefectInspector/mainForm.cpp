#include "mainForm.h"

#include <time.h>

#include <random>
#include <sstream>
#include <string>
#include <vector>
#include <queue>

#include <opencv2\imgproc\types_c.h>

using namespace System::Windows::Forms::DataVisualization::Charting;

using namespace DefectInspector;
using namespace System::Threading;

typedef struct _updateDieInfo {
	int LOT_ID = 0;
	int DieX = 0, DieY = 0;;
	_updateDieInfo(int LOT_ID, int DieX, int DieY) :LOT_ID(LOT_ID), DieX(DieX), DieY(DieY) {}
}updateDieInfo;

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

// upadte Dies
queue<updateDieInfo*> updateDies;
string updateInfoLog;

// stack Image
vector<cv::Mat> comparedFrames;
cv::Mat currentMat;
//===============================

//---------------------------------------------------------------------

System::Drawing::Bitmap^ MatToBitmap(cv::Mat img, bool isROI = false) {
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
		roi = new ROI();
		die_map = new Map();
		data_controller = new DataControlUnit;
		this->Filter_comboBox->SelectedIndex = 0;//initial show data type is "AllDefeat"

		// updatePlot();
		this->chartDies->Series->Clear();
		this->chartDies->Series->Add("DefectType");
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
		// TODO : make user know when UPDATE is done
		
		// construct the command with 3 params : LOT_ID, DieX, DieY
		int size = updateDies.size();
		for (int i = 0; i<size; i++) {
			updateDieInfo* info = updateDies.front();
			updateDies.pop();
			
			sql = new SqlCommunicator(L"Driver={ODBC Driver 17 for SQL Server};server=localhost;database=test;trusted_connection=Yes;");
			
			stringstream ss;

			ss << "DELETE FROM [test].[dbo].[2274_DefectData_TEST_PartALL]";
			ss << " WHERE [DieX] = " << info->DieX << " AND [DieY] = " << info->DieY;
			ss << " AND [Region] = " << info->LOT_ID << " ;";

			//ss << "UPDATE [test].[dbo].[2274_DefectData_TEST_PartALL]";
			//ss << " SET [DefectType] = 0";
			//ss << " WHERE [DieX] = " << info->DieX << " AND [DieY] = " << info->DieY;
			//ss << " AND [Region] = " << info->LOT_ID << ";";

			// convert string -> wstring
			string Sql = ss.str();
			
			// DEBUG
			lblInfo->Text = gcnew String(Sql.c_str());
			
			wstring updateSql(Sql.begin(), Sql.end());

			// TODO : update data_controller

			// send the UPDATE command to SqlCommunicator
			SQLHSTMT hstmt = sql->sqlCommand(updateSql.c_str());
			sql->close();
			// update local data_controller
			int abs_diex = 1000 * ((info->LOT_ID - 1) % 10) + info->DieX;
			int abs_diey = 1000 * ((info->LOT_ID - 1) / 10) + info->DieY;
			data_controller->update_data(abs_diex, abs_diey);

			Drop(1);
			Drop(0);

			// append on Log
			updateInfoLog += Sql;

		}

		this->listDieInfo->Items->Clear();
	}
	catch (System::Exception^ e) {
		lblInfo->Text += "\n------------------\n" + e->Message;
	}
}

System::Void mainForm::imgMap_MouseDown(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {
	// trigger function
	// whenever selected region on Mapper changing, Painter should refresh to correspond die infor.

	// TRANSFER : TODO : amplify function
	// only consider drop & moving
	if (e->Button == System::Windows::Forms::MouseButtons::Left || e->Button == System::Windows::Forms::MouseButtons::Right) {
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
		updateRealTimeInfo();

		xCurrent = xPosition;
		yCurrent = yPosition;

		// Left Click on imgMap only show the mat, return here 
		if (e->Button == System::Windows::Forms::MouseButtons::Left) {
			return;
		}


		/////////////////////////////////////////////////////////////////
		// Right Click on imgMap to select frame that need to be compared
		// add the current mat into comparedFrames
		cv::Mat current = roi->show(data_controller->pull_data(true), data_controller->return_level());
		comparedFrames.push_back(current.clone());

		int Region = data_controller->return_lotId();
		int Level = data_controller->return_level();

		this->listFrames->Items->Add("Region" + Region + ", FrameX = " + xPosition + ", FrameY = " + yPosition + ", Level = " + Level);

	}
}

System::Void mainForm::imgROI_MouseDown(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {
	
	// Right Click on imgROI to select dies that need to update
	if (e->Button == System::Windows::Forms::MouseButtons::Right) {
		// Only level 2 can update dies
		if (data_controller->return_level() != 2)
			return;

		int gridSize = 1;
		switch(data_controller->return_level()) {
		case 1:
			gridSize = 10;
			break;
		case 2:
			gridSize = 100;
			break;
		default:
			gridSize = 10;
			break;
		}
		int yPosition = (e->Y / gridSize), xPosition = (e->X / gridSize);

		int LOT_ID = data_controller->return_lotId();

		// TODO
		pair<int, int> DiePosition = data_controller->return_locat_xy(xPosition, yPosition);
		int DieX = DiePosition.first, DieY = DiePosition.second;

		// DEBUG
		lblInfo->Text += "\nLOT_ID" + LOT_ID + "\nDieX=" + DieX + "\nDieY=" + DieY + "\n";
		this->listDieInfo->Items->Add("Region" + LOT_ID + ", DieX = " + DieX + ", DieY = " + DieY);
		
		updateDies.push(new updateDieInfo(LOT_ID, DieX, DieY));
		lblInfo->Text += "Queue Size" + updateDies.size() + "\n";
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
	updateRealTimeInfo();
}

System::Void mainForm::mainForm_FormClosing(System::Object^ sender, System::Windows::Forms::FormClosingEventArgs^ e) {
	if(sql != nullptr)
		sql->close();
}

System::Void mainForm::tabControl1_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e) {
	// if selectedIndex is 'stackImage', show the compare of comapredFrames
	int selectedIndex = this->tctrlImage->SelectedIndex;
	if (selectedIndex == 1) {
		// BUG : Only Black will be presented

		// in tabPage : tpStackImage
		int len = comparedFrames.size();

		//for (int i = 0; i<len; ++i) {
		//	cv::imshow("showAll", comparedFrames[i]);
		//	cv::waitKey();
		//}

		cv::Mat diff = comparedFrames[0];
		cv::Mat compared = diff.clone();

		lblInfo->Text = "comparedFrames.size() = " + len + "\n";

		for (int i = 1; i<len; ++i) {
			// diff = (diff != comparedFrames[i]);

			// calc the difference
			cv::absdiff(diff, comparedFrames[i], diff);
		}

		//// Get the mask if difference greater than th
		//int th = 10;  // 0
		//cv::Mat mask(diff.size(), CV_8UC1);
		//for (int j = 0; j < diff.rows; ++j) {
		//	for (int i = 0; i < diff.cols; ++i) {
		//		cv::Vec3b pix = diff.at<cv::Vec3b>(j, i);
		//		int val = (pix[0] + pix[1] + pix[2]);
		//		if (val > th) {
		//			mask.at<unsigned char>(j, i) = 255;
		//		}
		//	}
		//}

		// BUG : Can't cvtColor and bitwise_and
		cv::Mat res;
		cv::bitwise_not(diff, diff);
		// cv::cvtColor(diff, diff, CV_BGR2GRAY);
		// cv::bitwise_and(compared, compared, compared, diff);

		// show the diff
		this->imgCompared->Image = MatToBitmap(diff);
	}
	else if (selectedIndex == 0) {
		// refresh the image ROI
		Drop(1);
		Drop(0);
	}
}

System::Void DefectInspector::mainForm::Filter_comboBox_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e)
{
	if (data_controller != nullptr)//check data_controller has been initial
	{
		if (data_controller->change_filter(this->Filter_comboBox->SelectedIndex))//if type didn't change do nothing
		{
			die_map->change_background(data_controller->return_fliter_setting());
			roi->change_background(data_controller->return_fliter_setting());
			imgMap->Image = MatToBitmap(die_map->show(data_controller->pull_data(false), data_controller->return_locat_x(), data_controller->return_locat_y()));
			imgROI->Image = MatToBitmap(roi->show(data_controller->pull_data(true), data_controller->return_level()), true);
		}
	}
	FilterBox->Text = Filter_comboBox->Text;//show what type filter is now
}

System::Void DefectInspector::mainForm::filter_button_Click(System::Object^ sender, System::EventArgs^ e)
{
	if (data_controller != nullptr)//check data_controller has been initial
	{
		if (data_controller->change_filter(this->Filter_comboBox->SelectedIndex))//if type didn't change do nothing
		{
			die_map->change_background(data_controller->return_fliter_setting());
			roi->change_background(data_controller->return_fliter_setting());
			imgMap->Image = MatToBitmap(die_map->show(data_controller->pull_data(false), data_controller->return_locat_x(), data_controller->return_locat_y()));
			imgROI->Image = MatToBitmap(roi->show(data_controller->pull_data(true), data_controller->return_level()), true);
		}
	}
	FilterBox->Text = Filter_comboBox->Text;//show what type filter is now
}

System::Void mainForm::mainForm_Click(System::Object^ sender, System::EventArgs^ e) {
	this->label1->Focus();
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
	sql = nullptr;

	// the consuming time used to load data
	float cost = (float)(clock() - start) / CLOCKS_PER_SEC;

	resInfo = cost.ToString() + "\n";

	// default area = 0 in Painter
	start = clock();

	if (this->InvokeRequired) {
		UpdateImage^ uiMapper = gcnew UpdateImage(this, &mainForm::UpdateMapperBitmap);
		UpdateImage^ uiPainter = gcnew UpdateImage(this, &mainForm::UpdatePainterBitmap);

		uiMapper->Invoke(MatToBitmap(die_map->show(data_controller->pull_data(false), data_controller->return_locat_x(), data_controller->return_locat_y())));
		uiPainter->Invoke(MatToBitmap(roi->show(data_controller->pull_data(true), data_controller->return_level())));
	}
	else {
		imgMap->Image = MatToBitmap(die_map->show(data_controller->pull_data(false), data_controller->return_locat_x(), data_controller->return_locat_y()));
		imgROI->Image = MatToBitmap(roi->show(data_controller->pull_data(true), data_controller->return_level()));
	}

	// (done)TODO : temp solution to avoid black Map occurs, should be deleted later and find the real bug
	//Drop(1);
	//Drop(0);

	// TODO : wait for delegate
	// updatePlot();

	// the consuming time that used to paint die on region 0
	cost = (float)(clock() - start) / CLOCKS_PER_SEC;
	resInfo += cost.ToString() + "\n";

	// make String^ in other thread can be used in main thread's ui lblInfo
	if (this->InvokeRequired) {
	    UpdateText^ uiInfo = gcnew UpdateText(this, &mainForm::UpdateInfoText);
	    Invoke(uiInfo, resInfo, 0);
	}
	else {
	    // convert string to String^ in CLR
	    // lblInfo->Text = gcnew String(text.c_str());
	    lblInfo->Text = resInfo;
	}
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
		imgMap->Image = MatToBitmap(die_map->relocate(data_controller->return_locat_x(), data_controller->return_locat_y()));
		imgROI->Image = MatToBitmap(roi->show(data_controller->pull_data(true), data_controller->return_level()), true);
	}
	if (data_controller->return_map_change()) {
		imgMap->Image = MatToBitmap(die_map->show(data_controller->pull_data(false), data_controller->return_locat_x(), data_controller->return_locat_y()));
	}

	xCurrent = data_controller->return_locat_x();
	yCurrent = data_controller->return_locat_y();
	// lblInfo->Text = "xCurrent=" + xCurrent + "\nyCurrent=" + yCurrent;
}

System::Void mainForm::Amplify(bool amplifyFlag) {
	if (data_controller->change_level(amplifyFlag)) {
		imgMap->Image = MatToBitmap(die_map->show(data_controller->pull_data(false), data_controller->return_locat_x(), data_controller->return_locat_y()));
		imgROI->Image = MatToBitmap(roi->show(data_controller->pull_data(true), data_controller->return_level()));
	}

	xCurrent = data_controller->return_locat_x();
	yCurrent = data_controller->return_locat_y();

	// TODO : temp solution to avoid black Map occurs, should be deleted later and find the real bug
	//Drop(0);
	//Drop(1);
	// lblInfo->Text = "xCurrent=" + xCurrent + "\nyCurrent=" + yCurrent;
}

//---------------------------------------------------------------------

System::Void mainForm::updateRealTimeInfo() {
	this->updatePlot();
	this->updateInfoList();
}

stringstream updateInfoText;

System::Void mainForm::updateInfoList() {
	// call data_controller for real-time info
	this->lblInfo->Text = gcnew String(data_controller->currentInfoList().c_str());;
}

System::Void mainForm::updatePlot() {
	this->chartDies->Series["DefectType"]->Points->Clear();
	updateInfoText.clear();

	map<int, int> res = data_controller->return_defect_count();
	for (pair<int, int> i : res) {
		//this->chart1->Series["DefectType"]->XValueType = ChartValueType::String;
		//this->chart1->Series["DefectType"]->YValueType = ChartValueType::Int64;
		this->chartDies->Series["DefectType"]->Points->AddXY(i.first, i.second);
	}
}

//---------------------------------------------------------------------