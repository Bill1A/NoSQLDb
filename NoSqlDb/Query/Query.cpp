/////////////////////////////////////////////////////////////////////
// Query.cpp  - Test stub for Query.h                              //
// ver 1.0                                                         //
// Biao A CSE 687                                                  //
/////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* - Test the function of Query.h
*
* Required Files:
* ---------------
* DbCore.h, DateTime.h
* Query.h
*
* Maintenance History:
* --------------------
* ver 1.0 : 2 Feb 2018
*/

#ifdef TEST_QUERY
#include "Query.h"
#include <iostream>
#include "../DbCore/DbCore.h"
using namespace NoSqlDb;
int main()
{
	DbElement<std::string> demoElem1;
	demoElem1.name("AAA");
	demoElem1.descrip("Instructor for CSE687");
	DateTime time("1 Jan 5 5 5 5 1995");
	demoElem1.dateTime(time);
	demoElem1.payLoad("The good news is ...");
	DbElement<std::string> demoElem2;
	demoElem2.name("AAAA");
	demoElem2.descrip("Instructor for CSE687");
	demoElem2.dateTime(DateTime().now());
	demoElem2.payLoad("The good news is ...");
	DbElement<std::string> demoElem3;
	demoElem3.name("AAAAA");
	demoElem3.descrip("Student");
	demoElem3.dateTime(DateTime().now());
	demoElem3.payLoad("The good news is ...");
	DbElement<std::string> demoElem4;
	demoElem4.name("DDD");
	demoElem4.descrip("Instructor for CSE687");
	demoElem4.dateTime(DateTime().now());
	demoElem4.payLoad("The good news is ...");
	DbCore<std::string> db;
	db["A"] = demoElem1;
	db["B"] = demoElem2;
	db["BBB"] = demoElem3;
	db["D"] = demoElem4;
	db["A"].children().push_back("B");
	db["A"].children().push_back("C");
	db["A"].children().push_back("D");
	Query<std::string> q1(db);
	Query<std::string> q2(db);
	q1.show();
	showDb(db);
	Condition a;
	DateTime begin("1 Jan 5 5 5 5 1994");
	DateTime end("1 Jan 5 5 5 5 1997");
	a.timeInterval(begin, end);
	q1.select(a).show();
    return 0;
}
#endif

