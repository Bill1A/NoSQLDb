/////////////////////////////////////////////////////////////////////
// DbCore.cpp - Test stub for Dbcore.h and PayLoad.h               //
// ver 1.0                                                         //
// Biao A, CSE687 - Object Oriented Design, Spring 2018            //
/////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* - Test the function of Dbcore.h and PayLoad.h
*
* Required Files:
* ---------------
* DbCore.h, PayLoad.h
* StringUtilities.h
* TestUtilities.h
* Maintenance History:
* --------------------
* ver 1.0 : 2 Feb 2018
*/

#ifdef TEST_DBCORE
#include "DbCore.h"
#include <iostream>
#include <iomanip>
#include <functional>
#include "PayLoad.h"

using namespace NoSqlDb;

//----< test stub >---------------------------------------------------

int main()
{
	DbCore<PayLoad> db;
	PayLoad payL;
	payL.path("demoPath");
	payL.category().push_back("demo1");
	payL.category().push_back("demo2");
	DbElement<PayLoad> demoElem;
	demoElem.name("File2");
	demoElem.descrip("an important file");
	demoElem.dateTime(DateTime().now());
	demoElem.payLoad(payL);
	db["F2"] = demoElem;
	if (!db.contains("F2"))
		return 1;
	DbElement<PayLoad> demoElem1;
	demoElem1.name("File3");
	demoElem1.descrip("an interesting file");
	demoElem1.dateTime(DateTime().now());
	demoElem1.payLoad(payL);
	db["F3"] = demoElem1;
	if (db.size() != 3)
		return 1;
	std::cout << "\n  after adding elements with keys: ";
	DbCore<PayLoad>::Keys keys = db.keys();
	showKeys(db);
	std::cout << "\n  make all the new elements children of element with key \"F1\"";
	db.addChild("F1", "F2");
	db.addChild("F1", "F3");
	std::cout << "\n  showing all the database elements:";
	showDb(db);
	std::cout << "\n  database keys are: ";
	showKeys(db);
	return 0;
}
#endif
