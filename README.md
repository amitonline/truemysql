# truemysql
C++ class to work with MySQL C++ connector

**OVERVIEW**  
The aim was to encapsulate all the basic mysql functions which an application would need to work with the MySql Connector for C++. 
The following functions are in place:
 - Opening and closing connections
 - Executing queries which do not return any data - with and without parameterized arguments
 - Executing queries which return a resultset - with and without parameterized arguments
 - Toggle transactions


**FILES**  
truemysql.h - header for includes  
truemysql.cpp - class implementation  
main.cpp - examples of how to use the above class  

**PRE-REQUISITES**  
For vc++ apps to work with MySql databases, it is assumed that MySql Connector for C++ is installed on the machine. Instructions to install it for Windows are given here:
https://truelogic.org/wordpress/2023/09/21/working-with-mysql-in-vc/

For Linux, the downloads can be found here: https://dev.mysql.com/downloads/connector/cpp/
Just download the tar and unzip contents into a folder. In your Project Config or Cmake File, you need to add the folders for includes and libs.

**SAMPLE CODE**  
```
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
mysql->closeConnection();

```
