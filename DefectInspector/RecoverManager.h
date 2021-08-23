#pragma once
#include "UpdateDieInfo.h"
#include "LogManager.h"

#include <vector>
#include <string>
using namespace std;

ref class RecoverManager
{
private:
	// 將 CSV 檔案欄位分開並回傳
	vector<string> csvSplit(string csvLog);
	// 檢查 Log 檔案，並回傳 需要更新點的 資訊
	vector<updateDieInfo*> checkLog();

	//
	static LogManager^ logManager = gcnew LogManager();

public:
	// Constructors
	RecoverManager();

	// 開始資料庫恢復作業
	void recovery();
};

