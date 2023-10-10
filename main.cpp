
#include <stdlib.h>
#include <iostream>

using namespace std;


#include "truemysql.h"

const std::string HOST = "localhost";
const std::string DB = "sample-db";
const std::string USER = "root";
const std::string PWD = "password";

int main(int argc, char** argv) {

	cout << "Starting.." << endl;


	TrueMySql* mysql = new TrueMySql(HOST, "3306", DB, USER, PWD);
	mysql->getConnection();
	if (!mysql->isConnected()) {
		cout << "Could not connect to mysql:" << mysql->getError() << endl;
		return EXIT_FAILURE;
	}

	sql::ResultSet* rst = mysql->getResultSet("Select * from Graph");
	if (rst == NULL) {
		cout << "Error :" << mysql->getError() << endl;
	}
	else {
		while (rst->next()) {
			cout << "ID=" << rst->getInt("id") << "," << rst->getString("name") << endl;
		}
		delete rst;
	}
	vector<TrueParam> vect;
	TrueParam parm1;
	parm1.setIntValue(120);
	parm1.setType(TRUE_PARAM_INT);
	vect.push_back(parm1);

	TrueParam parm2;
	parm2.setIntValue(140);
	parm2.setType(TRUE_PARAM_INT);
	vect.push_back(parm2);

	rst = mysql->getParamResultSet("select * from Graph where id >  ? and id < ? limit 0,5", vect);
	if (rst == NULL) {
		cout << "Error :" << mysql->getError() << endl;
	}
	else {
		cout << "Search result:" << endl;
		size_t count = rst->rowsCount();
		while (rst->next()) {
			std::string name = rst->getString("name");
			cout << "ID=" << rst->getInt("id") <<   "," << rst->getString("name") << endl;
		}
		delete rst;
	}

	if (mysql->doQuery("insert into Bookmarks (person_id, graph_id, date_added, instance_id) values (1,120,now(), 1)")) {
		cout << "Executed insert query" << endl;
	}
	else {
		cout << "Error while inserting: " << mysql->getError() << endl;
	};

	vect.clear();
	TrueParam pPerson;
	pPerson.setIntValue(2);
	pPerson.setType(TRUE_PARAM_INT);

	TrueParam pGraph;
	pGraph.setIntValue(121);
	pGraph.setType(TRUE_PARAM_INT);

	TrueParam pDate;
	pDate.setType(TRUE_PARAM_STRING);
	pDate.setStringValue("2023-09-22 00:00:00");

	TrueParam pInstance;
	pInstance.setIntValue(1);
	pInstance.setType(TRUE_PARAM_INT);
	vect.push_back(pPerson);
	vect.push_back(pGraph);
	vect.push_back(pDate);
	vect.push_back(pInstance);
	if (mysql->doParamQuery("insert into Bookmarks (person_id, graph_id, date_added, instance_id) values (?,?,?, ?)", vect)) {
		cout << "Executed insert query with params" << endl;
	}
	else {
		cout << "Error while inserting with params: " << mysql->getError() << endl;
	}

	bool isError = false;
	mysql->startTxn();
	if (mysql->doQuery("insert into Bookmarks (person_id, graph_id, date_added, instance_id) values (3,3,now(), 3)")) {
		cout << "Executed txn insert query" << endl;
	}
	else {
		cout << "Error in txn while inserting: " << mysql->getError() << endl;
		isError = true;
	};
	if (mysql->doQuery("insert into Bookmarks (person_id, graph_id, date_added, instance_id) values (4, 4, now(), 4)")) {
		cout << "Executed txn insert query" << endl;
	}
	else {
		cout << "Error in txn while inserting: " << mysql->getError() << endl;
		isError = true;
	};
	if (isError) {
		mysql->rollbackTxn();
		cout << "Txn Rollback" << endl;
	}
	else {
		mysql->commitTxn();
		cout << "Txn commit" << endl;
	}

	mysql->closeConnection();

	cout << "Ending" << endl;

	return EXIT_SUCCESS;

}

