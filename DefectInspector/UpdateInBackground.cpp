#include "UpdateInBackground.h"
#include "SqlCommunicator.h"

#include <sstream>

using namespace System;
using namespace System::IO;
using namespace System::Threading;

UpdateInBackground::UpdateInBackground(int DieX, int DieY, int LOT_ID) {
	// ConnectStr
	connectStr = L"Driver={ODBC Driver 17 for SQL Server};server=localhost;database=test;trusted_connection=Yes;";

	// 
	this->index = txnIndex++;
	this->DieX = DieX;
	this->DieY = DieY;
	this->LOT_ID = LOT_ID;
}

void UpdateInBackground::setConnectStr(const wchar_t* connectStr) { this->connectStr = connectStr; }

void UpdateInBackground::running() {

	// 建立資料庫連線 //
	SqlCommunicator* updateSql = new SqlCommunicator(L"Driver={ODBC Driver 17 for SQL Server};server=localhost;database=test;trusted_connection=Yes;");

	// 建構 SQL 命令 (遵循 SQL 語法)
	// 以 stringstream 建構 命令字串
	stringstream ss;

	ss << "DELETE FROM [test].[dbo].[2274_DefectData_TEST_PartALL]";
	ss << " WHERE [DieX] = " << this->DieX << " AND [DieY] = " << this->DieY;
	ss << " AND [Region] = " << this->LOT_ID << " ;";

	//ss << "UPDATE [test].[dbo].[2274_DefectData_TEST_PartALL]";
	//ss << " SET [DefectType] = 0";
	//ss << " WHERE [DieX] = " << info->DieX << " AND [DieY] = " << info->DieY;
	//ss << " AND [Region] = " << info->LOT_ID << ";";

	// 將 string 轉換為 wstring, 以供 SqlCommuncator 使用
	// convert string -> wstring
	string sqlCommand = ss.str();
	wstring updateSqlCommand(sqlCommand.begin(), sqlCommand.end());

	// command = updateSqlCommand.c_str();

	// 呼叫 sqlCommand() 將命令 上傳 SQL Server, 上傳結束後關閉連線
	// 若 SQL執行失敗, 會直接進入 catch 區域, 並顯示錯誤原因
	// send the UPDATE command to SqlCommunicator
	// Console::WriteLine(command);
	Monitor::Enter(updateLock);
	try {
		SQLHSTMT hstmt = updateSql->sqlCommand(updateSqlCommand.c_str());
		updateSql->close();
	}
	finally 
	{
		Monitor::Exit(updateLock);
	}

	// 撰寫進檔案時，以 Mutex鎖 鎖住IO資源，避免 Race Condition
	Monitor::Enter(obj);
	try
	{
		// 將更新資訊寫入 log.csv檔案內
		// 這些寫入資料 承諾 會更新回資料庫，若沒有對應回傳的話，代表更新失敗
		String^ fileName = "log.csv";
		StreamWriter^ sw = gcnew StreamWriter(fileName, true, System::Text::Encoding::UTF8);
		sw->WriteLine("DONE," + this->index + "," + DieX + "," + DieY + "," + LOT_ID + "," + DateTime::Now);
		sw->Close();
	}
	finally
	{
		Monitor::Exit(obj);
	}
}