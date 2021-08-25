#pragma once
#include <iostream>
using namespace std;

/*
 *	紀錄 儲存 UpdateDie 的資料結構
 *	任何會進行資料庫更新的物件，都應 Include 來統一資料結構
 */

typedef struct _updateDieInfo {
	// 歷史紀錄
	static int totIndex;

	// 更新晶粒資料
	int index = -1;
	int LOT_ID = 0;
	int DieX = 0, DieY = 0;
	//_updateDieInfo(int LOT_ID, int DieX, int DieY) :LOT_ID(LOT_ID), DieX(DieX), DieY(DieY) {
	//	//
	//	//this->index = totIndex++;
	//}
	_updateDieInfo(int LOT_ID, int DieX, int DieY, int index) :LOT_ID(LOT_ID), DieX(DieX), DieY(DieY), index(index) {}
} updateDieInfo;


//ref class UpdateDieInfo
//{
//private:
//	// 歷史紀錄
//	static int totIndex;
//
//	// 更新晶粒資料
//	int index = -1;
//	int LOT_ID = 0;
//	int DieX = 0, DieY = 0;
//
//public:
//
//	// 建構子 Constructors
//	UpdateDieInfo(int LOT_ID, int DieX, int DieY) :LOT_ID(LOT_ID), DieX(DieX), DieY(DieY) {
//		// 自動累積當前 Index，作為全域唯一
//		this->index = totIndex++;
//	}
//	UpdateDieInfo(int LOT_ID, int DieX, int DieY, int index) :LOT_ID(LOT_ID), DieX(DieX), DieY(DieY), index(index) {}
//};

