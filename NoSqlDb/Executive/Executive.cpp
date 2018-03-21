/////////////////////////////////////////////////////////////////////
// Executive.cpp - executive file to demo all the requirments      //
// ver 1.0                                                         //
// Biao A, CSE687 - Object Oriented Design, Spring 2018            //
/////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* - Demo all the requirments and show the result
*
* Required Files:
* ---------------
* DbCore.h  Test.h
* TestUtilities.h
*
* Maintenance History:
* --------------------
* ver 1.0 : 2 Feb 2018
*/

#include <iostream>
#include <string>
#include "../Test/Test.h"
#include "../Utilities/TestUtilities/TestUtilities.h"


//----< requeirment 1 - 5 >------
void testHelper1(TestExecutive& ex, Test test) {
	TestExecutive::TestStr ts1{ std::bind(&Test::testR1,test), "Use C++11" };
	TestExecutive::TestStr ts2{ std::bind(&Test::testR2,test), "Use streams and new and delete" };
	TestExecutive::TestStr ts3a{ std::bind(&Test::testR3a,test), "Create DbElement<PayLoad>" };
	TestExecutive::TestStr ts3b{ std::bind(&Test::testR3b,test), "Create DbCore<PayLoad>" };
	TestExecutive::TestStr ts4{ std::bind(&Test::testR4,test), "Add and delete key/value pairs" };
	TestExecutive::TestStr ts5{ std::bind(&Test::testR5,test), "Edit and replace" };
	ex.registerTest(ts1);
	ex.registerTest(ts2);
	ex.registerTest(ts3a);
	ex.registerTest(ts3b);
	ex.registerTest(ts4);
	ex.registerTest(ts5);
}

//----< requeirment 6 - 9 >------
void testHelper2(TestExecutive& ex, Test test) {
	TestExecutive::TestStr ts6a{ std::bind(&Test::testR6a,test), "Query for the value of a specified key" };
	TestExecutive::TestStr ts6b{ std::bind(&Test::testR6b,test), "Query for the children of a specified key" };
	TestExecutive::TestStr ts6c{ std::bind(&Test::testR6c,test), "Query for a specified regular-expression pattern" };
	TestExecutive::TestStr ts6d{ std::bind(&Test::testR6d,test), "Query for a time interval" };
	TestExecutive::TestStr ts7{ std::bind(&Test::testR7,test), "\"and\" and \"or\"" };
	TestExecutive::TestStr ts8a{ std::bind(&Test::testR8a,test), "Creating XML representation of Database using XmlDocument" };
	TestExecutive::TestStr ts8b{ std::bind(&Test::testR8b,test), "Building a database from XmlDocument" };
	TestExecutive::TestStr ts9{ std::bind(&Test::testR9,test), "Show the PayLoad Class" };
	ex.registerTest(ts6a);
	ex.registerTest(ts6b);
	ex.registerTest(ts6c);
	ex.registerTest(ts6d);
	ex.registerTest(ts7);
	ex.registerTest(ts8a);
	ex.registerTest(ts8b);
	ex.registerTest(ts9);
}

int main()
{
	TestExecutive ex;
	Test test;
	testHelper1(ex, test);
	testHelper2(ex, test);
	bool result = ex.doTests();
	if (result == true)
		std::cout << "\n  all tests passed";
	else
		std::cout << "\n  at least one test failed";
	getchar();
	getchar();
	return 0;
}

