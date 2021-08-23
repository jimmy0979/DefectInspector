#pragma once
#include "LogManager.h"
#include "UpdateDieInfo.h"

ref class UpdateInBackground
{
private:
	static Object^ updateLock = gcnew Object();
	static Object^ obj = gcnew Object();

	static int txnIndex = 0;

	// 
	static LogManager^ logManager = gcnew LogManager();

	const wchar_t* connectStr;
	updateDieInfo* info;
	int index;
	int DieX, DieY, LOT_ID;

public:
	// Constructor
	UpdateInBackground(updateDieInfo* info);
	
	// Set
	void setConnectStr(const wchar_t* connectStr);

	// Get
	int get_Id() { return this->info->index; }
	int get_DieX() { return this->info->DieX; }
	int get_DieY() { return this->info->DieY; }
	int get_LotId() { return this->info->LOT_ID; }

	// Start Running in Background
	void running();
};