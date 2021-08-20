// SqlCommunicator.cpp
#include "SqlCommunicator.h"

//---------------------------------------------------------------------

SqlCommunicator::SqlCommunicator(const wchar_t* sqlConnectStr) {
	// constructor
	// sqlConnectStr is the connectString to the database

	try {
		// Iinitializations
		sqlConnHandle = NULL;
		sqlStmtHandle = NULL;

		// Allocations
		retCode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &sqlEnvHandle);
		if (retCode != SQL_SUCCESS && retCode != SQL_SUCCESS_WITH_INFO) {
			close();
			throw gcnew System::Exception(gcnew System::String(status(retCode).c_str()));
			return;
		}

		retCode = SQLSetEnvAttr(sqlEnvHandle, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0);
		if (retCode != SQL_SUCCESS && retCode != SQL_SUCCESS_WITH_INFO) {
			close();
			throw gcnew System::Exception(gcnew System::String(status(retCode).c_str()));
			return;
		}

		retCode = SQLAllocHandle(SQL_HANDLE_DBC, sqlEnvHandle, &sqlConnHandle);
		if (retCode != SQL_SUCCESS && retCode != SQL_SUCCESS_WITH_INFO) {
			close();
			throw gcnew System::Exception(gcnew System::String(status(retCode).c_str()));
			return;
		}

		// connect to SQL Server  
		// I am using a trusted connection and port 14808
		// it does not matter if you are using default or named instance
		// just make sure you define the server name and the port
		// You have the option to use a username/password instead of a trusted connection
		// but is more secure to use a trusted connection
		retCode = SQLDriverConnect(sqlConnHandle,
			NULL,
			//(SQLWCHAR*)L"DRIVER={SQL Server};SERVER=localhost, 1433;DATABASE=master;UID=username;PWD=password;",
			(SQLWCHAR*)sqlConnectStr,
			SQL_NTS,
			retconstring,
			1024,
			NULL,
			SQL_DRIVER_NOPROMPT);
		if (retCode != SQL_SUCCESS && retCode != SQL_SUCCESS_WITH_INFO) {
			close();
			throw gcnew System::Exception(gcnew System::String(status(retCode).c_str()));
			return;
		}

		// check if there is a problem connecting then exit application
		retCode = SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle);
		if (retCode != SQL_SUCCESS && retCode != SQL_SUCCESS_WITH_INFO) {
			close();
			throw gcnew System::Exception(gcnew System::String(status(retCode).c_str()));
			return;
		}
	}
	catch (System::Exception^ e) {
		throw gcnew System::Exception("SqlCommunicator.SqlCommunicator : " + e->Message);
	}


}

//---------------------------------------------------------------------

SQLHSTMT SqlCommunicator::sqlCommand(const wchar_t* sqlQueryStr) {
	// return the statementText of the sqlQueryStr
	// return NULL if the query is failed
	// this function only return the SQLHSTAT, not the real data

	try {
		retCode = SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)sqlQueryStr, SQL_NTS);
		if (retCode != SQL_SUCCESS && retCode != SQL_SUCCESS_WITH_INFO) {
			// "Error querying SQL Server"
			close();
			throw gcnew System::Exception(gcnew System::String(status(retCode).c_str()));
		}

		return sqlStmtHandle;
	}
	catch (System::Exception^ e) {
		throw gcnew System::Exception("SqlCommunicator.sqlQuery : " + e->Message);
	}
}

int SqlCommunicator::get_statistics_data(const int& cur_level, int* locat_data, char* filter_code)
{
	stringstream ss;//儲存轉譯後的容器
	int start_x, end_x, start_y, end_y;
	try {
		if (locat_data != nullptr) {
			switch (cur_level) {
			case 0:
				start_x = locat_data[0] * 1000 - 1;
				start_y = locat_data[3] * 1000 - 1;
				end_x = (locat_data[0] + 1) * 1000 + 1;
				end_y = (locat_data[3] + 1) * 1000 + 1;
				break;
			case 1:
				start_x = locat_data[0] * 1000 + locat_data[1] * 100 - 1;
				start_y = locat_data[3] * 1000 + locat_data[4] * 100 - 1;
				end_x = locat_data[0] * 1000 + (locat_data[1] + 1) * 100 + 1;
				end_y = locat_data[3] * 1000 + (locat_data[4] + 1) * 100 + 1;
				break;
			case 2:
				start_x = locat_data[0] * 1000 + locat_data[1] * 100 + locat_data[2] * 10 - 1;
				start_y = locat_data[3] * 1000 + locat_data[4] * 100 + locat_data[5] * 10 - 1;
				end_x = locat_data[0] * 1000 + locat_data[1] * 100 + (locat_data[2] + 1) * 10 + 1;
				end_y = locat_data[3] * 1000 + locat_data[4] * 100 + (locat_data[5] + 1) * 10 + 1;
			}
		}
		else
			throw gcnew System::Exception(gcnew System::String("location data array lose"));
	}
	catch (System::Exception^ e) {
		throw gcnew System::Exception("Warning : " + e->Message);
		return 0;
	}
	switch (filter_code[0]) {
	case 0://全部的normal die
		break;
	case 1://全部的defect die
		ss << "SELECT COUNT (*) FROM [test].[dbo].[2274_DefectData_TEST_PartALL] WHERE [DieX] > " << start_x << "And [DieX] < " << end_x << "And [DieY] > " << start_y << "And [DieY] < " << end_y;//這之後有bin code時要改
		break;
	case 2:
		break;
	case 3:
		break;
	}
	// 將 string 轉換為 wstring, 以供 SqlCommuncator 使用
	// convert string -> wstring
	string sqlCommand = ss.str();
	// 呼叫 sqlCommand() 將命令 上傳 SQL Server, 上傳結束後關閉連線
	// 若 SQL執行失敗, 會直接進入 catch 區域, 並顯示錯誤原因
	// send the search command to SqlCommunicator
	wstring searchSqlCommand(sqlCommand.begin(), sqlCommand.end());
	SQLHSTMT hstmt = this->sqlCommand(searchSqlCommand.c_str());
	// 依 SELECT 命令 放入變數空間
	SQLBIGINT Total;
	SQLLEN cbTotal;
	// seperate each column from HSTMT
	SQLBindCol(hstmt, 1, SQL_C_SBIGINT, &Total, 0, &cbTotal);
	// 獲得命令回傳
	// the return state of ODBC command
	SQLRETURN retCode = SQLFetch(hstmt);
	try {
		if (retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
			return Total;
		else
			throw gcnew System::Exception(gcnew System::String("SQL featching fail"));
	}
	catch (System::Exception^ e){
		throw gcnew System::Exception("Warning : " + e->Message);
		return 0;
	}
}

void SqlCommunicator::close() {
	// close the connection to database
	// and release the resource holded before

	SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
	SQLDisconnect(sqlConnHandle);
	SQLFreeHandle(SQL_HANDLE_DBC, sqlConnHandle);
	SQLFreeHandle(SQL_HANDLE_ENV, sqlEnvHandle);

	cout << "Close connection\n";
}

//---------------------------------------------------------------------

string SqlCommunicator::status(SQLRETURN retCode)
{
	// print the represented status of retCode
	// https://docs.microsoft.com/zh-tw/sql/odbc/reference/develop-app/return-codes-odbc?view=sql-server-ver15

	switch (retCode)
	{
	case SQL_SUCCESS:
		return "SQL_SUCCESS";
		break;
	case SQL_SUCCESS_WITH_INFO:
		return "SQL_SUCCESS_WITH_INFO";
		break;
	case SQL_NEED_DATA:
		return "SQL_NEED_DATA";
		break;
	case SQL_STILL_EXECUTING:
		return "SQL_STILL_EXECUTING";
		break;
	case SQL_ERROR:
		return "SQL_ERROR";
		break;
	case SQL_NO_DATA:
		return "SQL_NO_DATA";
		break;
	case SQL_INVALID_HANDLE:
		return "SQL_INVALID_HANDLE";
		break;
	case SQL_PARAM_DATA_AVAILABLE:
		return "SQL_PARAM_DATA_AVAILABLE";
		break;
	}
}

//---------------------------------------------------------------------