#pragma once
#include <vector>

using namespace std;

#include <jdbc/mysql_connection.h>
#include <jdbc/cppconn/connection.h>
#include <jdbc/cppconn/driver.h>
#include <jdbc/cppconn/resultset.h>
#include <jdbc/cppconn/statement.h>
#include <jdbc/cppconn/prepared_statement.h>

//Prepared Statement param type constants
const int TRUE_PARAM_INT = 1;
const int TRUE_PARAM_STRING = 2;

//Class to store prepared statement parameter
class TrueParam {
public:
	virtual inline int getType() { return mType; }
	virtual void inline setIntValue(int val) { mIntValue = val; };
	virtual inline void setType(int t) { mType = t; };
	virtual inline int getIntValue() { return mIntValue; }
	virtual void inline setStringValue(std::string val) { mStringValue = val; };
	virtual inline std::string getStringValue() { return mStringValue; }

protected:
	int mType;
	int mIntValue;
	std::string mStringValue;
};


/////////////////////////////////////////////////////////////////////////

//Main MySQL db handling class
class TrueMySql {
private:
	std::string mHost;
	std::string mPort;
	std::string mDB;
	std::string mUser;
	std::string mPwd;

	sql::Driver* mDriver;
	sql::Connection* mConn;

	bool mIsConnected;
	std::string mError;


public:
	TrueMySql(std::string host, std::string port, std::string db, std::string user, std::string pwd);
	inline std::string getError() { return mError; }
 	sql::Connection* getConnection();
	inline bool isConnected() { return mIsConnected; }
	sql::ResultSet* getResultSet(std::string sql);
	sql::ResultSet* getParamResultSet(std::string sql, std::vector<TrueParam> vParams);
	bool doQuery(std::string sql);
	bool doParamQuery(std::string sql, std::vector<TrueParam> vParams);
	bool startTxn();
	bool commitTxn();
	bool rollbackTxn();
	bool closeConnection();
};
