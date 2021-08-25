#pragma once
#include "UpdateDieInfo.h"

#include <vector>

ref class LogManager
{
private:
	static Object^ writeLock = gcnew Object();
	// static String^ filePath = "log.csv";

public:
	// Constructor
	LogManager();

	void guaranteeToUpdate(vector<updateDieInfo*>);
	void recoveryToUpdate(updateDieInfo* info);
	void doneUpdate(updateDieInfo* info);
	void checkSave();

	void clear();
};

