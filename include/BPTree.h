//
// Created by zzh on 2020/6/3.
//

#ifndef DB_PRO_BPTREE_H
#define DB_PRO_BPTREE_H
#include "BPNode.h"
#include <algorithm>
#include <vector>
#include <string>

template<typename T>
class BPTree{
private:
    //重命名
    typedef TreeNode<T>* tree_ptr;
    //用于储存查找结果的特殊结构
    struct search_node_parse{
        tree_ptr pNode;
        int index;
        bool bool_found;
    };
public:
    //文件名，文件读写操作
    string file_name;
    //根节点
    tree_ptr tree_root;
    //用一个指针指向叶节点开始处
    tree_ptr leaf_start;
    //节点的key的数量
    int key_num;
    //节点的level
    int level;
    //节点的node的数量
    int node_num;
    //key的size
    int key_size;
    //树的叉数
    int degree;

public:
    //构造函数：文件名，节点大小，度数
    BPTree(string fname,int key_size,int degree);
    ~BPTree();
    //根据key返回value
    int searchVal(T& key);
    //在树中插入一个值,返回表示是否插入成功
    bool insertKey(T& key,int val);
    //删除一个节点
    bool deleteKey(T& key);
    //删除整棵树
    void dropTree(tree_ptr node);
    //从磁盘读取数据
    void readFromDisk();
    //将数据写回磁盘
    void writtenBackDisk();
    //从磁盘读取某一块的数据
    void readFromDiskBlock(char *s,char* e);
    //初始化B+树，分配内存
    void initTree();
    //插入后节点调整
    bool adjustAfterInsert(tree_ptr node);
    //删除后调整
    bool adjustAfterDelete(tree_ptr node);
    //在叶节点查找某个key值
    void findToLeaf(tree_ptr node,T key,search_node_parse&snp);

    void getFile(string filePath);

    int getBlockNum(string table_name);
};

#endif //DB_PRO_BPTREE_H
