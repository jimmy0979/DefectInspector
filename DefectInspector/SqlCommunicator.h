/*******************************************************************************
* SqlCommunicator.h
* 以 ODBC 方式連接 SQL Server 2019
 *******************************************************************************/

#pragma once
#include <sql.h>
#include <sqlext.h>
#include <sqltypes.h>

#include <string>
#include <sstream>
using namespace std;

#define SQL_RESULT_LEN 240
#define SQL_RETURN_CODE_LEN 1000

class SqlCommunicator {
private:
	// PROPERTIES
	// define handles and variables
	SQLHANDLE sqlConnHandle;
	SQLHANDLE sqlStmtHandle;
	SQLHANDLE sqlEnvHandle;
	SQLWCHAR retconstring[SQL_RETURN_CODE_LEN];

	// the return state of each ODBC command
	SQLRETURN retCode;

	// the flag whether (this)sqlCommunicator is connect to a database successfully
	bool isConnect;

public:
	// constructor
	SqlCommunicator(const wchar_t* sqlConnectStr);
	// disconstructor : disconnect with server
	~SqlCommunicator();

	// COMMUNICATION
	// return the statementText of the sqlQueryStr
	SQLHSTMT sqlCommand(const wchar_t* sqlQueryStr);
	// 讀取指定的統計資料，在特定的範圍中(暫時寫好放著)
	int get_statistics_data(const int& cur_level, int* locat_data, char* filter_code);
	// setup the info of columns from HSTMT
	void sqlBindCol(int column);
	// close the connection to database
	void close();

	// STATUS
	// print the represented status of retCode
	string status(SQLRETURN retCode);
};
