//
// Created by zzh on 2020/6/3.
//

#ifndef DB_PRO_BPNODE_H
#define DB_PRO_BPNODE_H
#include <vector>
#include "BPNode.h"
#include <iostream>
#include <algorithm>

#ifndef ZZHDEBUG
#define ZZHDEBUG
#endif

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
    void debug_print_onlykey() {
        cout<<"k|";
        for(int i = 0;i<key_num;i++)
            cout<<keys[i]<<"->";
        cout<<"| ";
    }
};



template<class T>
TreeNode<T>::TreeNode(int in_degree,bool in_bool_leaf):
        key_num(0),parent(NULL),next_leaf(NULL),degree(in_degree),bool_leaf(in_bool_leaf)
{
    //将子节点指针，key,val按照degree初始化
    for(int i = 0;i < degree+1;i++){
        ptrs.push_back(NULL);
        keys.push_back(T());
        vals.push_back(int());
    }
    ptrs.push_back(NULL);
}

template<class T>
TreeNode<T>::~TreeNode(){
    //默认析构函数
}

template<class T>
bool TreeNode<T>::isRoot() {
    //根节点没有父亲(NULL)
    return (!parent);
}

template<class T>
bool TreeNode<T>::findKey(T key, int &index) {

    if(key_num == 0){
        index = 0;
        return false;
    } else{
        index = lower_bound(keys.begin(),keys.begin() + key_num,key) - keys.begin();
        //返回的是一个迭代器指针
        if(keys[index] != key) {
            return false;
        }
        else{
            return true;
        }
    }
}

template<class T>
TreeNode<T>* TreeNode<T>::split(T &key) {
    TreeNode* newNode = new TreeNode(this->degree, this->bool_leaf);
    if(newNode == NULL){
        cout<<"Error: can't create a new b+ tree node"<<endl;
        return NULL;
    }
    if(this->bool_leaf){
        //如果这是一个叶子节点
        //最小的值数 ceil( (degree-1)/2 ) -> degree / 2
        int minnum = degree/2;
        key = keys[minnum];
        for(int i = minnum;i< degree;i++){
            //将本节点的后面一段复制到新节点，并清空后面
            newNode->keys[i-minnum] = keys[i];
            keys[i] = T();
            newNode->vals[i-minnum] = vals[i];
            vals[i] = int();
        }
        //本节点next_leaf指向新节点
        newNode->next_leaf = this->next_leaf;
        this->next_leaf = newNode;
        //他们拥有同一个父亲
        newNode->parent = this->parent;
        //修改两个节点的key_num
        newNode->key_num = this->key_num - minnum;
        this->key_num = minnum;
    } else{
        //非叶节点，minmun比叶节点小1
        int minnum = (degree-1)/2;
        key = keys[minnum];
        for (int i = minnum+1; i < degree; ++i) {
            newNode->keys[i - minnum - 1] = this->keys[i];
            this->keys[i] = T();
            newNode->ptrs[i - minnum-1] = this->ptrs[i];
            newNode->ptrs[i - minnum-1]->parent = newNode;
            this->ptrs[i] = NULL;
        }
        this->keys[minnum] = T();
        //在最后一个值的右边还有一个指针，循环里面没有处理
        newNode->ptrs[this->key_num - minnum - 1] = this->ptrs[this->key_num];
        newNode->ptrs[this->key_num - minnum - 1]->parent = newNode;
        this->ptrs[this->key_num] = NULL;
        //更新节点的key_num
        newNode->key_num = this->key_num - minnum-1;
        this->key_num = minnum;
        newNode->parent = this->parent;
    }
    return newNode;
}

template<class T>
int TreeNode<T>::add(T &key) {
    if(key_num == 0){
        keys[0] = key;
        key_num++;
        return 0;
    } else{
        int index = 0;
        bool exist = findKey(key,index);
        if(exist){
            cout<<"Error:Insert an key already exist in the tree!"<<endl;
            return 0;
        } else{
            //不存在，则插入
            auto insert_pos =  lower_bound(keys.begin(),keys.begin() + key_num,key);
            int insert_idx = insert_pos - keys.begin();
            keys.insert(insert_pos,key);
            //在对应值的右边同时插入一个空指针
//            ptrs.insert(insert_pos+1,NULL);
//            和上面等价
//            //将插入值右边的指针向右移动
            for(int ptr_i = key_num+1;ptr_i > insert_idx+1;ptr_i--)
                this->ptrs[ptr_i] = this->ptrs[ptr_i-1];
            this->ptrs[insert_idx+1] = NULL;
            key_num++;
            return insert_idx;
        }
    }
}

template<class T>
int TreeNode<T>::add(T &key, int val) {
    if(!bool_leaf){
        cout<<"Error:can't insert a leaf value into a nonleaf node!"<<endl;
        return -1;
    }
    if(key_num==0){
        keys[0] = key;
        vals[0] = val;
        key_num++;
        return 0;
    } else{
        int index = 0;
        bool exist = findKey(key,index);
        if(exist){
            cout<<"Error:Insert an key already exist in the tree!"<<endl;
            return 0;
        } else {
            auto insert_pos =  lower_bound(keys.begin(),keys.begin() + key_num,key);
            int insert_idx = insert_pos - keys.begin();
            keys.insert(insert_pos,key);
            vals.insert(vals.begin() + insert_idx,val);
            key_num++;

#ifndef ZZHDEBUG
            cout<<"Add "<<key<<"-"<<val<<" at "<< insert_idx<<".\n";
#endif
            return insert_idx;
        }
    }
}

template<class T>
bool TreeNode<T>::remove_at(int index) {
    //如果index超出范围
    if(index > key_num){
        cout<<"Error: out of range in delete tree node key at index: "<<index<<" !"<<endl;
        return false;
    }else{
        if(bool_leaf){
            for(int i = index;i<key_num-1;i++){
                keys[i] = keys[i+1];
                vals[i] = vals[i+1];
            }
            keys[key_num-1] = T();
            vals[key_num-1] = int();
        } else{
            for(int i = index;i<key_num-1;i++)
                keys[i] = keys[i+1];
            for(int i = index+1;i<key_num;i++)
                ptrs[i] = ptrs[i+1];
            keys[key_num-1] = T();
            ptrs[key_num] = NULL;
        }
        key_num--;
        return true;
    }
}

template<class T>
void TreeNode<T>::debug_print() {
    cout<<"keys|";
    for(int i = 0;i<key_num;i++)
        cout<<keys[i]<<"->";
    cout<<"| key_num:"<<key_num;
    if(bool_leaf){
        cout<<" vals|";
        for(int i = 0;i<key_num;i++)
            cout<<vals[i]<<"->";
        cout<<"|\n";
    }
}


#endif //DB_PRO_BPNODE_H
