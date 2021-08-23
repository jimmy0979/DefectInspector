#include "LogManager.h"

using namespace System;
using namespace System::IO;
using namespace System::Threading;

LogManager::LogManager() {

}

void LogManager::guaranteeToUpdate(vector<updateDieInfo*> infos) {
	//

	// 撰寫進檔案時，以 Mutex鎖 鎖住IO資源，避免 Race Condition
	Monitor::Enter(writeLock);
	try
	{
		String^ filePath = "log.csv";
		StreamWriter^ sw = gcnew StreamWriter(filePath, true, System::Text::Encoding::UTF8);
		for (updateDieInfo* info : infos)
		{
			sw->WriteLine("UPDATE," + info->index + "," + info->DieX + "," + info->DieY + "," + info->LOT_ID + "," + DateTime::Now);
		}
		sw->Close();
	}
	finally
	{
		Monitor::Exit(writeLock);
	}
}

void LogManager::recoveryToUpdate(updateDieInfo* info) {
	//

	// 撰寫進檔案時，以 Mutex鎖 鎖住IO資源，避免 Race Condition
	Monitor::Enter(writeLock);
	try
	{
		String^ filePath = "log.csv";
		StreamWriter^ sw = gcnew StreamWriter(filePath, true, System::Text::Encoding::UTF8);
		sw->WriteLine("RECOVER," + info->index + "," + info->DieX + "," + info->DieY + "," + info->LOT_ID + "," + DateTime::Now);
		sw->Close();
	}
	finally
	{
		Monitor::Exit(writeLock);
	}
}

void LogManager::doneUpdate(updateDieInfo* info) {
	//

	// 撰寫進檔案時，以 Mutex鎖 鎖住IO資源，避免 Race Condition
	Monitor::Enter(writeLock);
	try
	{
		String^ filePath = "log.csv";
		// 將更新資訊寫入 log.csv檔案內
		// 這些寫入資料 承諾 會更新回資料庫，若沒有對應回傳的話，代表更新失敗
		StreamWriter^ sw = gcnew StreamWriter(filePath, true, System::Text::Encoding::UTF8);
		sw->WriteLine("DONE," + info->index + "," + info->DieX + "," + info->DieY + "," + info->LOT_ID + "," + DateTime::Now);
		sw->Close();
	}
	finally
	{
		Monitor::Exit(writeLock);
	}
}

void LogManager::checkSave() {
	//

	// 撰寫進檔案時，以 Mutex鎖 鎖住IO資源，避免 Race Condition
	Monitor::Enter(writeLock);
	try
	{
		String^ filePath = "log.csv";
		StreamWriter^ sw = gcnew StreamWriter(filePath, false, System::Text::Encoding::UTF8);

		sw->WriteLine("SAVE,-1,-1,-1,-1," + DateTime::Now);
		sw->Close();
	}
	finally
	{
		Monitor::Exit(writeLock);
	}
}

//void LogManager::writeLog(String^ content) {
//	// 撰寫進檔案時，以 Mutex鎖 鎖住IO資源，避免 Race Condition
//	Monitor::Enter(writeLock);
//	try
//	{
//		String^ filePath = "log.csv";
//		StreamWriter^ sw = gcnew StreamWriter(filePath, true, System::Text::Encoding::UTF8);
//		sw->WriteLine(content);
//		sw->Close();
//	}
//	finally
//	{
//		Monitor::Exit(writeLock);
//	}
//}