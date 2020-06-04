//
// Created by zzh on 2020/6/3.
//

#ifndef DB_PRO_BPNODE_H
#define DB_PRO_BPNODE_H
#include <vector>

using namespace std;

template <typename T>
class TreeNode {
public:
    //节点内key的数量
    int key_num;
    //父节点指针
    TreeNode *parent;
    //存放key的容器
    vector<T> keys;
    //存放子节点指针的容器
    vector<TreeNode *> ptrs;
    vector<int> vals;
    //是否是叶子节点
    bool bool_leaf;
    //指向下一个叶子节点的指针
    TreeNode *next_leaf;
    //树的叉数
    int degree;


public:
    //构造函数
    TreeNode(int in_degree, bool bool_leaf = false);
    //析构函数
    ~TreeNode();
    bool isRoot();
    //判断是否存在key值
    //返回值：存在true, 不存在 false
    //地址index: 返回该值的索引
    bool findKey(T key,int &index);
    //将一个节点分作两个节点
    //返回后一个节点
    //key用作传入父节点
    TreeNode* split(T& key);
    //在非叶节点添加一个值，返回添加的位置
    int add(T &key);
    //在叶子节点添加一个值，返回添加位置
    int add(T& key,int val);
    //在节点中删除index对应的值
    bool remove_at(int index);
    //调试输出
    void debug_print();
};




#endif //DB_PRO_BPNODE_H
