#pragma once
/////////////////////////////////////////////////////////////////////
// PayLoad.h - Design a class to test Dbcore                       //
// ver 1.0                                                         //
// Biao A, CSE687 - Object Oriented Design, Spring 2018            //
/////////////////////////////////////////////////////////////////////
/*
* 
* Package Operations:
* -------------------
* Class: PayLoad
* - provides a specific class as data type in database
* - overload some operation for this specific type
*
* Maintenance History:
* --------------------
* ver 1.0 : 2 Feb 2018
*/
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>

/////////////////////////////////////////////////////////////////////
// PayLoad class
// - provides a specific class for data element
class PayLoad {
public:
	using Path = std::string;
	using Category = std::vector<std::string>;

	// make private member accessable
	// methods to get and set

	Path& path() { return path_; }
	Path path() const { return path_; }
	void path(const Path& path) { path_ = path; }

	Category& category() { return category_; }
	Category category() const { return category_; }
	void category(const Category& category) { category_ = category; }

	//overload = operators for this PayLoad

	PayLoad& operator=(const PayLoad& payload) {
		path_ = payload.path();
		category_ = payload.category();
		return *this;
	}

private:
	Path path_;
	Category category_;

};

//----< overload << operator for PayLoad >---------------------------------
std::ostream& operator<<(std::ostream& out, const PayLoad& payload) {
	std::string path = "Path: " + payload.path();
	out << path;
	out << "  " << "Category: ";
	for (auto ca : payload.category()) {
		out << ca << " ";
	}
	return out;
}