/*******************************************************************************
* SqlCommunicator.h
* 以 ODBC 方式連接 SQL Server 2019
 *******************************************************************************/

#pragma once
#include <sql.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <string>

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

public:
    // constructor
    SqlCommunicator(const wchar_t* sqlConnectStr);
    // disconstructor : disconnect with server
    ~SqlCommunicator();

    // COMMUNICATION
    // return the statementText of the sqlQueryStr
    SQLHSTMT sqlQuery(const wchar_t* sqlQueryStr);
    // close the connection to database
    void close();

    // STATUS
    // print the represented status of retCode
    string status(SQLRETURN retCode);
};
