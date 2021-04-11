// SqlCommunicator.cpp

#include <iostream>
#include <windows.h>
#include "SqlCommunicator.h"

//---------------------------------------------------------------------

SqlCommunicator::SqlCommunicator(const wchar_t* sqlConnectStr) {
    // constructor
    // sqlConnectStr is the connectString to the database

    // Iinitializations
    sqlConnHandle = NULL;
    sqlStmtHandle = NULL;

    // Allocations
    retCode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &sqlEnvHandle);
    if (retCode != SQL_SUCCESS && retCode != SQL_SUCCESS_WITH_INFO) {
        status(retCode);
        close();
        return;
    }

    retCode = SQLSetEnvAttr(sqlEnvHandle, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0);
    if (retCode != SQL_SUCCESS && retCode != SQL_SUCCESS_WITH_INFO) {
        status(retCode);
        close();
        return;
    }

    retCode = SQLAllocHandle(SQL_HANDLE_DBC, sqlEnvHandle, &sqlConnHandle);
    if (retCode != SQL_SUCCESS && retCode != SQL_SUCCESS_WITH_INFO) {
        status(retCode);
        close();
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
        status(retCode);
        close();
        return;
    }

    // check if there is a problem connecting then exit application
    retCode = SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle);
    if (retCode != SQL_SUCCESS && retCode != SQL_SUCCESS_WITH_INFO) {
        status(retCode);
        close();
        return;
    }
}

//---------------------------------------------------------------------

SQLHSTMT SqlCommunicator::sqlQuery(const wchar_t* sqlQueryStr) {
    // return the statementText of the sqlQueryStr
    // return NULL if the query is failed
    // this function only return the SQLHSTAT, not the real data

    retCode = SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)sqlQueryStr, SQL_NTS);
    if (retCode != SQL_SUCCESS && retCode != SQL_SUCCESS_WITH_INFO) {
        cout << "Error querying SQL Server, ";
        status(retCode);
        cout << "\n";

        close();
        return NULL;
    }

    return sqlStmtHandle;
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

void SqlCommunicator::status(SQLRETURN retCode)
{
    // print the represented status of retCode
    // https://docs.microsoft.com/zh-tw/sql/odbc/reference/develop-app/return-codes-odbc?view=sql-server-ver15

    switch (retCode)
    {
    case SQL_SUCCESS:
        cout << "SQL_SUCCESS\n";
        break;
    case SQL_SUCCESS_WITH_INFO:
        cout << "SQL_SUCCESS_WITH_INFO\n";
        break;
    case SQL_NEED_DATA:
        cout << "SQL_NEED_DATA\n";
        break;
    case SQL_STILL_EXECUTING:
        cout << "SQL_STILL_EXECUTING\n";
        break;
    case SQL_ERROR:
        cout << "SQL_ERROR\n";
        break;
    case SQL_NO_DATA:
        cout << "SQL_NO_DATA\n";
        break;
    case SQL_INVALID_HANDLE:
        cout << "SQL_INVALID_HANDLE\n";
        break;
    case SQL_PARAM_DATA_AVAILABLE:
        cout << "SQL_PARAM_DATA_AVAILABLE\n";
        break;
    }
}

//---------------------------------------------------------------------