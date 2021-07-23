#include "mainForm.h"
#include "Rendering.h"

#include <time.h>

#include <random>
#include <sstream>
#include <string>
#include <vector>
#include <queue>

#include <opencv2\imgproc\types_c.h>

using namespace DefectInspector;
using namespace System::Windows::Forms::DataVisualization::Charting;
using namespace System::Threading;

typedef struct _updateDieInfo {
	// 更新晶粒資料
	int LOT_ID = 0;
	int DieX = 0, DieY = 0;;
	_updateDieInfo(int LOT_ID, int DieX, int DieY) :LOT_ID(LOT_ID), DieX(DieX), DieY(DieY) {}
} updateDieInfo;

//===============================
// Global Variables Declartion

// OpenCV MAT Image
static ROI* roi;
static Map* die_map;
static DataControlUnit* data_controller;
static vector<paint_unit> paint_buffer;

// 資料庫連線
SqlCommunicator* sql = nullptr;

// Map
static int yCurrent = 0, xCurrent = 0;

// 更新晶粒
static vector<updateDieInfo*> updateDies;	// 儲存準備更新之晶粒資訊
static string updateInfoLog;				// 更新歷史紀錄，可供回顧或反悔用 (TODO: 反悔功能)

// 疊圖
static vector<cv::Mat> comparedFrames;		// 儲存準備疊圖比較之 Frame 圖
static cv::Mat currentMat;					// 
//===============================

//---------------------------------------------------------------------

System::Drawing::Bitmap^ MatToBitmap(cv::Mat img, bool isROI = false) {
	// 將 cv::Mat 轉換為 Bitmap^ 格式
	// 使 clr::pictureBox 可呼叫此函示 以獲取可顯示的Bitmap^格式

	// covert OpenCV::Mat to Bitmap^
	// in order to show on clr::pictureBox simply

	System::IntPtr ptr(img.ptr());
	System::Drawing::Bitmap^ res = gcnew System::Drawing::Bitmap(img.cols, img.rows, img.step, System::Drawing::Imaging::PixelFormat::Format24bppRgb, ptr);
	return res;
}

//---------------------------------------------------------------------
// 事件觸發 函式實踐區

System::Void mainForm::mainForm_Load(System::Object^ sender, System::EventArgs^ e) {
	// mainForm_load
	try {

		// 初始建構 各項元件內容，包括 ROI, MAP... //
		// initial Painter, Mapper
		roi = new ROI();
		die_map = new Map();
		data_controller = new DataControlUnit;

		// 初始化 各項元件內容 //
		// 篩選內容 初始化
		this->Filter_comboBox->SelectedIndex = 0;//initial show data type is "AllDefeat"
		// 即時更新圖表 初始化
		this->chartDies->Series->Clear();
		this->chartDies->Series->Add("DefectType");
		// 載入圖，將 imgROI 設為 Parent 來使背景以 imgROI為基準透明
		this->pictLoading->Visible = false;
		this->pictLoading->Parent = this->imgROI;
	}
	catch (System::Exception^ e) {
		lblInfo->Text = e->Message;
	}
}

//---------------------------------------------------------------------

System::Void mainForm::btnConnectSql_Click(System::Object^ sender, System::EventArgs^ e) {

	// 建立 新執行續 來執行 connectToDb, 以避免類當機的問題//
	// use another thread to deal with connectToDb(),
	//      or it will cause GUI react like "no response"

	Thread^ thr1 = gcnew ::Thread(gcnew ThreadStart(this, &mainForm::connectToDb));
	thr1->Start();

	// TODO: 增加 人性化提示，如等待中
	// 人性化提示, 讓使用者確切知道程式正在執行, 並非當機 //

	// thr1->Join();
}

System::Void mainForm::btnUpdate_Click(System::Object^ sender, System::EventArgs^ e) {
	try {
		Thread^ thr1 = gcnew ::Thread(gcnew ThreadStart(this, &mainForm::updateToDb));
		thr1->Start();
	}
	catch (System::Exception^ e) {
		// SQL 命令錯誤 //
		// 即時資訊 顯示 錯誤訊息
		lblInfo->Text += "\n------------------\n" + e->Message;
	}
}

System::Void mainForm::updateToDb(System::Void){
	// 將 updateDies 內的 更新資訊 上傳至 SQL Server, 進行實際更新

	// TODO : make user know when UPDATE is done
	if (this->InvokeRequired) {
		UpdateLoading^ uiLoadingStart = gcnew UpdateLoading(this, &mainForm::setLoading);
		Invoke(uiLoadingStart, true);
	}
	else {
		setLoading(true);
	}

	// TODO : 可選擇安全性, 選擇與資料庫
	//			1. 同步更新：可確保資料一致性，畫面更改時，資料庫必定已經改好了
	//			2. 非同步更新：資料不一定一致，畫面更改時，資料庫可能尚未更新。不過會確保資料最終一致

	// 更新命令 主要 由(Region, DieX, DieY) 來 確定更新點位置
	// construct the command with 3 params : Region, DieX, DieY
	int size = updateDies.size();
	for (int i = 0; i < size; i++) {
		updateDieInfo* info = updateDies[i];

		// 建立資料庫連線 //
		SqlCommunicator* updateSql = new SqlCommunicator(L"Driver={ODBC Driver 17 for SQL Server};server=localhost;database=test;trusted_connection=Yes;");

		// 將更新資訊上傳至資料庫 //
		// 建構 SQL 命令 (遵循 SQL 語法)
		// 以 stringstream 建構 命令字串
		stringstream ss;

		ss << "DELETE FROM [test].[dbo].[2274_DefectData_TEST_PartALL]";
		ss << " WHERE [DieX] = " << info->DieX << " AND [DieY] = " << info->DieY;
		ss << " AND [Region] = " << info->LOT_ID << " ;";

		//ss << "UPDATE [test].[dbo].[2274_DefectData_TEST_PartALL]";
		//ss << " SET [DefectType] = 0";
		//ss << " WHERE [DieX] = " << info->DieX << " AND [DieY] = " << info->DieY;
		//ss << " AND [Region] = " << info->LOT_ID << ";";

		// 將 string 轉換為 wstring, 以供 SqlCommuncator 使用
		// convert string -> wstring
		string sqlCommand = ss.str();
		wstring updateSqlCommand(sqlCommand.begin(), sqlCommand.end());

		// 呼叫 sqlCommand() 將命令 上傳 SQL Server, 上傳結束後關閉連線
		// 若 SQL執行失敗, 會直接進入 catch 區域, 並顯示錯誤原因
		// send the UPDATE command to SqlCommunicator
		SQLHSTMT hstmt = updateSql->sqlCommand(updateSqlCommand.c_str());
		updateSql->close();

		// 本地端資料更新 //
		// 本地端 呼叫 DataController::update_data() 更新
		// update local data_controller
		int abs_diex = 1000 * ((info->LOT_ID - 1) % 10) + info->DieX;
		int abs_diey = 1000 * ((info->LOT_ID - 1) / 10) + info->DieY;
		data_controller->update_data(abs_diex, abs_diey);

		// 更新 ROI 圖案
		Drop(1);
		Drop(0);

		// 紀錄歷史更新資訊 //
		// append on Log
		updateInfoLog += sqlCommand;

	}
	
	// 更新完成，清空容器
	updateDies.clear();
	
	if (this->InvokeRequired) {
		// 更新完成, 將 UI元件 內容清空 
		UpdateInterface^ uiInterface = gcnew UpdateInterface(this, &mainForm::UpdateUpdateDies);
		Invoke(uiInterface);
	}
	else {
		// 更新完成, 將 UI元件 內容清空 
		this->listDieInfo->Items->Clear();
	}

	if (this->InvokeRequired) {
		UpdateLoading^ uiLoadingStart = gcnew UpdateLoading(this, &mainForm::setLoading);
		Invoke(uiLoadingStart, false);
	}
	else {
		setLoading(false);
	}
}

System::Void mainForm::imgMap_MouseDown(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {
	// trigger function
	// whenever selected region on Mapper changing, Painter should refresh to correspond die info.

	// TRANSFER : TODO : amplify function
	// only consider drop & moving
	if (e->Button == System::Windows::Forms::MouseButtons::Left || e->Button == System::Windows::Forms::MouseButtons::Right) {
		// 左右鍵 皆須 定位晶粒位置，並更新 ROI, MAP //
		
		// 獲取 滑鼠屬標 所在的格子點 (0-indexed)
		int yPosition = (e->Y / 48), xPosition = (e->X / 24);

		// 即時資訊 顯示 選取的晶粒位置
		this->lblInfo->Text = "";
		lblInfo->Text = "xPosition=" + xPosition + "\nyPosition=" + yPosition + "\n";

		// 更新 MAP, ROI 圖片 //
		// 計算 當前晶粒點 與 前一晶粒點 位移
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

		// 其他即時資訊更新
		updateRealTimeInfo();

		// 更新 晶粒位置，供下次計算位移量用
		xCurrent = xPosition;
		yCurrent = yPosition;

		// Left Click on imgMap only show the mat, return here 
		if (e->Button == System::Windows::Forms::MouseButtons::Left) {
			return;
		}


		/////////////////////////////////////////////////////////////////
		// 右鍵點擊 Map 以選取 待疊圖的 Frames
		// Right Click on imgMap to select frame that need to be compared
		// add the current mat into comparedFrames

		// 將當前 ROI 圖選取，並放入 comparedFrames 容器內
		cv::Mat current = roi->show(data_controller->pull_data(true), data_controller->return_level());
		comparedFrames.push_back(current.clone());

		// 在 listFrames 中 顯示 以選取之 Frames 內容
		int Region = data_controller->return_lotId();
		int Level = data_controller->return_level();
		this->listFrames->Items->Add("Region" + Region + ", FrameX = " + xPosition + ", FrameY = " + yPosition + ", Level = " + Level);

	}
}

System::Void mainForm::imgROI_MouseDown(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {
	
	// 右邊點擊 晶粒點 以進行後續更新
	// 更新晶粒 會 先儲存在 updateDies 容器內
	// Right Click on imgROI to select dies that need to update
	if (e->Button == System::Windows::Forms::MouseButtons::Right) {

		// 若 當前放大等級 不是最大(level != 2), 不進行任何動作
		// 僅在 最大放大等級 時, 開放選取 晶粒 更新
		// Only level 2 can update dies
		if (data_controller->return_level() != 2)
			return;

		// 確定 更新點 地理資料 //
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
		// 獲取 相對位置(x, y) 及 當前 更新點所處之 Region
		int yPosition = (e->Y / gridSize), xPosition = (e->X / gridSize);
		int LOT_ID = data_controller->return_lotId();

		// 將相對資料傳給 DataController, 獲得絕對位置 以進行 資料庫更新
		// 呼叫 return_locat_xy() 得到 資料 在 資料庫的格式
		pair<int, int> DiePosition = data_controller->return_locat_xy(xPosition, yPosition);
		int DieX = DiePosition.first, DieY = DiePosition.second;

		// 確認資料沒有重複
		// TODO : 以 更加的 資料結構 確認有無重複
		for (int i = 0; i < updateDies.size(); i++) {
			updateDieInfo* cur = updateDies[i];
			if (cur->LOT_ID == LOT_ID && cur->DieX == DieX && cur->DieY == DieY) {
				// 已經記錄過了，重複，直接 Early return
				return ;
			}
		}

		// 即時資訊 更新 //
		lblInfo->Text += "\nLOT_ID" + LOT_ID + "\nDieX=" + DieX + "\nDieY=" + DieY + "\n";
		this->listDieInfo->Items->Add("Region" + LOT_ID + ", DieX = " + DieX + ", DieY = " + DieY);
		
		// 將資料 存進 updateDies 容器內 //
		updateDies.push_back(new updateDieInfo(LOT_ID, DieX, DieY));
	}
}

System::Void mainForm::mainForm_KeyDown(System::Object^ sender, System::Windows::Forms::KeyEventArgs^ e) {
	// 鍵盤觸發函式 //
	// trigger function
	// whenever selected region on Mapper changing, Painter should refresh to correspond die infor.

	// TODO : a section to check whether info are loaded to prevent null section

	// directions : {0: left, 1: right, 2: up, 3: down}
	bool isDrop = false, isAmplify = false;
	int dir = -1;              // for Drop
	bool amplifyFlag = true;   // for Amplify

	// 依鍵盤輸入 獲得 參數
	// get the direction from keypad input
	switch (e->KeyCode) {
	// 上下左右位移 鍵位
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

	// 放大, 縮小 鍵位
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

	// 若鍵盤沒觸發任何更新，Early retrun
	// if nothing is done, just return
	if (!(isDrop || isAmplify)) return;

	// 觸發 位移 更新
	if (isDrop) {
		Drop(dir);
	}
	// 觸發 縮放 更新
	if (isAmplify) {
		Amplify(amplifyFlag);
	}

	// 其他即時資訊更新
	updateRealTimeInfo();
}

System::Void mainForm::mainForm_FormClosing(System::Object^ sender, System::Windows::Forms::FormClosingEventArgs^ e) {
	// 總程式關閉 //

	// 若 sql 連線尚存在，關閉連線，避免占用資料庫
	if(sql != nullptr)
		sql->close();
}

System::Void mainForm::tabControl1_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e) {
	// 得知當前所處之 tabPage Index
	int selectedIndex = this->tctrlImage->SelectedIndex;

	if (selectedIndex == 1) {
		// in tabPage : tpStackImage
		// if selectedIndex is 'stackImage', show the compare of comapredFrames

		int len = comparedFrames.size();

		//// DEBUG
		//for (int i = 0; i<len; ++i) {
		//	cv::imshow("showAll", comparedFrames[i]);
		//	cv::waitKey();
		//}

		// 取 comparedFrames 第一張 Mat 作為比較圖
		cv::Mat diff = comparedFrames[0];
		cv::Mat compared = diff.clone();

		// 將所有在 comparedFrames的 Mat 一一與 diff 比較
		// diff 會在不斷比較中更新
		for (int i = 1; i<len; ++i) {
			// diff = (diff != comparedFrames[i]);

			// 將diff, comparedFrames[i] 差異 更新至 diff
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

		// 顯示疊圖結果
		// show the diff
		this->imgCompared->Image = MatToBitmap(diff);
	}
	else if (selectedIndex == 0) {
		// in tabPage : tpROI
		// 退回 tpROI時，應將畫面更改為 非疊圖模式

		//// refresh the image ROI
		//Drop(1);
		//Drop(0);

		//
		imgMap->Image = MatToBitmap(die_map->show(data_controller->pull_data(false), data_controller->return_locat_x(), data_controller->return_locat_y()));
		imgROI->Image = MatToBitmap(roi->show(data_controller->pull_data(true), data_controller->return_level()));
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

	if (this->InvokeRequired) {
		UpdateLoading^ uiLoadingStart = gcnew UpdateLoading(this, &mainForm::setLoading);
		Invoke(uiLoadingStart, true);
	}
	else {
		setLoading(true);
	}


	// collects info. that needs to update to a element in ui
	// will later call delegate to help display the info. on the element
	String^ resInfo;

	// 對資料庫 下 SELECT 命令，進行資料載入 //

	// 以 連線字串 連線至 資料庫 SQL Server
	// connect to database with its ConnectString
	sql = new SqlCommunicator(L"Driver={ODBC Driver 17 for SQL Server};server=localhost;database=test;trusted_connection=Yes;");
	
	// make a query and get the statmentHandle
	SQLHSTMT hstmt = sql->sqlCommand(L"SELECT [Region], [DieX], [DieY] FROM [test].[dbo].[2274_DefectData_TEST_PartALL] WHERE [Region] <= 100");
	
	// 依 SELECT 命令 放入變數空間
	SQLBIGINT region, diex, diey;
	SQLLEN cbRegion = 0, cbDiex = 0, cbDiey = 0;

	// seperate each column from HSTMT
	SQLBindCol(hstmt, 1, SQL_C_SBIGINT, &region, 0, &cbRegion);
	SQLBindCol(hstmt, 2, SQL_C_SBIGINT, &diex, 0, &cbDiex);
	SQLBindCol(hstmt, 3, SQL_C_SBIGINT, &diey, 0, &cbDiey);

	// 開始計時 資料載入時間
	clock_t start = clock();

	// 匯入資料
	while (true) {
		// 獲得命令回傳
		// the return state of each ODBC command
		SQLRETURN retCode = SQLFetch(hstmt);

		if (retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO) {
			// 若 命令執行成功，將資料存入 DataController
			int abs_diex = 1000 * ((region -1)% 10) + diex;
			int abs_diey = 1000 * ((region -1)/ 10) + diey;
			data_controller->put_data(abs_diex, abs_diey, 255);
		}
		else {
			// 資料提取結束，結束迴圈
			break;
		}
	}
	// 資料提取結束，關閉資料庫連線
	sql->close();
	sql = nullptr;


	// 計算資料載入花費時間(單位：秒)
	// the consuming time used to load data
	float cost = (float)(clock() - start) / CLOCKS_PER_SEC;
	resInfo = cost.ToString() + "\n";

	// 開始計時 繪圖時間
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

	// TODO : wait for delegate
	// updatePlot();
	
	// 計算繪製花費時間(單位：秒)
	// the consuming time that used to paint die on region 0
	cost = (float)(clock() - start) / CLOCKS_PER_SEC;

	// 將花費時間顯示至即時資訊欄位 //
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

	if (this->InvokeRequired) {
		UpdateLoading^ uiLoadingEnd = gcnew UpdateLoading(this, &mainForm::setLoading);
		Invoke(uiLoadingEnd, false);
	}
	else {
		setLoading(false);
	}
}

//---------------------------------------------------------------------
// 對應委派 函式宣告

System::Void mainForm::UpdateUpdateDies(System::Void) {
	this->listDieInfo->Items->Clear();
}

System::Void mainForm::UpdateMapperBitmap(Bitmap^ uImage) {
	this->imgMap->Image = uImage;
}

System::Void mainForm::UpdatePainterBitmap(Bitmap^ uImage) {
	this->imgROI->Image = uImage;
}

System::Void mainForm::UpdateInfoText(String^ uText, int mode) {
	// update text in different way by its mode
	// mode (0, 1) = (overwrite, append)

	switch (mode) {
	case 0:
		this->lblInfo->Text = uText;
		break;
	case 1:
		this->lblInfo->Text += uText;
		break;
	default:
		this->lblInfo->Text = uText;
		break;
	}
}

System::Void mainForm::setLoading(bool displayLoader) {
	// displayLoader {0: 等待視窗關閉, 1:等待視窗開啟}
	if (displayLoader)
	{
		// 等待視窗開啟 // 
		this->pictLoading->Visible = true;
		this->Cursor = System::Windows::Forms::Cursors::WaitCursor;
		// this->Enabled = false;
		
		//if (this->imgROI->Image != nullptr) {
		//	Rendering::BlendPictures(this->imgROI, this->pictLoading);
		//}
	}
	else
	{
		// 等待視窗關閉 // 
		this->pictLoading->Visible = false;
		this->Enabled = true;
		this->Cursor = System::Windows::Forms::Cursors::Default;
	}
}

//---------------------------------------------------------------------
// ROI, MAP 圖更新

System::Void mainForm::Drop(int dir) {
	// 依照輸入方向，位移當前晶粒格子點，並同時更新 MAP, ROI 畫面	
	// directions : {0: left, 1: right, 2: up, 3: down}

	// 呼叫 DataController
	if (data_controller->change_block(dir)) {
		imgMap->Image = MatToBitmap(die_map->relocate(data_controller->return_locat_x(), data_controller->return_locat_y()));
		imgROI->Image = MatToBitmap(roi->show(data_controller->pull_data(true), data_controller->return_level()), true);
	}
	if (data_controller->return_map_change()) {
		imgMap->Image = MatToBitmap(die_map->show(data_controller->pull_data(false), data_controller->return_locat_x(), data_controller->return_locat_y()));
	}
	// 更新 當前所處的晶粒格子點位置
	xCurrent = data_controller->return_locat_x();
	yCurrent = data_controller->return_locat_y();
	// lblInfo->Text = "xCurrent=" + xCurrent + "\nyCurrent=" + yCurrent;
}

System::Void mainForm::Amplify(bool amplifyFlag) {
	// 依照 amplifyFlag 去更改縮放
	// amplifyFlag : {0: shrink, 1:amplify}

	// 呼叫 DataController
	if (data_controller->change_level(amplifyFlag)) {
		imgMap->Image = MatToBitmap(die_map->show(data_controller->pull_data(false), data_controller->return_locat_x(), data_controller->return_locat_y()));
		imgROI->Image = MatToBitmap(roi->show(data_controller->pull_data(true), data_controller->return_level()));
	}

	// 更新 當前所處的晶粒格子點位置
	xCurrent = data_controller->return_locat_x();
	yCurrent = data_controller->return_locat_y();
	// lblInfo->Text = "xCurrent=" + xCurrent + "\nyCurrent=" + yCurrent;
}

//---------------------------------------------------------------------
// 即時資訊更新

System::Void mainForm::updateRealTimeInfo() {
	// 即時資訊更新, 包括 數據圖表, 布告欄等等 //
	this->updatePlot();
	this->updateInfoList();
}

System::Void mainForm::updateInfoList() {
	// 即時更新 布告欄 //

	// 呼叫 DataController 獲取 需顯示之資料
	// call data_controller for real-time info
	this->lblInfo->Text = gcnew String(data_controller->currentInfoList().c_str());;
}

System::Void mainForm::updatePlot() {
	// 即時更新圖表 //

	// 初始化
	// 將當前圖表資訊 清空初始化
	this->chartDies->Series["DefectType"]->Points->Clear();
	
	// 繪製圖表 // 
	// 呼叫 DataController 獲取 需繪製之資料
	map<int, int> res = data_controller->return_defect_count();
	for (pair<int, int> i : res) {
		//// XY 資料點 型態轉換
		//this->chart1->Series["DefectType"]->XValueType = ChartValueType::String;
		//this->chart1->Series["DefectType"]->YValueType = ChartValueType::Int64;
		
		// 繪製資料點
		this->chartDies->Series["DefectType"]->Points->AddXY(i.first, i.second);
	}
}

//---------------------------------------------------------------------


//---------------------------------------------------------------------