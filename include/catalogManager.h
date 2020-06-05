#pragma once
//catalogManager.h
//CatalogManager类
#ifndef catalogManager_h
#define catalogManager_h

#include "base.h"
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
using namespace std;

class CatalogManager
{
private:
	int tableNum;//表数目
	int indexNum;//索引数目
	vector<Table> tables;//表数组
	vector<Index> indexes;//索引数组
	void readTable();//读入table.catlog
	void readIndex();//读入index.catlog
	void writeTable();//写入table.catlog
	void writeIndex();//写入index.catlog
public:
	CatalogManager()
	{
		readTable();
		readIndex();
	}
	~CatalogManager()
	{
		writeTable();
		writeIndex();
	}
	bool has_Table(string tname);//是否存在该表
	bool has_Attribute(string tname, string aname);//是否存在属性
	bool has_Index(string tname, string aname);//是否存在索引，属性名
	bool has_Index(string iname);//是否存在索引，索引名
	void create_Table(Table& table);//建立表
	void create_Attribute(string tname, Attribute& attribute);//插入属性
	void create_Index(Index& index);//建立索引
	void drop_Table(string tname);//删除表
	void drop_Atttribute(string tname, string aname);//删除属性
	void drop_Index(string iname);//删除索引
	void drop_Index(string tname, string aname);//删除索引，某张表的某个属性
	void update_Table(Table& table);//修改表
	void update_Attribute(string tname, Attribute& attribute);//修改属性
	void update_Index(Index& index);//修改索引
	Table& get_Table(string tname);//获取表
	Attribute& get_Attribute(string tname, string aname);//获取属性
	Index& get_Index(string iname);//获取索引
	Index& get_Index(string tname, string aname);//获取索引，某张表的某个属性
};

#endif // !catalogManager_h
