#pragma once
/////////////////////////////////////////////////////////////////////
// Query.h  - Implements Query for the database                    //
// ver 1.0                                                         //
// Biao A CSE 687                                                  //
/////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* Including two classes: Condition and Query
* Condition:
* - record the select condition for query.
* - including: regular-expression for metadata and time interval 
*
* Query:
* - provide query functions for the database
* - including:
*   query for a condition
*   query for a special key
*	query for children
*	and and or functions
	display function for keys in this query
*
* Required Files:
* ---------------
* DbCore.h
*
* Maintenance History:
* --------------------
* ver 1.0 : 2 Feb 2018
*/
#include <unordered_map>
#include <string>
#include <vector>
#include <set>
#include <regex>
#include <algorithm>
#include <iostream>
#include "../DbCore/DbCore.h"

using namespace NoSqlDb;

/////////////////////////////////////////////////////////////////////
// Condition class
// - record the select condition for query
class Condition {
	using Pattern = std::string;
public:
	//----< constructor for Condition class: set default value >----------------
	Condition(){
		name_ = "default";
		description_ = "default";
		keyPattern_ = "default";
		timeDefault_ = "default";
	}

	// methods to set pattern and return the reference to itselt
	Condition& name(const Pattern& name) { name_ = name; return *this; }
	Pattern name() const { return name_; }

	Condition& descrip(const Pattern& descrip) { description_ = descrip; return *this; }
	Pattern descrip() const { return description_; }

	Condition& keyPattern(const Pattern& keyPattern) { keyPattern_ = keyPattern; return *this; }
	Pattern keyPattern() const { return keyPattern_; };

	Condition& timeInterval(const DateTime& begin, const DateTime& end = DateTime().now()) {
		timeDefault_ = "no";
		timeBegin_ = begin;
		timeEnd_ = end;
		return *this;
	}
	DateTime timeBegin() const { return timeBegin_; }
	DateTime timeEnd() const { return timeEnd_; }
	std::string timeDefault() const { return timeDefault_; }
	
private:
	Pattern name_;
	Pattern description_;
	Pattern keyPattern_;
	std::string timeDefault_;
	DateTime timeBegin_;
	DateTime timeEnd_;
};


/////////////////////////////////////////////////////////////////////
// Query class
// - provide query functions for the database
template<typename T>
class Query {
public:
	using Key = std::string;
	using Keys = std::vector<Key>;

	//methods to make queries and access Query's qkeys
	Query(DbCore<T> &db) : qdb(db),qkeys(db.keys()) {};
	Query& select(const Condition& c);
	void selectOneKey(Key key);
	Query& selectChildren(Key key);
	Query& selectPath(std::string path);
	Query& from(const Keys& keys);
	Query& and(Query<T> q);
	Query& or(Query<T> q);
	void show();
	Keys keys() { return qkeys; };

private:
	void selectName(const std::string& name);
	void selectKey(const std::string& key);
	void selectDescrip(const std::string& descrip);
	void selectInterval(const DateTime& timeBegin, const DateTime& timeEnd);
	DbCore<T>& qdb;
	Keys qkeys;
};

/////////////////////////////////////////////////////////////////////
// Query<T> public methods 

//----< select a set of key of a condition >-----------------------
template<typename T>
Query<T>& Query<T>::select(const Condition &c) {
	if (c.name() != "default")
		selectName(c.name());
	if (c.keyPattern() != "default")
		selectKey(c.keyPattern());
	if (c.descrip() != "default")
		selectDescrip(c.descrip());
	if (c.timeDefault() != "default") {
		selectInterval(c.timeBegin(), c.timeEnd());
	}
	return *this;
}

//----< select one key and show its content >-----------------------
template<typename T>
void Query<T>::selectOneKey(Key key) {
	bool flag = true;
	for (auto i : qkeys)
		if (i == key) {
			NoSqlDb::showElem(qdb[key]);
			flag = false;
		}
	if (flag) {
		std::cout << "  This Query does not include this key!" << std::endl;
		return;
	}
	qkeys.clear();
	qkeys.push_back(key);
}

//----< select children of a specified key >-----------------------
template<typename T>
Query<T>& Query<T>::selectChildren(Key key) {
	if (!qdb.contains(key)) {
		std::cout << "  This Database does not include this key!" << std::endl;
		return *this;
	}
	Keys& keys = qdb[key].children();
	for (int i = 0; i < keys.size(); i++) {
		if (!qdb.contains(keys[i]))
			keys.erase(keys.begin() + i);
	}
	qkeys = keys;
	return *this;
}

//----< query for a path name in payload >-----------------------
template<typename T>
Query<T>& Query<T>::selectPath(std::string path) {
	if (std::is_same<T, PayLoad>::value) { // if T is PayLoad?
		std::cout << std::endl;
		std::cout << "  T is Payload! Continue query...";
	}
	else {
		std::cout << std::endl;
		std::cout << "  Not payLoad type! End query...";
		return *this;
	}
	for (int i = 0; i < qkeys.size(); i++) {
		if(qdb[qkeys[i]].payLoad().path() != path)
			qkeys.erase(qkeys.begin() + i);
	}
	return *this;
}

//----< get keys from a vector >-----------------------------------
template<typename T>
Query<T>& Query<T>::from(const Keys& keys) {
	qkeys = keys;
	return *this;
}

//----< implement the "and" function >--------------------------------
template<typename T>
Query<T>& Query<T>::and(Query<T> q) {
	Keys keys = q.keys();
	Keys nKeys;
	std::sort(qkeys.begin(), qkeys.end());
	std::sort(keys.begin(), keys.end());
	std::set_intersection(qkeys.begin(), qkeys.end(), keys.begin(), keys.end(), back_inserter(nKeys));
	qkeys = nKeys;
	return *this;
}

//----< implement the "or" function >--------------------------------
template<typename T>
Query<T>&  Query<T>::or(Query<T> q) {
	Keys keys = q.keys();
	std::set<Key> nKeys;
	nKeys.insert(keys.begin(), keys.end());
	nKeys.insert(qkeys.begin(), qkeys.end());
	qkeys.clear();
	for (auto key : nKeys)
		qkeys.push_back(key);
	return *this;
}

//----< show keys in this query >----------------------------------
template<typename T>
void Query<T>::show() {
	std::cout << std::endl;
	std::cout << "  Show all the keys in this query: ";
	for (auto key : qkeys)
		std::cout << key << " ";
	std::cout << std::endl;
}

/////////////////////////////////////////////////////////////////////
// Query<T> private methods 

//----< select a set of keys whose names match a regular-expression >------
template<typename T>
void Query<T>::selectName(const std::string& name) {
	std::regex re(name);
	Keys newQkeys;
	for (auto key : qkeys) {
		std::string temp = qdb[key].name();
		bool ret = std::regex_match(temp, re);
		if (ret)
			newQkeys.push_back(key);
	}
	qkeys = newQkeys;
}

//----< select a set of keys which match a  regular-expression >------
template<typename T>
void Query<T>::selectKey(const std::string& key) {
	std::regex re(key);
	Keys newQkeys;
	for (auto key : qkeys) {
		std::string temp = key;
		bool ret = std::regex_match(temp, re);
		if (ret)
			newQkeys.push_back(key);
	}
	qkeys = newQkeys;
}

//----< select a set of keys whose description match a regular-expression >------
template<typename T>
void Query<T>::selectDescrip(const std::string& descrip) {
	std::regex re(descrip);
	Keys newQkeys;
	for (auto key : qkeys) {
		std::string temp = qdb[key].descrip();
		bool ret = std::regex_match(temp, re);
		if (ret)
			newQkeys.push_back(key);
	}
	qkeys = newQkeys;
}

//----< select a set of keys whose description match a regular-expression >------
template<typename T>
void Query<T>::selectInterval(const DateTime& timeBegin, const DateTime& timeEnd) {
	Keys newQkeys;
	for (auto key : qkeys) {
		DateTime a(qdb[key].dateTime());
		if (a > timeBegin && a < timeEnd)
			newQkeys.push_back(key);
	}
	qkeys = newQkeys;
}




