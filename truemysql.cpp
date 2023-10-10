#include "truemysql.h"

/**
 * Constructor. All args are reqd
 * @param all connection values as string
 * @return None
 */
TrueMySql::TrueMySql(std::string host, std::string port, std::string db, std::string user, std::string pwd) {
	  mHost = host; mPort = port; mDB = db; mUser = user; mPwd = pwd;

}

/**
 * Get a Connection. If already created then connection object else create new one
 * @param None
 * @return Connection object
 */
sql::Connection* TrueMySql::getConnection() {
	if (!mIsConnected) {
		try {
			mDriver = get_driver_instance();
			std::string connStr = "tcp://" + mHost + ":" + mPort;
			mConn = mDriver->connect(connStr, mUser, mPwd);
			mConn->setSchema(mDB);
			mIsConnected = true;
			return mConn;
		}
		catch (sql::InvalidArgumentException& ex) {
			mError = "";
			mError.append("Error: ").append(ex.what()).append(", Code: ").append(std::to_string(ex.getErrorCode()));
			return NULL;
		}
	}
	else
		return mConn;
}

/**
 * Get a ResultSet on execution of a query. This is a plain string query. No parametrized values used here
 * @param sql string
 * @return Resultset object
 */
sql::ResultSet* TrueMySql::getResultSet(std::string sql) {
	if (mIsConnected) {
		try {
			sql::Statement* stmt = mConn->createStatement();
			sql::ResultSet* rst = stmt->executeQuery(sql);
			delete stmt;
			return rst;
		}
		catch (sql::SQLException& ex) {
			mError = "";
			mError.append("Error: ").append(ex.what()).append(", Code: ").append(std::to_string(ex.getErrorCode()));
			return NULL;
		}

	}
	else {
		mError = "Database connection is not active";
		return NULL;
	}
}

/**
 * Execute a query which does not return anything. This is a plain string query. No parametrized values used here
 * @param sql string
 * @return true if successful else false
 */
bool TrueMySql::doQuery(std::string sql) {
	bool retVal = true;
	if (mIsConnected) {
		try {
			sql::Statement* stmt = mConn->createStatement();
			stmt->execute(sql);
			delete stmt;
			return true;
		}
		catch (sql::SQLException& ex) {
			mError = "";
			mError.append("Error: ").append(ex.what()).append(", Code: ").append(std::to_string(ex.getErrorCode()));
			return false;
		}

	}
	else {
		mError = "Database connection is not active";
		return false;
	}
}

/**
 * Get a ResultSet on execution of a query. Uses parametrized values with Prepared Statement, hence injection-proof
 * @param sql string, vector of parameter values
 * @return Resultset object
 */
sql::ResultSet* TrueMySql::getParamResultSet(std::string sql, std::vector<TrueParam> vParams) {
	if (mIsConnected) {
		try {
			sql::PreparedStatement* stmt = mConn->prepareStatement(sql);
			for (int i = 0; i < vParams.size(); i++) {
				TrueParam p = vParams.at(i);
				if (p.getType() == TRUE_PARAM_INT) {
					stmt->setInt(i+1, p.getIntValue());
				}
				else if (p.getType() == TRUE_PARAM_STRING) {
					stmt->setString(i+1, p.getStringValue());
				}
			}

			sql::ResultSet* rst = stmt->executeQuery();
			delete stmt;
			return rst;
		}
		catch (sql::SQLException& ex) {
			mError = "";
			mError.append("Error: ").append(ex.what()).append(", Code: ").append(std::to_string(ex.getErrorCode()));
			return NULL;
		}

	}
	else {
		mError = "Database connection is not active";
		return NULL;
	}

}

/**
 * Execute a query which does not return anything. Uses parametrized values with Prepared Statement. Hence injection-proof
 * @param sql string
 * @return true if successful else false
 */
bool TrueMySql::doParamQuery(std::string sql, std::vector<TrueParam> vParams) {

		if (mIsConnected) {
			try {
				sql::PreparedStatement* stmt = mConn->prepareStatement(sql);
				for (int i = 0; i < vParams.size(); i++) {
					TrueParam p = vParams.at(i);
					if (p.getType() == TRUE_PARAM_INT) {
						stmt->setInt(i+1, p.getIntValue());
					}
					else if (p.getType() == TRUE_PARAM_STRING) {
						stmt->setString(i+1, p.getStringValue());
					}
				}

				stmt->execute();
				delete stmt;
				return true;
			}
			catch (sql::SQLException& ex) {
				mError = "";
				mError.append("Error: ").append(ex.what()).append(", Code: ").append(std::to_string(ex.getErrorCode()));
				return NULL;
			}

		}
		else {
			mError = "Database connection is not active";
			return false;
		}
}

/**
 * Sets transaction flag for subsequent queries
 * @param None
 * @return true if successful else false
 */
bool TrueMySql::startTxn() {
	return this->doQuery("start transaction");
}

/**
 * Rolls back current transaction
 * @param None
 * @return true if successful else false
 */
bool TrueMySql::rollbackTxn() {
	return this->doQuery("rollback");
}

/**
 * Commits current transaction
 * @param None
 * @return true if successful else false
 */
bool TrueMySql::commitTxn() {
	return this->doQuery("commit");
}

/**
 * Closes open connection object
 * @param None
 * @return true if successful else false
 */
bool TrueMySql::closeConnection() {
	bool retVal = true;

	if (mConn != NULL) {
		if (mIsConnected)
			mConn->close();
		delete mConn;
	}
	return retVal;
}
