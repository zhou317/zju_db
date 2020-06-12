//
// Created by zzh on 2020/6/5.
//

#ifndef DB_PRO_INDEXMANAGER_H
#define DB_PRO_INDEXMANAGER_H
#include "BPTree.h"
#include "bufferManager.h"
#include "catalogManager.h"
#include "base.h"
#include <map>


struct Data{
    int type;
    int datai;
    float dataf;
    std::string datas;
};


using namespace std;

class IndexManager {
public:
    //对每个索引创建一个map来储存对应的b+树
    map<string,BPTree<int>*> intMap;
    map<string,BPTree<string>*> stringMap;
    map<string,BPTree<float>*> floatMap;

public:
    //接受catalog索引信息
    //从磁盘中读取对应的索引
    IndexManager(CatalogManager catalog){
        vector<Index> allidx;
        //获取所有已经创建的索引，并从文件中读取出来
        allidx = catalog.getAllIdx();
        for(int i = 0;i < allidx.size();i++){
            string fname = allidx[i].table_name +"_" + allidx[i].attribute_name+"_"
                    + allidx[i].index_name+".idx";
            cout<<"=========fname "<<fname<<endl;
            Attribute attri = catalog.get_Attribute(allidx[i].table_name,allidx[i].attribute_name);
            createIdx(fname,attri.type,attri.length);
        }
    }

    void createIdx(string fname, int type, int keylength){
        int degree = getDegree(keylength);
        if(type == INT){
            BPTree<int> * tree = new BPTree<int>(fname,keylength,degree);
            intMap[fname] = tree;
        } else if(type == FLOAT){
            BPTree<float> * tree = new BPTree<float>(fname,keylength,degree);
            floatMap[fname] = tree;
        } else{
            BPTree<string> * tree = new BPTree<string>(fname,keylength,degree);
            stringMap[fname] = tree;
        }
        return;
    }

    void dropIdx(string fname, int type){
        //对不同的数据类型采用不同的处理方式
        if(type == INT){
            if(intMap.find(fname) != intMap.end()){
                //存在该索引
                intMap.erase(fname);
            } else{
                cout<<"Error: "<<fname<<"索引不存在"<<endl;
            }
        }else if(type == FLOAT){
            if(floatMap.find(fname) != floatMap.end()){
                floatMap.erase(fname);
            } else{
                cout<<"Error: "<<fname<<"索引不存在"<<endl;
            }
        }else{
            if(stringMap.find(fname) != stringMap.end()){
                stringMap.erase(fname);
            } else{
                cout<<"Error: "<<fname<<"索引不存在"<<endl;
            }
        }
    }


    int findIdx(string fname, Data sdata, int type){
        if(type == INT){
            if(intMap.find(fname) != intMap.end()){
                //存在该索引
                int data = sdata.datai;
                BPTree<int>* t = intMap[fname];
                return t->searchVal(data);
            } else{
                cout<<"Error: "<<fname<<"索引不存在"<<endl;
                return -1;
            }
        }else if(type == FLOAT){
            if(floatMap.find(fname) != floatMap.end()){
                float data = sdata.dataf;
                BPTree<float>* t = floatMap[fname];
                return t->searchVal(data);

            } else{
                cout<<"Error: "<<fname<<"索引不存在"<<endl;
                return -1;
            }
        }else{
            if(stringMap.find(fname) != stringMap.end()){
                string data = sdata.datas;
                BPTree<string>* t = stringMap[fname];
                return t->searchVal(data);
            } else{
                cout<<"Error: "<<fname<<"索引不存在"<<endl;
                return -1;
            }
        }
    }

    void insertIdx(string fname,int type ,Data sdata, int block_id)
    {
        if(type == INT){
            if(intMap.find(fname) != intMap.end()){
                //存在该索引
                int data = sdata.datai;
                BPTree<int>* t = intMap[fname];
                t->insertKey(data,block_id);
            } else{
                cout<<"Error: "<<fname<<"索引不存在"<<endl;
            }
        }else if(type == FLOAT){
            if(floatMap.find(fname) != floatMap.end()){
                float data = sdata.dataf;
                BPTree<float>* t = floatMap[fname];
                t->insertKey(data,block_id);
            } else{
                cout<<"Error: "<<fname<<"索引不存在"<<endl;
            }
        }else{
            if(stringMap.find(fname) != stringMap.end()){
                string data = sdata.datas;
                BPTree<string>* t = stringMap[fname];
                t->insertKey(data,block_id);
            } else{
                cout<<"Error: "<<fname<<"索引不存在"<<endl;
            }
        }
    }

    void deleteIdxByKey(string fname, int type, Data sdata){
        if(type == INT){
            if(intMap.find(fname) != intMap.end()){
                //存在该索引
                int data = sdata.datai;
                BPTree<int>* t = intMap[fname];
                t->deleteKey(data);
            } else{
                cout<<"Error: "<<fname<<"索引不存在"<<endl;
            }
        }else if(type == FLOAT){
            if(floatMap.find(fname) != floatMap.end()){
                float data = sdata.dataf;
                BPTree<float>* t = floatMap[fname];
                t->deleteKey(data);
            } else{
                cout<<"Error: "<<fname<<"索引不存在"<<endl;
            }
        }else{
            if(stringMap.find(fname) != stringMap.end()){
                string data = sdata.datas;
                BPTree<string>* t = stringMap[fname];
                t->deleteKey(data);
            } else{
                cout<<"Error: "<<fname<<"索引不存在"<<endl;
            }
        }
    }

    //计算B+树的叉树
    int getDegree(int attrLength){
        int de = (PAGESIZE)/(attrLength + sizeof(int));
        if(de % 2 == 0)
            de--;
        return de;
    }
};


#endif //DB_PRO_INDEXMANAGER_H
