#pragma once
ref class UpdateInBackground
{
private:
	static Object^ updateLock = gcnew Object();
	static Object^ obj = gcnew Object();

	static int txnIndex = 0;

	const wchar_t* connectStr;
	int index;
	int DieX, DieY, LOT_ID;

public:
	// Constructor
	UpdateInBackground(int DieX, int DieY, int LOT_ID);
	
	// Set
	void setConnectStr(const wchar_t* connectStr);

	// Get
	int get_Id() { return this->index; }
	int get_LotId() { return this->LOT_ID; }
	int get_DieX() { return this->DieX; }
	int get_DieY() { return this->DieY; }

	// Start Running in Background
	void running();
};