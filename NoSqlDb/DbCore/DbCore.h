#pragma once
/////////////////////////////////////////////////////////////////////
// DbCore.h - Implements NoSql database prototype                  //
// ver 1.0                                                         //
// Biao A, CSE687 - Object Oriented Design, Spring 2018            //
/////////////////////////////////////////////////////////////////////
/*
* P.S.: Based on the DbCore.h by Dr.Fawcett
* Package Operations:
* -------------------
* This package provides two classes:
* - DbCore implements core NoSql database operations including editing the database
    and the value of data member
* - DbElement provides the value part of our key-value database.
*   It contains fields for name, description, date, child collection
*   and a payload field of the template type. 
* The package also provides functions for displaying:
* - set of all database keys
* - database elements
* - all records in the database

* Required Files:
* ---------------
* DbCore.h, DbCore.cpp
* DateTime.h, DateTime.cpp
* Utilities.h, Utilities.cpp
*
* Maintenance History:
* --------------------
* ver 1.0 : 2 Feb 2018
*/

#include <unordered_map>
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include "../DateTime/DateTime.h"


namespace NoSqlDb
{
  /////////////////////////////////////////////////////////////////////
  // DbElement class
  // - provides the value part of a NoSql key-value database

  template<typename T>
  class DbElement
  {
  public:
    using Key = std::string;
    using Children = std::vector<Key>;

	//make private member accessable
    // methods to get and set DbElement fields

    std::string& name() { return name_; }
    std::string name() const { return name_; }
    void name(const std::string& name) { name_ = name; }

    std::string& descrip() { return descrip_; }
    std::string descrip() const { return descrip_; }
    void descrip(const std::string& name) { descrip_ = name; }
    
    DateTime& dateTime() { return dateTime_; }
    DateTime dateTime() const { return dateTime_; }
    void dateTime(const DateTime& dateTime) { dateTime_ = dateTime; }

    Children& children() { return children_; }
    Children children() const { return children_; }
    void children(const Children& children) { children_ = children; }

    T& payLoad() { return payLoad_; }
    T payLoad() const { return payLoad_; }
    void payLoad(const T& payLoad) { payLoad_ = payLoad; }

  private:
    std::string name_;
    std::string descrip_;
    DateTime dateTime_;
    Children children_;
    T payLoad_;
  };

  /////////////////////////////////////////////////////////////////////
  // DbCore class
  // - provides core NoSql db operations
  // - does not provide editing, querying, or persistance operations

  template <typename T>
  class DbCore
  {
  public:
    using Key = std::string;
    using Keys = std::vector<Key>;
    using Children = Keys;
    using DbStore = std::unordered_map<Key,DbElement<T>>;
    using iterator = typename DbStore::iterator;

    // methods to access database elements

    Keys keys();
    bool contains(const Key& key);
    size_t size();
    DbElement<T>& operator[](const Key& key);
    DbElement<T> operator[](const Key& key) const;
    typename iterator begin() { return dbStore_.begin(); }
    typename iterator end() { return dbStore_.end(); }

    // methods to get and set the private database hash-map storage

    DbStore& dbStore() { return dbStore_; }
    DbStore dbStore() const { return dbStore_; }
    void dbStore(const DbStore& dbStore) { dbStore_ = dbStore; }

	//methods to edit the database
	bool deleteEle(const Key& key);
	bool addEle(const Key& key, DbElement<T> dbEle);
	bool replace(Key key, DbElement<T> dbEle);
	bool edit(Key key, std::string type, std::string str);
	bool addChild(Key key, Key ckey);
	bool delChild(Key key, Key ckey);
  
  private:
    DbStore dbStore_;
  };

  /////////////////////////////////////////////////////////////////////
  // DbCore<T> methods 

  //----< does db contain this key? >----------------------------------

  template<typename T>
  bool DbCore<T>::contains(const Key& key)
  {
    iterator iter = dbStore_.find(key);
    return iter != dbStore_.end();
  }
  //----< returns current key set for db >-----------------------------

  template<typename T>
  typename DbCore<T>::Keys DbCore<T>::keys()
  {
    DbCore<T>::Keys dbKeys;
    DbStore& dbs = dbStore();
    size_t size = dbs.size();
    dbKeys.reserve(size);
    for (auto item : dbs)
    {
      dbKeys.push_back(item.first);
    }
    return dbKeys;
  }
  //----< return number of db elements >-------------------------------

  template<typename T>
  size_t DbCore<T>::size()
  {
    return dbStore_.size();
  }
  //----< extracts value from db with key >----------------------------
  /*
  *  - indexes non-const db objects
  */
  template<typename T>
  DbElement<T>& DbCore<T>::operator[](const Key& key)
  {
    if (!contains(key))
    {
      dbStore_[key] = DbElement<T>();
    }
    return dbStore_[key];
  }
  //----< extracts value from db with key >----------------------------
  /*
  *  - indexes const db objects
  */
  template<typename T>
  DbElement<T> DbCore<T>::operator[](const Key& key) const
  {
    if (!contains(key))
    {
      dbStore_[key] = DbElement<T>();
    }
    return dbStore_[key];
  }

  //----< delete a pair according to the key >---------------------------
  template<typename T>
  bool DbCore<T>::deleteEle(const Key& key) {
	  if (!contains(key)) {
		  std::cout << std::endl;
		  std::cout << "  This database does not contain this key!"<<std::endl;
		  return false;
	  }
	  dbStore_.erase(key);
	  return true;

  }

  //----< delete a pair according to the key >---------------------------
  template<typename T>
  bool DbCore<T>::addEle(const Key& key, DbElement<T> dbEle) {
	  if (contains(key)) {
		  std::cout << std::endl;
		  std::cout << "  This key has already exist, please use replace method!" << std::endl;
		  return false;
	  }
	  dbStore_[key] = dbEle;
	  return true;
  }

  //----< replace a data element >---------------------------
  template<typename T>
  bool DbCore<T>::replace(Key key, DbElement<T> dbEle) {
	  std::cout << std::endl;
	  if (!contains(key)) {
		  std::cout << "  This Database does not include this key!" << std::endl;
		  return false;
	  }
	  std::cout << "  Replace the element of Key:" << key;
	  dbStore_[key] = dbEle;
	  return true;
  }

  //----< edit a data element >---------------------------
  template<typename T>
  bool DbCore<T>::edit(Key key, std::string type, std::string str) {
	  std::cout << std::endl;
	  if (!contains(key)) {
		  std::cout << "  This Database does not include this key!" << std::endl;
		  return false;
	  }
	  DbElement<T>& ele = dbStore_[key];
	  std::cout << "  Editing " << type;
	  if (type == "name") {
		  ele.name() = str;
	  }
	  else if (type == "description") {
		  ele.descrip() = str;
	  }
	  else 
		  std::cout << "  Types only include: name, datetime, payload" << std::endl;
	  return true;
  }

  //----< add a child to a key >---------------------------
  template<typename T>
  bool DbCore<T>::addChild(Key key, Key ckey) {
	  if (!contains(key)) {
		  std::cout << "  The key:" << key << " does not exist!" << std::endl;
		  return false;
	  }
	  dbStore_[key].children().push_back(ckey);
	  return true;
  }

  //----< delete a child of a key >---------------------------
  template<typename T>
  bool DbCore<T>::delChild(Key key, Key ckey) {
	  if (!contains(key)) {
		  std::cout << "  The key:" << key << " does not exist!" << std::endl;
		  return false;
	  }
	  DbCore<T>::Keys& reKeys = dbStore_[key].children();
	  for (int i = 0; i < reKeys.size(); i++) {
		  if (ckey == reKeys[i]) {
			  reKeys.erase(reKeys.begin() + i);
			  return true;
		  }
	  }
	  std::cout << "  " << key << " does not have this child!" << std::endl;
	  return false;

  }

  /////////////////////////////////////////////////////////////////////
  // display functions

  //----< display database key set >-----------------------------------

  template<typename T>
  void showKeys(DbCore<T>& db, std::ostream& out = std::cout)//out = cout by default
  {
    out << "\n  ";
    for (auto key : db.keys())
    {
      out << key << " ";
    }
  }
  //----< show record header items >-----------------------------------

  inline void showHeader(std::ostream& out = std::cout)
  {
    out << "\n  ";
    out << std::setw(26) << std::left << "DateTime";
    out << std::setw(10) << std::left << "Name";
    out << std::setw(25) << std::left << "Description";
    out << std::setw(25) << std::left << "Payload";
    out << "\n  ";
    out << std::setw(26) << std::left << "------------------------";
    out << std::setw(10) << std::left << "--------";
    out << std::setw(25) << std::left << "-----------------------";
    out << std::setw(25) << std::left << "-------------------------------------";
  }
  //----< display DbElements >-----------------------------------------

  template<typename T>
  void showElem(const DbElement<T>& el, std::ostream& out = std::cout)
  {
    out << "\n  ";
    out << std::setw(26) << std::left << std::string(el.dateTime());
    out << std::setw(10) << std::left << el.name();
    out << std::setw(25) << std::left << el.descrip();
    out << std::setw(10) << std::left << el.payLoad();
    typename DbElement<T>::Children children = el.children();
    if (children.size() > 0)
    {
      out << "\n    child keys: ";
      for (auto key : children)
      {
        out << " " << key;
      }
    }
  }
  //----< display all records in database and check the children of each element>------

  template<typename T>
  void showDb(DbCore<T>& db, std::ostream& out = std::cout)
  {
    showHeader(out);
    typename DbCore<T>::DbStore& dbs = db.dbStore();
	for (auto item : dbs) {
		std::vector<std::string> rekeys = item.second.children();
		for (int i = 0; i < rekeys.size(); i++) {
			if (!db.contains(rekeys[i]))
				db.delChild(item.first, rekeys[i]);
		}
	}
    for (auto item : dbs)
    {
		showElem(item.second, out);
    }
  }
}
