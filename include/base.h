#pragma once
//base.h
//里面定义了数据库需要的表，属性和索引类
#ifndef base_h
#define base_h

#define INT 1
#define CHAR 2
#define FLOAT 3

#include <iostream>
#include <vector>
#include <string>
using namespace std;

//字段类
class Attribute
{
public:
	string name;//属性名称
	int type;//属性类型
	int length;//如果类型是char的话，需要长度
	bool isPrimery;//是否是主键
	bool isUnique;//是否唯一
	Attribute()
	{
		type = 0;
		length = 0;
		isPrimery = false;
		isUnique = false;
	}
	Attribute(string n, int t, int l, bool p, bool u)
		:name(n), type(t), length(l), isPrimery(p), isUnique(u) {}
};


//表类
class Table
{
public:
	string name;//表的名称
	int attriNum;//属性数目
	int blockNum;//表在盘中的位置
	vector<Attribute> attributes;//属性数组
	Table() :attriNum(0),blockNum(0) {}
	Table(string n,int a,int b,vector<Attribute> attri):name(n),attriNum(a),blockNum(b),attributes(attri) {}
};

//索引类
class Index
{
public:
	string table_name;//建立在哪张表上
	string attribute_name;//建立在哪个属性上
	string index_name;//索引名称
	int blockNum;//索引在盘中的位置
	Index():blockNum(0) {}
	Index(string t,string a,string i,int b):table_name(t),attribute_name(a),index_name(i),blockNum(0) {}
};

class TableExpection :public exception
{
private:
	string exp;
public:
	TableExpection(string s):exp(s) {}
	string what()
	{
		return exp;
	}
};

#endif // !base_h