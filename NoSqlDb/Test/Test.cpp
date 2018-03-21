/////////////////////////////////////////////////////////////////////
// Test.cpp  - Implements methods in Test class                    //
// ver 1.0                                                         //
// Biao A CSE 687                                                  //
/////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* Class Provider:
* - Provides mechanism to share a test database and a xml string between test functions
*   without explicitly passing as a function argument
* 
* Funciton main:
* - Test stub for Test class
*
* Required Files:
* ---------------
* DbCore.h         StringUtilities.h
* TestUtilities.h  XmlDocument.h
* Query.h          PayLoad.h
* DateTime.h
*
* Maintenance History:
* --------------------
* ver 1.0 : 2 Feb 2018
*/

#include "Test.h"
#include "../DbCore/DbCore.h"
#include "../Utilities/StringUtilities/StringUtilities.h"
#include "../Utilities/TestUtilities/TestUtilities.h"
#include "../XmlDocument/XmlDocument.h"
#include "../XmlElement/XmlElement.h"
#include "../Query/Query.h"
#include "../DbCore/PayLoad.h"
#include <iostream>
#include <iomanip>
#include <functional>

using namespace NoSqlDb;
using namespace XmlProcessing;
//----< reduce the number of characters to type >----------------------

auto putLine = [](size_t n = 1, std::ostream& out = std::cout)
{
	Utilities::putline(n, out);
};

///////////////////////////////////////////////////////////////////////
// Provider class
// - provides mechanism to share a test database and a xml string between test functions
//   without explicitly passing as a function argument.

class Provider
{
public:
	DbCore<PayLoad>& db() { return db_; }
	std::string& xml() { return xml_; }
private:
	static DbCore<PayLoad> db_;
	static std::string xml_;
};
DbCore<PayLoad> Provider::db_;
std::string Provider::xml_;



///////////////////////////////////////////////////////////////////////
// Test methods

//----< demo requirement #1 >------------------------------------------
bool Test::testR1()
{
	Utilities::title("Demonstrating Requirement #1");
	std::cout << "\n  " << typeid(std::function<bool()>).name()
		<< ", declared in this function, "
		<< "\n  is only valid for C++11 and later versions.";
	putLine();
	return true; // would not compile unless C++11
}

//----< demo requirement #2 >------------------------------------------
bool Test::testR2() {
	Utilities::title("Demonstrating Requirement #2");
	std::cout << "\n  A visual examination of all the submitted code "
		<< "will show only\n  use of streams and operators new and delete.";
	putLine();
	return true;
}

//----< demo first part of requirement #3 >----------------------------
bool Test::testR3a() {
	Utilities::title("Demonstrating Requirement #3a - creating DbElement");
	std::cout << "\n  Creating a db element with key \"F1\":";

	// create database to hold std::string payload

	DbCore<PayLoad> db;
	Provider dbp;
	dbp.db() = db;

	// create some demo elements and insert into db
	PayLoad payL;
	payL.path("demoPath");
	payL.category().push_back("demo1");
	payL.category().push_back("demo2");
	
	DbElement<PayLoad> demoElem;

	demoElem.name("File1");
	demoElem.descrip("an important file");
	demoElem.dateTime(DateTime().now());
	demoElem.payLoad(payL);

	if (demoElem.name() != "File1")
		return false;
	if (demoElem.descrip() != "an important file")
		return false;
	if (demoElem.dateTime().now() != DateTime().now())
		return false;
	if (demoElem.payLoad().path() != payL.path())
		return false;

	showHeader();
	showElem(demoElem);

	db["F1"] = demoElem;
	dbp.db() = db;
	putLine();
	return true;
}

//----< demo second part of requirement #3 >---------------------------
bool Test::testR3b() {
	Utilities::title("Demonstrating Requirement #3b - creating DbCore");
	Provider dbp;
	DbCore<PayLoad> db = dbp.db();
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
		return false;
	DbElement<PayLoad> demoElem1;
	demoElem1.name("File3");
	demoElem1.descrip("an interesting file");
	demoElem1.dateTime(DateTime().now());
	demoElem1.payLoad(payL);
	db["F3"] = demoElem1;
	if (db.size() != 3)
		return false;
	std::cout << "\n  after adding elements with keys: ";
	DbCore<PayLoad>::Keys keys = db.keys();
	showKeys(db);
	putLine();
	std::cout << "\n  make all the new elements children of element with key \"F1\"";
	db.addChild("F1","F2");
	db.addChild("F1", "F3");
	putLine();
	std::cout << "\n  showing all the database elements:";
	showDb(db);
	putLine();
	std::cout << "\n  database keys are: ";
	showKeys(db);
	dbp.db() = db;
	putLine();
	return true;
}

//----< demo requirement #4 >---------------------------
bool Test::testR4() {
	Utilities::title("Demonstrating Requirement #4 - add and delete key/value pairs");
	Provider dbp;
	DbCore<PayLoad> db = dbp.db();
	DbElement<PayLoad> demoElem1;

	PayLoad payL;
	payL.path("demoPath");
	payL.category().push_back("demo1");
	payL.category().push_back("demo2");

	demoElem1.name("File4");
	demoElem1.descrip("a useless file");
	demoElem1.dateTime(DateTime().now());
	demoElem1.payLoad(payL);
	db.addEle("F4", demoElem1);
	putLine();
	std::cout << "\n  Adding File4. ";
	showDb(db);

	putLine();
	std::cout << "\n  Delete File4. ";
	db.deleteEle("F4");
	showDb(db);
	putLine();
	return true;
}

//----< demo requirement #5  >---------------------------
bool Test::testR5() {
	Utilities::title("Demonstrating Requirement #5 - edit and replace");
	Provider dbp;
	DbCore<PayLoad> db = dbp.db();
	PayLoad payL;
	payL.path("demoPath");
	payL.category().push_back("demo1");
	payL.category().push_back("demo2");
	DbElement<PayLoad> demoElem1;
	demoElem1.name("File4");
	demoElem1.descrip("a useless file");
	DateTime a("Thu Jan 4 3 2 1 1999");
	demoElem1.dateTime(a);
	demoElem1.payLoad(payL);
	db.addEle("F4",demoElem1);
	putLine();
	std::cout << "  Add F4 to F1's children";
	db.addChild("F1", "F4");
	showDb(db);
	db.delChild("F1", "F4");
	putLine();
	std::cout << "  Delete F4 from F1's children";
	showDb(db);
	putLine();
	std::cout << "\n  Editing F1'description to : an very important file";
	db.edit("F1","description", "an very important file");
	showDb(db);

	DbElement<PayLoad> demoElem5;
	demoElem5.name("File2");
	demoElem5.descrip("new element!");
	demoElem5.dateTime(DateTime().now());
	payL.path("New demo!");
	demoElem5.payLoad(payL);
	putLine();
	std::cout << "\n  Replace File2 with a new element";
	db.replace("F2", demoElem5);
	showDb(db);
	putLine();
	dbp.db() = db;
	return true;
}

//----< demo 1st part of requirement #6 >---------------------------
bool Test::testR6a() {
	Utilities::title("Demonstrating Requirement 1st of #6 - query for the value of a specified key");
	Provider dbp;
	DbCore<PayLoad> db = dbp.db();
	showDb(db);
	Query<PayLoad> q1(db);
	putLine();
	std::cout << "\n  Looking for the key: F1.";
	q1.selectOneKey("F1");
	putLine();
	return true;
}

//----< demo 2nd part of requirement #6 >---------------------------
bool Test::testR6b() {
	Utilities::title("Demonstrating Requirement 2nd of #6 - query for the children of a specified key.");

	Provider dbp;
	DbCore<PayLoad> db = dbp.db();

	showHeader();
	showElem(db["F1"]);
	std::cout << "\n  Looking for the children of this key: F1.";
	Query<PayLoad> q1(db);
	q1.selectChildren("F1").show();

	dbp.db() = db;


	return true;
}

//----< demo 3rd part of requirement #6 >---------------------------
bool Test::testR6c() {
	Utilities::title("Demonstrating Requirement 3rd of #6 - The set of all keys matching a specified regular-expression pattern."); 
	Provider dbp;
	DbCore<PayLoad> db = dbp.db();
	showDb(db);
	Query<PayLoad> q1(db);
	Condition c1;
	c1.name("F+ile1");
	std::cout << "\n  Query for regular-expression \"F+ile1\" for name.";
	q1.select(c1).show();
	putLine();
	return true;
}

//----< demo 4th part of requirement #6 >---------------------------
bool Test::testR6d() {
	Utilities::title("Demonstrating Requirement 4th of #6 - Query for a time interval.");
	Provider dbp;
	DbCore<PayLoad> db = dbp.db();
	showDb(db);
	DateTime begin("1 Jan 5 5 5 5 1994");
	DateTime end("1 Jan 5 5 5 5 2000");
	Condition c;
	c.timeInterval(begin, end);
	Query<PayLoad> q1(db);
	std::cout << "\n  Query for time interval between Jan 5 1994 and Jan 5 2000";
	q1.select(c);
	q1.show();
	return true;
}

//----< demo requirement #7 >---------------------------
bool Test::testR7() {
	Utilities::title("Demonstrating Requirement #7 - \"and\" and \"or\"");
	Provider dbp;
	DbCore<PayLoad> db = dbp.db();
	showDb(db);
	putLine();
	Query<PayLoad> q1(db);
	Condition c1;
	c1.name("File(1|2)");
	std::cout << "\n  Query for regular-expression \"File(1|2)\" for name: q1.";
	q1.select(c1).show();

	Query<PayLoad> q2(db);
	Condition c2;
	c2.name("File(2|3)");
	std::cout << "\n  Query for regular-expression \"File(2|3)\" for name: q2.";
	q2.select(c2).show();

	std::cout << "\n  Show the intersection between q1 and q2.";
	q1. and (q2).show();

	Query<PayLoad> q3(db);
	Condition c3;
	c3.name("File(3|4)");
	std::cout << "\n  Query for regular-expression \"File(3|4)\" for name: q3.";
	q3.select(c3).show();

	std::cout << "\n  Show q2 or q3.";
	q2.or (q3).show();

	return true;
}

//----< demo first part of requirement #8 >---------------------------
bool Test::testR8a() {
	Utilities::title("Demonstrating Requirement #8a - Creating XML representation of Database using XmlDocument");
	putLine();
	Provider dbp;
	DbCore<PayLoad> db = dbp.db();
	db.deleteEle("F3");
	db.deleteEle("F4");
	showDb(db);
	using Sptr = std::shared_ptr<AbstractXmlElement>;
	Sptr pDb = makeTaggedElement("db");
	Sptr pDocElem = makeDocElement(pDb);
	XmlDocument xDoc(pDocElem);
	for (auto item : db)
	{
		Sptr pRecord = makeTaggedElement("dbRecord");
		pDb->addChild(pRecord);
		Sptr pKey = makeTaggedElement("key", item.first);
		pRecord->addChild(pKey);

		Sptr pValue = makeTaggedElement("value");
		pRecord->addChild(pValue);
		Sptr pName = makeTaggedElement("name", item.second.name());
		pValue->addChild(pName);
		Sptr pDescrip = makeTaggedElement("description", item.second.descrip());
		pValue->addChild(pDescrip);

		Sptr pPayLoad = makeTaggedElement("payLoad");
		pValue->addChild(pPayLoad);
		Sptr pPath = makeTaggedElement("path", item.second.payLoad().path());
		pPayLoad->addChild(pPath);
		for (auto cat : item.second.payLoad().category()) {
			Sptr pCat = makeTaggedElement("category", cat);
			pPayLoad->addChild(pCat);
		}
		for (auto child : item.second.children()) {
			Sptr pChild = makeTaggedElement("child", child);
			pValue->addChild(pChild);
		}
	}
	std::string Xml = xDoc.toString();
	dbp.xml() = Xml;
	std::cout << Xml << std::endl;
	return true;
}

//----< demo second part of requirement #8 >---------------------------
bool Test::testR8b() {
	Utilities::title("Demonstrating Requirement #8b - Building a database from XmlDocument");
	std::cout << "\n  Creating a new database.";
	DbCore<PayLoad> newDb;
	Provider dbp;
	using Sptr = std::shared_ptr<AbstractXmlElement>;
	XmlDocument newXDoc(dbp.xml(), XmlDocument::str);
	std::vector<Sptr> records = newXDoc.descendents("dbRecord").select();
	using Key = std::string;
	for (auto pRecord : records)
	{
		Key key;
		DbElement<PayLoad> elem;
		PayLoad payL;
		std::vector<Sptr> pChildren = pRecord->children();
		for (auto pChild : pChildren) {
			if (pChild->tag() == "key") {
				key = pChild->children()[0]->value();
				if (newDb.contains(key))
					continue;
			}
			else {
				std::vector<Sptr> pValueChildren = pChild->children();
				for (auto pValueChild : pValueChildren) {
					if (pValueChild->tag() == "name") 
						elem.name(pValueChild->children()[0]->value());
					if (pValueChild->tag() == "description") 
						elem.descrip(pValueChild->children()[0]->value());
					if (pValueChild->tag() == "child") 
						elem.children().push_back(pValueChild->children()[0]->value());
					if (pValueChild->tag() == "payLoad") {
						std::vector<Sptr> pPayLChildren = pValueChild->children();
						for (auto payLL : pPayLChildren) {
							if (payLL->tag() == "path")
								payL.path() = payLL->children()[0]->value();
							if(payLL->tag() == "category")
								payL.category().push_back(payLL->children()[0]->value());
						}
					}
				}
			}
		}
		elem.payLoad(payL);
		newDb[key] = elem;
	}
	showDb(newDb);
	putLine();
	return true;
}

//----< demo requirement #9 >---------------------------
bool Test::testR9() {
	Utilities::title("Demonstrating Requirement #9 - Payload type");
	std::cout << "\n  Creating a new database:";
	PayLoad payL;
	payL.path("demopath");
	payL.category().push_back("demo1");
	payL.category().push_back("demo2");
	DbElement<PayLoad> demoEle;
	demoEle.name("R9");
	demoEle.descrip("Demo for payload class");
	demoEle.dateTime(DateTime().now());
	demoEle.payLoad(payL);

	DbCore<PayLoad> db;
	db["r9"] = demoEle;
	payL.path("ddddd");
	demoEle.payLoad(payL);
	demoEle.name("demoR9");
	db["demor9"] = demoEle;
	showDb(db);
	putLine();
	std::cout << "\n  Querying for path: demopath";
	Query<PayLoad> q1(db);
	q1.selectPath("demopath").show();
	putLine();
	return true;
}


#ifdef TEST_TEST
int main() {
	Test testStub;
	if(testStub.testR1())
		return 0;
	return 1;
}
#endif

