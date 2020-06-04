//
// Created by zzh on 2020/6/4.
//
#include "BPTree.h"
#include "bufferManager.h"
#include <algorithm>
#include <istream>
#include <vector>

template<class T>
BPTree<T>::BPTree(string fname, int key_size, int degree) {

}

template<class T>
BPTree<T>::~BPTree<T>() {

}

template<class T>
int BPTree<T>::searchVal(T &key) {

}

template<class T>
bool BPTree<T>::insertKey(T &key, int val) {

}

template<class T>
bool BPTree<T>::deleteKey(T &key) {

}

template<class T>
void BPTree<T>::readFromDisk() {

}

template<class T>
void BPTree<T>::dropTree(tree_ptr node) {

}

template<class T>
void BPTree<T>::readFromDiskBlock(char *s, char *e) {

}

template<class T>
void BPTree<T>::writtenBackDisk() {

}

template<class T>
void BPTree<T>::initTree() {

}

template<class T>
bool BPTree<T>::adjustAfterInsert(tree_ptr node) {

}

template<class T>
bool BPTree<T>::adjustAfterDelete(tree_ptr node) {

}

template<class T>
void BPTree<T>::findToLeaf(tree_ptr node, T key, search_node_parse &snp) {

}

template<class T>
void BPTree<T>::getFile(string filePath) {

}

template<class T>
int BPTree<T>::getBlockNum(string table_name) {

}