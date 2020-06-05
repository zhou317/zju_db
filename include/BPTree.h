//
// Created by zzh on 2020/6/3.
//

#ifndef DB_PRO_BPTREE_H
#define DB_PRO_BPTREE_H
#include "BPNode.h"
#include "BPTree.h"
#include "bufferManager.h"
#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <bitset>
#include "config.h"
#include <cstring>

//去掉debug信息
#define ZZHDEBUG
#define ZZHDELETEDEBUG
//#define ZZHINDEXPAGEDEBUG

using namespace std;
extern BufferManager buffer_manager;

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
    //调试输出
    void debug_print();
    void print_bnode(tree_ptr node);
    int getLevel(tree_ptr node);
    void scan_leaf(){
        tree_ptr tmp = leaf_start;
        while(tmp != NULL){
            tmp->debug_print();
            tmp = tmp->next_leaf;
        }
        cout<<endl;
    }

};




template<class T>
BPTree<T>::BPTree(string fname, int key_size, int degree):
        file_name(fname),key_num(0),level(0),node_num(0),
        tree_root(NULL),leaf_start(NULL),key_size(key_size),
        degree(degree)
{
    //初始化分配内存从磁盘读取数据
    //创建索引
#ifndef ZZHDEBUG
    cout<<"Construction of bptree.\n";
#endif
    initTree();
    readFromDisk();
}

template<class T>
BPTree<T>::~BPTree<T>() {
    dropTree(tree_root);
    level = 0;
    key_num = 0;
    level = 0;
}

template<class T>
int BPTree<T>::searchVal(T &key) {
    //根节点为空
    if(!tree_root)
        return -1;
    search_node_parse snp;
    findToLeaf(tree_root,key,snp);
    if(!snp.bool_found)
        return -1;
    else
        return snp.pNode->vals[snp.index];
}

template<class T>
bool BPTree<T>::insertKey(T &key, int val) {
    search_node_parse snp;
    findToLeaf(tree_root,key,snp);
    if(snp.bool_found){
        cout<<"Error: insert a existing key: "<<key
            <<" into B+ tree!\n";
        return false;
    } else{
#ifndef ZZHDEBUG
        cout<<"Insert "<<key<<"-"<<val<<".\n";
#endif
        //不存在,插入
        snp.pNode->add(key,val);
        //key的数量为degree,调整
        if(snp.pNode->key_num == degree){
            adjustAfterInsert(snp.pNode);
        }
        key_num++;
        return true;
    }
}


template<class T>
bool BPTree<T>::adjustAfterInsert(tree_ptr node) {

    T key;
    tree_ptr newNode= node->split(key);
#ifndef ZZHDEBUG
    cout<<"Insert ADJUST.\n";
    node->debug_print();
    newNode->debug_print();
    cout<<"\n";
#endif
    //b+树的节点+1
    node_num++;
    //如果当前是根节点
    if(node->isRoot()){
        tree_ptr root = new TreeNode<T>(degree, false);
        if(root == NULL){
            cout<<"Error: can't create a new node\n";
            return false;
        } else{
            level++;
            node_num++;
            tree_root=root;
            node->parent = tree_root;
            newNode->parent = tree_root;
            root->add(key);
            root->ptrs[0] = node;
            root->ptrs[1] = newNode;
            return true;
        }
    } else{
        tree_ptr parent = node->parent;
        int index = parent->add(key);
        parent->ptrs[index+1] = newNode;
        newNode->parent = parent;
        if(parent->key_num == degree)
            return adjustAfterInsert(parent);
        return true;
    }
}

template<class T>
bool BPTree<T>::deleteKey(T &key) {
    search_node_parse snp;

    findToLeaf(tree_root,key,snp);
    if(!snp.bool_found){
        cout<<"Error:delete an unexist key "<<key
            <<"!\n";
        return false;
    } else{
        if(snp.pNode->isRoot()){
            //如果找到的是根节点,删除
            snp.pNode->remove_at(snp.index);
            key_num--;
            return adjustAfterDelete(snp.pNode);
        } else{

            //如果找到的索引是0，那么：
            //最左值：不存在于内部节点
            //非最左值：存在于内部节点
            if(snp.index==0 && leaf_start != snp.pNode){

                //解决key存在于内部的问题，找到对应的node以及key的index
                //将其key变成该子树中第二小的
                int index = 0;
                tree_ptr tmp_parent = snp.pNode->parent;
                bool found_inBranch = tmp_parent->findKey(key,index);
                while (!found_inBranch){
                    if(tmp_parent->parent)
                        tmp_parent = tmp_parent->parent;
                    else
                        break;
                    found_inBranch = tmp_parent->findKey(key,index);
                }
                tmp_parent->keys[index] = snp.pNode->keys[1];
                snp.pNode->remove_at(0);
                key_num--;
#ifndef ZZHDELETEDEBUG
                cout<<"Delete "<<key<<" at leaf and non leaf node. And tree before adjuct\n";
                debug_print();
#endif
                return adjustAfterDelete(snp.pNode);
            } else{
                //只在叶子里面

                snp.pNode->remove_at(snp.index);
                key_num--;
#ifndef ZZHDELETEDEBUG
                cout<<"Delete "<<key<<" only at leaf node. And tree before adjuct\n";
                //debug_print();
#endif
                return adjustAfterDelete(snp.pNode);
            }
        }
    }
}


template<class T>
bool BPTree<T>::adjustAfterDelete(tree_ptr node) {
    int min_leaf = degree/2;
    int min_branch = (degree-1)/2;
    if(((node->bool_leaf) && node->key_num >= min_leaf)
       ||((!node->bool_leaf)&&node->key_num>=min_branch)){
        return true;
    }
    //如果是根节点
    if(node->isRoot()){
        if(node->key_num > 0)
            return true;
        if(node->bool_leaf){
            //空树

        } else{
            //根节点指向左节点
            tree_root = node->ptrs[0];
            tree_root->parent = NULL;
            delete node;
            level--;
            node_num--;
        }
    }else{
        tree_ptr parent = node->parent;
        tree_ptr sibling = NULL;
        if(node->bool_leaf){
            //是一个叶子节点
            int index = 0;
            parent->findKey(node->keys[0],index);
            if((parent->ptrs[0] != node) && index + 1 == parent->key_num){
                //当前节点是最后一个孩子节点，和他的左兄弟合并
                sibling = parent->ptrs[index];
                if(sibling->key_num > min_leaf){
                    //如果右边的节点大于要求的个数
                    //将其最后一个直接拿过来
                    for(int i = node->key_num;i>0;i--){
                        node->keys[i] = node->keys[i-1];
                        node->vals[i] = node->vals[i-1];
                    }
                    node->keys[0] = sibling->keys[sibling->key_num-1];
                    node->vals[0] = sibling->vals[sibling->key_num-1];
                    sibling->remove_at(sibling->key_num-1);
                    node->key_num++;
                    //更新父节点的key值
                    parent->keys[index] = node->keys[0];
                    return true;
                }else{

                    //将两个节点合并
                    parent->remove_at(index);
                    for(int i = 0;i < node->key_num;i++){
                        sibling->keys[sibling->key_num+i] = node->keys[i];
                        sibling->vals[sibling->key_num+i] = node->vals[i];
                    }
                    sibling->key_num += node->key_num;
                    sibling->next_leaf = node->next_leaf;
                    delete node;
                    node_num--;
#ifndef ZZHDELETEDEBUG
                    cout<<"Adjust merge left sibling leaf:\n";
                    debug_print();
#endif
                    return adjustAfterDelete(parent);
                }
            }
            else{
                //与右边的兄弟节点合并
                if(node == parent->ptrs[0])
                    sibling = parent->ptrs[1];
                else
                    sibling = parent->ptrs[index+2];
                if(sibling->key_num > min_leaf){
                    node->keys[node->key_num] = sibling->keys[0];
                    node->vals[node->key_num] = sibling->vals[0];
                    node->key_num++;
                    sibling->remove_at(0);
                    if(node == parent->ptrs[0])
                        parent->keys[0] = sibling->keys[0];
                    else
                        parent->keys[index+1] = sibling->keys[0];
                    return true;
                } else{
                    //两个节点合并
                    for(int i = 0;i < sibling->key_num;i++){
                        node->keys[node->key_num+i] = sibling->keys[i];
                        node->vals[node->key_num+i] = sibling->vals[i];
                    }
                    if(node == parent->ptrs[0])
                        parent->remove_at(0);
                    else
                        parent->remove_at(index+1);
                    node->key_num += sibling->key_num;
                    node->next_leaf = sibling->next_leaf;
                    delete sibling;
                    node_num--;
#ifndef ZZHDELETEDEBUG
                    cout<<"Adjust merge right sibling leaf:\n";
                    debug_print();
#endif
                    return adjustAfterDelete(parent);
                }

            }
        }
        else{
            //非叶节点
            int index = 0;
            //如果这个非叶节点删掉一个值之后没有key了（degree = 3的情况）
            //不能按照key搜索，遍历找到index
//            if(node->key_num == 0){
//                for(index = 0;index <= parent->key_num;index++)
//                {
//                    if(node->parent->ptrs[index] == node)
//                        break;
//                }
//
//            } else{
//                parent->findKey(node->keys[0],index);
//            }
            tree_ptr tmp = node->ptrs[0];
            while(tmp->ptrs[0] != NULL) tmp = tmp->ptrs[0];
            parent->findKey(tmp->keys[0],index);
#ifndef ZZHDELETEDEBUG
            cout<<"Adjust nonleaf , index:"<<index<<"\n";
#endif
            if((parent->ptrs[0] != node) && index + 1 == parent->key_num){

                //在最右边，选择左边的兄弟合并
                sibling = parent->ptrs[index];
                if(sibling->key_num > min_branch){
                    //从兄弟哪里拿一个
                    node->ptrs[node->key_num + 1] = node->ptrs[node->key_num];
                    for(int i = node->key_num;i>0;i--){
                        node->ptrs[i] = node->ptrs[i-1];
                        node->keys[i] = node->keys[i-1];
                    }
                    node->ptrs[0] = sibling->ptrs[sibling->key_num];
                    //由于其向右移动了一位，该节点的最左边的key值是原来
                    //以该节点为根的子树的最左值
                    //即存在于父亲节点的index处的值
                    node->keys[0] = parent->keys[index];
                    node->key_num++;
                    //父节点index处key改为新子树的最小值
                    parent->keys[index] = sibling->keys[sibling->key_num-1];
                    node->ptrs[0]->parent = node;
                    sibling->remove_at(sibling->key_num-1);
                    return true;
                }else{

                    sibling->keys[sibling->key_num] = parent->keys[index];
                    parent->remove_at(index);
                    sibling->key_num++;
                    int tmpc = sibling->key_num;
                    for(int i = 0;i<node->key_num;i++){
                        sibling->ptrs[i+tmpc] = node->ptrs[i];
                        sibling->keys[i+tmpc] = node->keys[i];
                        sibling->ptrs[i+tmpc]->parent = sibling;
                    }
                    sibling->ptrs[tmpc+node->key_num] = node->ptrs[node->key_num];
                    sibling->ptrs[tmpc+node->key_num]->parent = sibling;
                    sibling->key_num += node->key_num;
#ifndef ZZHDELETEDEBUG
                    cout<<"Adjust merge left sibling non leaf:\n";
                    debug_print();
#endif
                    delete node;
                    node_num--;
                    return adjustAfterDelete(parent);
                }
            }
            else{
                //选择右边的节点
                if(parent->ptrs[0] == node)
                    sibling = parent->ptrs[1];
                else
                    sibling = parent->ptrs[index+2];
                if(sibling->key_num > min_branch){
                    //右边拿一个给左边
                    node->ptrs[node->key_num+1] = sibling->ptrs[0];
                    if(parent->ptrs[0] == node){
                        node->keys[node->key_num] = parent->keys[0];
                        parent->keys[0] = sibling->keys[0];
                    } else{
                        node->keys[node->key_num] = parent->keys[index+1];
                        parent->keys[index+1] = sibling->keys[0];
                    }
                    node->ptrs[node->key_num+1]->parent = node;
                    node->key_num++;
                    sibling->ptrs[0] = sibling->ptrs[1];
                    sibling->remove_at(0);
                    return true;
                } else{
#ifndef ZZHDEBUG
                    cout<<"Adjust merge right sibling non leaf:\n";
                    debug_print();
#endif
                    if(parent->ptrs[0] == node){
                        node->keys[node->key_num] = parent->keys[0];
                        parent->remove_at(0);
                    } else{
                        node->keys[node->key_num] = parent->keys[index+1];
                        parent->remove_at(index+1);
                    }
                    node->key_num++;
                    int tmpc = node->key_num;
                    for(int i = 0;i < sibling->key_num;i++){
                        node->ptrs[tmpc+i] = sibling->ptrs[i];
                        node->keys[tmpc+i] = sibling->keys[i];
                        node->ptrs[tmpc+i]->parent = node;
                    }
                    node->ptrs[tmpc+sibling->key_num] = sibling->ptrs[sibling->key_num];
                    node->ptrs[tmpc+sibling->key_num]->parent = node;

                    node->key_num += sibling->key_num;
#ifndef ZZHDEBUG
                    cout<<"----Adjust merge right sibling non leaf:\n";
                    debug_print();
#endif
                    delete sibling;
                    node_num--;
                    return adjustAfterDelete(parent);
                }
            }
        }
    }
}

template<class T>
void BPTree<T>::findToLeaf(tree_ptr node, T key, search_node_parse &snp) {
    int index = 0;

    //在当前节点查找
    if(node->findKey(key,index)){
#ifndef ZZHDEBUG
        cout<<"FINDtoLeaf found "<<key<<".\n";
#endif
        //找到
        if(node->bool_leaf){
            snp.pNode = node;
            snp.index = index;
            snp.bool_found = true;
        } else{
            //非叶子节点 pi keyi pi+1
            //在pi+1子树的最左节点
            node = node->ptrs[index+1];
            while (!node->bool_leaf){
                node = node->ptrs[0];
            }
            snp.pNode = node;
            snp.index = 0;
            snp.bool_found = true;
        }
    } else{
        //在这一层里面没有
        if(node->bool_leaf){
            //已经是叶子节点，不存咋
            snp.pNode = node;
            snp.index = index;
            snp.bool_found = 0;
        } else {
            //非叶子节点 pi (search-key) < keyi pi+1
            //                   ^
            //                   |
            //                  index
            //在pi指针里面查找
            findToLeaf(node->ptrs[index],key,snp);
        }
    }
    return;
}


//将索引文件加载到page里面，并构造b+树
int getBlockNum(string fileName){
#ifndef ZZHDEBUG
    cout<<"getBlockNum: ";
#endif
    char*p;
    int block_num = -1;
    do{
        int pgid = buffer_manager.getPageID(fileName,block_num+1);
        p = buffer_manager.getPageBuffer(pgid);
        block_num++;
        //一页页地加载文件，直到最后一个页是空的
    }while (p[0] != 0);
    //返回页数（除去最后一个空页）
    return block_num;
}

template<class T>
void BPTree<T>::readFromDisk() {

    string filePath = "..\\database\\index\\"+file_name;
#ifndef ZZHDEBUG
    cout<<"Read index from file: "<<filePath<<endl;
#endif
    getFile(filePath);
#ifndef ZZHDEBUG
    cout<<"File has "<<block_num<<" blocks.\n";
#endif
    FILE * pFile;
    long size;
    pFile = fopen (filePath.c_str(),"r");
    if (pFile==NULL)
        perror ("Error opening file");
    else
    {
        fseek (pFile, 0, SEEK_END);   ///将文件指针移动文件结尾
        size=ftell (pFile); ///求出当前文件指针距离文件开始的字节数
        fclose (pFile);
    }
    if(size == 0) return;//并没有索引
    //文件需要多少个page
    int block_num = size / PAGESIZE + 1;
    for(int i = 0;i < block_num;i++){

        int pgid = buffer_manager.getPageID(filePath,i);
        char* p = buffer_manager.getPageBuffer(pgid);
        readFromDiskBlock(p,p+PAGESIZE);
    }
}

template<class T>
void BPTree<T>::dropTree(tree_ptr node) {
    if(!node){
        return;
    }
    if(!node->bool_leaf){
        for(int i = 0;i <= node->key_num;i++){
            dropTree(node->ptrs[i]);
            node->ptrs[i] = NULL;
        }
    }
    delete node;
    node_num--;
    return;
}

bool zero_key = true;
template<class T>
void BPTree<T>::readFromDiskBlock(char *s, char *e) {

    int value_size = sizeof(int);
    T key;
    int val;
    //遍历page,向树插入key,val对
    //当还在这一页，且未到达末尾
    char * ts = s;
    int total_size = key_size+value_size;
    while (ts + total_size < e){
        //得到key,val
        key = *(T*)ts;
        val = *(int*)(ts+key_size);
        ts += total_size;

        if((key == 0 && val == 0)){
            if(key_size == 4 && zero_key){
                zero_key = false;
                //判断是的确插入0-0 还是page里面的空值
                //如果是float和int，且key val = 0
                insertKey(key,val);
            }else{
                //是字符串类型
            }
        }else{
            insertKey(key,val);
        }
    }
}

void clearFile(string filePath) {
    //以写的方式打开而清空
    FILE *f = fopen(filePath.c_str(),"w+");
    fclose(f);
    return;
}

//将索引写入文件
template<class T>
void BPTree<T>::writtenBackDisk() {
    string filePath = "..\\database\\index\\"+file_name;
    clearFile(filePath);
    int block_num = getBlockNum(filePath);
    int value_size = sizeof(int);
    tree_ptr leaf_tmp = leaf_start;
    int j = 0;
    int i = 0;
    int search_key_per_page = PAGESIZE / (value_size + key_size);
#ifndef ZZHINDEXPAGEDEBUG
    cout<<"search-key per page"<<search_key_per_page<<endl;
#endif
    //获得一个page
    int pgid = buffer_manager.getPageID(filePath,j++);
    char* p = buffer_manager.getPageBuffer(pgid);
    memset(p,0,PAGESIZE);
    //当前页中的写入位置
    int offset = 0;
    while(leaf_tmp != NULL){
        //如果这一个page里面放满了(指无法再放入一个叶节点中所有的key
        if(i + leaf_tmp->key_num >= search_key_per_page){
            //老的pg设成已经修改
            buffer_manager.setPageDirty(pgid, true);
            //获取一个新的page,将写入位置重新置为0
            pgid = buffer_manager.getPageID(filePath,j++);
            p = buffer_manager.getPageBuffer(pgid);
            memset(p,0,PAGESIZE);
            offset = 0;
            i = 0;
        }
        //向page中写入
        for(int k = 0;k < leaf_tmp->key_num;k++){
            char*key = (char*)&(leaf_tmp->keys[k]);
            char*val = (char*)&(leaf_tmp->vals[k]);
#ifndef ZZHINDEXPAGEDEBUG
            cout<<"write into "<<leaf_tmp->keys[k]<<"-"<<leaf_tmp->vals[k]<<endl;
            cout<<"write into page "<<pgid<<" and offset is "<<offset<<" adddr: "<<(void*)(p+offset)<<" num "<<i<<endl;
#endif
            memcpy(p+offset,key,key_size);
            offset += key_size;
            memcpy(p+offset,val,value_size);
            offset += value_size;
            //向page中写入的serch_key加1
            i++;
        }
        leaf_tmp = leaf_tmp->next_leaf;
    }
    //将当前的页设置为脏页
    buffer_manager.setPageDirty(pgid, true);
}

template<class T>
void BPTree<T>::initTree() {
    tree_root = new TreeNode<T>(degree, true);
    key_num=0;
    level=1;
    node_num=1;
    leaf_start=tree_root;
#ifndef ZZHDEBUG
    cout<<"Init Tree.\n";
    this->debug_print();
#endif
    return;
}


template<class T>
void BPTree<T>::getFile(string filePath) {
    //如果文件存在，不做啥
    //不存在，新建一个
    FILE *f = fopen(filePath.c_str(),"r");
    if(!f){
        f = fopen(filePath.c_str(),"w+");
        fclose(f);
        f = fopen(filePath.c_str(),"r");
    }
    fclose(f);
    return;
}
template<class T>
int BPTree<T>::getLevel(tree_ptr node){
    int i = 0;
    tree_ptr tmp = node;
    while(tmp->parent != NULL){
        i++;
        tmp = tmp->parent;
    }
    return i;
}


template<class T>
void BPTree<T>:: print_bnode(tree_ptr node){

    cout<< "node :" << (void*)node;
    if(node->parent != NULL)    cout<< " parent: " << (void * ) node->parent;
    node->debug_print();
    cout<<endl;
    if(!node->bool_leaf){
        for(int i = 0;i <= node->key_num;i++) {
            int tmp_level = getLevel(node);
            for(int i = 0;i <= tmp_level;i++)
                cout<<"----";
            print_bnode(node->ptrs[i]);
        }
    }

}

template<class T>
void BPTree<T>::debug_print() {
    cout << "############DEBUG FOR THE TREE############" << endl;
    cout<<"name:" << file_name << " root:" << (void*)tree_root
        << " leafHead:" << (void * )leaf_start << " keycount:" <<
        key_num << " level:" << level << " nodeCount:" << node_num << endl;
    if(tree_root){
        print_bnode(tree_root);
    }
    cout<<"############END OF DEBUG PRINT FOR B+ TREE#########"<<endl;
}


#endif //DB_PRO_BPTREE_H
