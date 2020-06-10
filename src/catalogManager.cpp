//catalogManager.cpp

#include "catalogManager.h"
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
using namespace std;

//读入table.catlog
void CatalogManager::readTable()
{
	int i, j;
	const string filename = "..\\database\\catalog\\table.catalog";


	fstream  fin(filename.c_str(), ios::in);
	fin >> tableNum;
	for (i = 0; i < tableNum; i++)
	{
		Table temp_table;
		fin >> temp_table.name;
		fin >> temp_table.attriNum;
		fin >> temp_table.blockNum;
		for (j = 0; j < temp_table.attriNum; j++)
		{
			Attribute temp_attri;
			fin >> temp_attri.name;
			fin >> temp_attri.type;
			fin >> temp_attri.length;
			fin >> temp_attri.isPrimery;
			fin >> temp_attri.isUnique;
			temp_table.attributes.push_back(temp_attri);
		}
		tables.push_back(temp_table);
	}
	fin.close();
}

//读入index.catlog
void CatalogManager::readIndex()
{
	int i;
	const string filename = "..\\database\\catalog\\index.catalog";
	fstream  fin(filename.c_str(), ios::in);
	fin >> indexNum;
	for (i = 0; i < indexNum; i++)
	{
		Index temp_index;
		fin >> temp_index.table_name;
		fin >> temp_index.attribute_name;
		fin >> temp_index.index_name;
		fin >> temp_index.blockNum;
		indexes.push_back(temp_index);
	}
	fin.close();
}

//写入table.catlog
void CatalogManager::writeTable()
{
	int i, j;
	const string filename = "..\\database\\catalog\\table.catalog";
	fstream  fout(filename.c_str(), ios::out);

	fout << tableNum << endl;
	for (i = 0; i < tableNum; i++)
	{
		fout << tables[i].name << " ";
		fout << tables[i].attriNum << " ";
		fout << tables[i].blockNum << endl;

		for (j = 0; j < tables[i].attriNum; j++)
		{
			fout << tables[i].attributes[j].name << " ";
			fout << tables[i].attributes[j].type << " ";
			fout << tables[i].attributes[j].length << " ";
			fout << tables[i].attributes[j].isPrimery << " ";
			fout << tables[i].attributes[j].isUnique << endl;
		}
	}
	fout.close();
}

//写入index.catlog
void CatalogManager::writeIndex()
{
	int i;
	const string filename = "..\\database\\catalog\\index.catalog";
	fstream  fout(filename.c_str(), ios::out);
	fout << indexNum << endl;
	for (int i = 0; i < indexNum; i++)
	{
		fout << indexes[i].table_name << " ";
		fout << indexes[i].attribute_name << " ";
		fout << indexes[i].index_name << " ";
		fout << indexes[i].blockNum << endl;
	}
	fout.close();
}

//是否存在表
bool CatalogManager::has_Table(string tname)
{
    int i;
    for (i = 0;i < tableNum;i++) {
        if (tables[i].name == tname) {
            return true;
        }
    }
    return false;
}

//是否存在属性
bool CatalogManager::has_Attribute(string tname, string aname)
{
    int i, j;
    for (i = 0;i < tableNum;i++) {
        if (tables[i].name == tname) {
            for (j = 0;j < tables[i].attributes.size();j++) {
                if (tables[i].attributes[j].name == aname) {
                    return true;
                }
            }
        }
    }
    return false;
}

//是否存在索引，属性名
bool CatalogManager::has_Index(string tname, string aname)
{
    int i;
    for (i = 0;i < indexNum;i++) {
        if (indexes[i].table_name == tname && indexes[i].attribute_name==aname) {
            return true;
        }
    }
    return false;
}

//是否存在索引，索引名
bool CatalogManager::has_Index(string iname)
{
    int i;
    for (i = 0;i < indexNum;i++) {
        if (indexes[i].index_name == iname) {
            return true;
        }
    }
    return false;
}

//建立表
void CatalogManager::create_Table(Table& table)
{
    try {
        if (!has_Table(table.name)) {
            tableNum++;
            tables.push_back(table);
        }
        else {
            //抛出异常，表重复定义
            throw TableExpection("table repeat definition");
        }
    }
    catch (TableExpection e) {
        throw e;
    }
}

//插入属性
void CatalogManager::create_Attribute(string tname, Attribute& attribute)
{
    try {
        int i;
        if (!has_Attribute(tname, attribute.name)) {
            for (i = 0;i < tableNum;i++) {
                if (tables[i].name == tname) {
                    tables[i].attributes.push_back(attribute);
                    tables[i].attriNum++;
                }
            }
        }
        else {
            //抛出异常，属性重复定义
            throw TableExpection("attribute repeat definition");
        }
    }
    catch (TableExpection e) {
        throw e;
    }
}

//建立索引
void CatalogManager::create_Index(Index& index)
{
    try {
        if (!has_Index(index.index_name)) {
            indexNum++;
            indexes.push_back(index);
        }
        else {
            //抛出异常，索引重复定义
            throw TableExpection("index repeat definition");
        }
    }
    catch (TableExpection e) {
        throw e;
    }
}

//删除表
void CatalogManager::drop_Table(string tname)
{
    try {
        int i;
        if (has_Table(tname)) {
            for (i = 0;i < tableNum;i++) {
                if (tables[i].name == tname) {
                    tables.erase(tables.begin() + i);
                    tableNum--;
                }
            }
            for (i = 0;i < indexNum;i++) {//将表上的索引一并删除
                if (indexes[i].table_name == tname) {
                    indexes.erase(indexes.begin() + i);
                    indexNum--;
                }
            }
        }
        else {
            //抛出异常，表不存在
            throw TableExpection("table does not exist");
        }
    }
    catch (TableExpection e) {
        throw e;
    }
}

//删除属性
void CatalogManager::drop_Atttribute(string tname, string aname)
{
    try {
        int i, j;
        if (has_Attribute(tname, aname)) {
            for (i = 0;i < tableNum;i++) {
                if (tables[i].name == tname) {
                    for (j = 0;j < tables[i].attributes.size();j++) {
                        if (tables[i].attributes[j].name == aname) {
                            tables[i].attributes.erase(tables[i].attributes.begin() + j);
                            tables[i].attriNum--;
                        }
                    }
                }
            }
        }
        else {
            //抛出异常，属性不存在
            throw TableExpection("attribute does not exist");
        }
    }
    catch (TableExpection e) {
        throw e;
    }
}

//删除索引
void CatalogManager::drop_Index(string iname)
{
    try {
        int i;
        if (has_Index(iname)) {
            for (i = 0;i < indexNum;i++) {
                if (indexes[i].index_name == iname) {
                    indexes.erase(indexes.begin() + i);
                    indexNum--;
                }
            }
        }
        else {
            //抛出异常，索引不存在
            throw TableExpection("index does not exist");
        }
    }
    catch (TableExpection e) {
        throw e;
    }
}

//删除索引，某张表的某个属性
void CatalogManager::drop_Index(string tname, string aname)
{
    try {
        int i;
        if (has_Index(tname, aname)) {
            for (i = 0;i < indexNum;i++) {
                if (indexes[i].table_name == tname && indexes[i].attribute_name == aname) {
                    indexes.erase(indexes.begin() + i);
                    indexNum--;
                }
            }
        }
        else {
            //抛出异常，索引不存在
            throw TableExpection("index does not exist");
        }
    }
    catch (TableExpection e) {
        throw e;
    }
}

//修改表
void CatalogManager::update_Table(Table& table)
{
    try {
        int i;
        if (has_Table(table.name)) {
            for (i = 0;i < tableNum;i++) {
                if (tables[i].name == table.name) {
                    tables[i].attriNum = table.attriNum;
                    tables[i].blockNum = table.blockNum;
                    tables[i].attributes = table.attributes;
                }
            }
        }
        else {
            //抛出异常，表不存在
            throw TableExpection("table does not exist");
        }
    }
    catch (TableExpection e) {
        throw e;
    }
}

//修改属性
void CatalogManager::update_Attribute(string tname, Attribute& attribute)
{
    try {
        int i, j;
        if (has_Attribute(tname, attribute.name)) {
            for (i = 0;i < tableNum;i++) {
                if (tables[i].name == tname) {
                    for (j = 0;j < tables[i].attributes.size();j++) {
                        if (tables[i].attributes[j].name == attribute.name) {
                            tables[i].attributes[j].type = attribute.type;
                            tables[i].attributes[j].length = attribute.length;
                            tables[i].attributes[j].isPrimery = attribute.isPrimery;
                            tables[i].attributes[j].isUnique = attribute.isUnique;
                        }
                    }
                }
            }
        }
        else {
            //抛出异常，属性不存在
            throw TableExpection("attribute does not exist");
        }
    }
    catch (TableExpection e) {
        throw e;
    }
}

//修改索引
void CatalogManager::update_Index(Index& index)
{
    try {
        int i;
        if (has_Index(index.index_name)) {
            for (i = 0;i < indexNum;i++) {
                if (indexes[i].index_name == index.index_name) {
                    indexes[i].table_name = index.table_name;
                    indexes[i].attribute_name = index.attribute_name;
                    indexes[i].blockNum = index.blockNum;
                }
            }
        }
        else {
            //抛出异常，索引不存在
            throw TableExpection("index does not exist");
        }
    }
    catch (TableExpection e) {
        throw e;
    }
}

//获取表
Table& CatalogManager::get_Table(string tname)
{
    try {
        int i;
        if (has_Table(tname)) {
            for (i = 0;i < tableNum;i++) {
                if (tables[i].name == tname) {
                    return tables[i];
                }
            }
        }
        else {
            //抛出异常，表不存在
            throw TableExpection("table does not exist");
        }
    }
    catch (TableExpection e) {
        throw e;
    }
}

//获取属性
Attribute& CatalogManager::get_Attribute(string tname, string aname)
{
    try {
        int i, j;
        if (has_Attribute(tname, aname)) {
            for (i = 0;i < tableNum;i++) {
                if (tables[i].name == tname) {
                    for (j = 0;j < tables[i].attributes.size();j++) {
                        if (tables[i].attributes[j].name == aname) {
                            return tables[i].attributes[j];
                        }
                    }
                }
            }
        }
        else {
            //抛出异常，属性不存在
            throw TableExpection("attribute does not exist");
        }
    }
    catch (TableExpection e) {
        throw e;
    }
}

//获取索引
Index& CatalogManager::get_Index(string iname)
{
    try {
        int i;
        if (has_Index(iname)) {
            for (i = 0;i < indexNum;i++) {
                if (indexes[i].index_name == iname) {
                    return indexes[i];
                }
            }
        }
        else {
            //抛出异常，索引不存在
            throw TableExpection("index does not exist");
        }
    }
    catch (TableExpection e) {
        throw e;
    }
}

//获取索引，某张表的某个属性
Index& CatalogManager::get_Index(string tname, string aname)
{
    try {
        int i;
        if (has_Index(tname, aname)) {
            for (i = 0;i < indexNum;i++) {
                if (indexes[i].table_name == tname && indexes[i].attribute_name == aname) {
                    return indexes[i];
                }
            }
        }
        else {
            //抛出异常，索引不存在
            throw TableExpection("index does not exist");
        }
    }
    catch (TableExpection e) {
        throw e;
    }
}

//展示表，用于测试程序
void CatalogManager::show_Table()
{
    int i, j;
    cout << "##    show table    ##" << endl;
    cout << "table number: " << tableNum << endl;
    for (i = 0;i < tableNum;i++) {
        cout << "TABLE " << i << endl;
        cout << "table name: " << tables[i].name << endl;
        cout << "attribute number: " << tables[i].attriNum << endl;
        cout << "block number: " << tables[i].blockNum << endl;
        for (j = 0;j < tables[i].attriNum;j++) {
            cout << tables[i].attributes[j].name << " ";
            switch (tables[i].attributes[j].type)
            {
                case CHAR: cout << "char(" << tables[i].attributes[j].length << ") ";	break;
                case INT: cout << "int ";	break;
                case FLOAT: cout << "float ";	break;
                default: cout << "unknow ";	break;
            }
            if (tables[i].attributes[j].isPrimery) {
                cout << "primery key ";
            }
            if (tables[i].attributes[j].isUnique) {
                cout << "unique " << endl;
            }
            else {
                cout << "not unique " << endl;
            }
        }
        cout << endl;
    }
}

//展示索引，用于测试程序
void CatalogManager::show_Index()
{
    int i;
    cout << "##    show index    ##" << endl;
    cout << "index number: " << indexNum << endl;
    for (i = 0;i < indexNum;i++) {
        cout << "INDEX " << i << endl;
        cout << "index name: " << indexes[i].index_name << endl;
        cout << "table name: " << indexes[i].table_name << endl;
        cout << "attribute name: " << indexes[i].attribute_name << endl;
        cout << "block number: " << indexes[i].blockNum << endl;
    }
}