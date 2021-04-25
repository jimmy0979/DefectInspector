// SqlCommunicator.cpp

#include <iostream>
#include <windows.h>
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