#pragma once
//base.h
//���涨�������ݿ���Ҫ�ı����Ժ�������
#ifndef base_h
#define base_h

#define INT 1
#define CHAR 2
#define FLOAT 3

#include <iostream>
#include <vector>
#include <string>
using namespace std;

//�ֶ���
class Attribute
{
public:
	string name;//��������
	int type;//��������
	int length;//���������char�Ļ�����Ҫ����
	bool isPrimery;//�Ƿ�������
	bool isUnique;//�Ƿ�Ψһ
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


//����
class Table
{
public:
	string name;//�������
	int attriNum;//������Ŀ
	int blockNum;//�������е�λ��
	vector<Attribute> attributes;//��������
	Table() :attriNum(0),blockNum(0) {}
	Table(string n,int a,int b,vector<Attribute> attri):name(n),attriNum(a),blockNum(b),attributes(attri) {}
};

//������
class Index
{
public:
	string table_name;//���������ű���
	string attribute_name;//�������ĸ�������
	string index_name;//��������
	int blockNum;//���������е�λ��
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