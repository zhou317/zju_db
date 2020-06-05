#pragma once
//catalogManager.h
//CatalogManager��
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
	int tableNum;//����Ŀ
	int indexNum;//������Ŀ
	vector<Table> tables;//������
	vector<Index> indexes;//��������
	void readTable();//����table.catlog
	void readIndex();//����index.catlog
	void writeTable();//д��table.catlog
	void writeIndex();//д��index.catlog
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
	bool has_Table(string tname);//�Ƿ���ڸñ�
	bool has_Attribute(string tname, string aname);//�Ƿ��������
	bool has_Index(string tname, string aname);//�Ƿ����������������
	bool has_Index(string iname);//�Ƿ����������������
	void create_Table(Table& table);//������
	void create_Attribute(string tname, Attribute& attribute);//��������
	void create_Index(Index& index);//��������
	void drop_Table(string tname);//ɾ����
	void drop_Atttribute(string tname, string aname);//ɾ������
	void drop_Index(string iname);//ɾ������
	void drop_Index(string tname, string aname);//ɾ��������ĳ�ű��ĳ������
	void update_Table(Table& table);//�޸ı�
	void update_Attribute(string tname, Attribute& attribute);//�޸�����
	void update_Index(Index& index);//�޸�����
	Table& get_Table(string tname);//��ȡ��
	Attribute& get_Attribute(string tname, string aname);//��ȡ����
	Index& get_Index(string iname);//��ȡ����
	Index& get_Index(string tname, string aname);//��ȡ������ĳ�ű��ĳ������
};

#endif // !catalogManager_h
