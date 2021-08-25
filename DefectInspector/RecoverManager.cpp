#include "RecoverManager.h"
#include "SqlCommunicator.h"

#include <sstream>
#include <set>

using namespace System;
using namespace System::IO;

vector<string> RecoverManager::csvSplit(string csvLog) {
	vector<string> columns;

	int n = csvLog.size();
	int curStartIndex = 0;
	for (int i = 0; i < n; i++) {
		if (csvLog[i] == ',') {
			columns.push_back(csvLog.substr(curStartIndex, i - curStartIndex));
			curStartIndex = i + 1;
		}
	}

	if (curStartIndex != n) {
		columns.push_back(csvLog.substr(curStartIndex, n - curStartIndex + 1));
	}

	return columns;
}

vector<updateDieInfo*> RecoverManager::checkLog() {
	// 讀取 Log 檔案，並回傳需要更新的資料點 資訊

	// 讀取檔案，將其記錄在 content 內
	vector<string> content;
	String^ fileName = "log.csv";
	try
	{
		StreamReader^ din = File::OpenText(fileName);

		String^ str;
		while ((str = din->ReadLine()) != nullptr)
		{
			const char* chars = (const char*)(System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(str)).ToPointer();
			string dest = chars;
			content.push_back(dest);
		}

		din->Close();
	}
	catch (Exception^ e)
	{
		if (dynamic_cast<FileNotFoundException^>(e))
			Console::WriteLine("file '{0}' not found", fileName);
		else
			Console::WriteLine("problem reading file '{0}'", fileName);
	}

	int n = content.size();
	set<int> txnsDone, inLastwill;
	vector<updateDieInfo*> lastwill;

	// 將檔案內容由後至前 開始一一讀取
	for (int line = n - 1; line >= 0; line--) {
		// 反序列化 檔案內容
		vector<string> split = csvSplit(content[line]);
		string txnCode = split[0];		// 第1格 為 交易類型(RECOVER, UPDATE, DONE...)等等
		int txnIndex = stoi(split[1]);	// 第2格 為 交易代碼，每筆交易皆有其唯一代碼，用於追蹤

		if (txnCode == "DONE") {
			// 若交易類型為 DONE，代表此交易已經成功更新至資料庫
			// 將其記錄在 txnsDone
			txnsDone.insert(txnIndex);
		}
		else if (txnCode == "UPDATE" || txnCode == "RECOVER") {
			
			// 若該 更新Index 已經進入更新列表 (lastwill內)，則提早退出，避免二次更新
			if (inLastwill.find(txnIndex) != inLastwill.end())
				continue;

			// 若交易類型為 RECOVER 或 UPDATE，代表交易接受，但還未知此交易是否成功更新至資料庫
			// if txnIndex don't exist in txnDone, it means that this txns broke. 
			if (txnsDone.find(txnIndex) == txnsDone.end()) {
				// 但若 txnsDone 內並未出現 對應交易代碼，代表交易失敗
				// 則進行重新更新

				// 更新晶粒資料
				int DieX = stoi(split[2]), DieY = stoi(split[3]);
				int LOT_ID = stoi(split[4]);

				// RECOVER 的 txnIndex 為負號，以此跟正常更新 Die 做區分
				lastwill.push_back(new updateDieInfo(LOT_ID, DieX, DieY, txnIndex));
				inLastwill.insert(txnIndex);
			}
		}
	}

	return lastwill;
}

RecoverManager::RecoverManager() {
	// do no-thing
}

void RecoverManager::recovery() {
	// 獲取需要更新的資料點資訊
	vector<updateDieInfo*> lastwill = checkLog();

	// 將更新資訊寫入 log.csv檔案內
	// 這些寫入資料 承諾 會更新回資料庫，若沒有對應回傳的話，代表更新失敗
	// 若無資料點需要更新，Early Return
	if (lastwill.size() <= 0) {
		logManager->checkSave();
		return;
	}

	for (int i = 0; i < lastwill.size(); i++) {
		// 建立資料庫連線 //
		SqlCommunicator* updateSql = new SqlCommunicator(L"Driver={ODBC Driver 17 for SQL Server};server=localhost;database=test;trusted_connection=Yes;");

		updateDieInfo* info = lastwill[i];

		// sw->WriteLine("RECOVER," + info->index + "," + info->DieX + "," + info->DieY + "," + info->LOT_ID + "," + DateTime::Now);
		logManager->recoveryToUpdate(info);

		// 建構 SQL 命令 (遵循 SQL 語法)
		// 以 stringstream 建構 命令字串
		stringstream ss;

		ss << "DELETE FROM [test].[dbo].[2274_DefectData_TEST_PartALL]";
		ss << " WHERE [DieX] = " << info->DieX << " AND [DieY] = " << info->DieY;
		ss << " AND [Region] = " << info->LOT_ID << " ;";

		// 將 string 轉換為 wstring, 以供 SqlCommuncator 使用
		// convert string -> wstring
		string sqlCommand = ss.str();
		wstring updateSqlCommand(sqlCommand.begin(), sqlCommand.end());

		// 呼叫 sqlCommand() 將命令 上傳 SQL Server, 上傳結束後關閉連線
		// 若 SQL執行失敗, 會直接進入 catch 區域, 並顯示錯誤原因
		// send the UPDATE command to SqlCommunicator
		SQLHSTMT hstmt = updateSql->sqlCommand(updateSqlCommand.c_str());
		updateSql->close();


		// sw->WriteLine("DONE," + info->index + "," + info->DieX + "," + info->DieY + "," + info->LOT_ID + "," + DateTime::Now);
		logManager->doneUpdate(info);
	}
}