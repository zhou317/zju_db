#include "bufferManager.h"
#include <bits/stdc++.h>
using namespace std;

Page::Page(){
    buffer = new char[PAGESIZE];//申请buffer空间 
    initialize();//成员变量初始化
}

Page::~Page(){
    delete[](buffer);//释放buffer空间
}

void Page::initialize(){
    file = "";
    blockID = -1;
    dirty = false;
    avaliable = false;
    pin = false;
    clock = false;
}

void Page::setAll(string file, int blockID, bool dirty, bool avaliable, bool pin, bool clock){
    this->file = file;
    this->blockID = blockID;
    this->dirty = dirty;
    this->avaliable = avaliable;
    this->pin = pin;
    this->clock = clock;
}

BufferManager::BufferManager(){
    pages = new Page[MAXPAGENUM];//申请page空间
    pageNum = MAXPAGENUM;
    clockPosition = 0;
}

BufferManager::~BufferManager(){
    flushAllPage();//将所有page写回硬盘
    delete[](pages);//释放page空间
}

bool BufferManager::loadPage(string file, int blockID, int pageID){
    FILE* f = fopen(file.c_str(), "r");
    if (f==NULL) return false;//若无该文件返回false
    fseek(f, PAGESIZE * blockID, SEEK_SET);//根据blockID偏移
    fread(pages[pageID].buffer, PAGESIZE, 1, f);//写入buffer
    fclose(f);
    pages[pageID].setAll(file, blockID, false, true, false, true);//设置page状态
    return true;
}

int BufferManager::requestNewPage(){
    for (int i=0; i<MAXPAGENUM; i++) if (pages[i].avaliable == false) return i;//若有空闲page则直接返回对应pageID
    for (int i=0; i<2*MAXPAGENUM;i++){
        if (pages[clockPosition].clock == true || pages[clockPosition].pin == true) pages[clockPosition].clock = false;//经过最近使用过或锁定page时修改使用状态并跳过
        else{
            if (pages[clockPosition].dirty == true) flushPage(clockPosition);//若page被修改,则将page写回到硬盘
            pages[clockPosition].initialize();//page初始化
            return clockPosition++;
        }
        clockPosition = (clockPosition + 1) % MAXPAGENUM;//时钟向前行进
    }
    return -1;
}

int BufferManager::getPageID(string file, int blockID){
    for (int i=0; i<MAXPAGENUM; i++) if (pages[i].file == file && pages[i].blockID == blockID) return i;//若找到对应page返回pageID
    int newPageID = requestNewPage();//未找到则寻找新page
    if (newPageID == -1) return -2;//返回-2代表无可用page
    if (loadPage(file, blockID, newPageID)) return newPageID;//从硬盘加载page
    return -1;//返回-1代表无该文件
}

char* BufferManager::getPageBuffer(int pageID){
    return pages[pageID].buffer;
}

void BufferManager::setPageDirty(int pageID, bool dirty){
    pages[pageID].dirty = dirty;
}

void BufferManager::setPagePin(int pageID, bool pin){
    pages[pageID].pin = pin;
}

void BufferManager::flushPage(int pageID){
    if (pages[pageID].dirty == false) return;
    FILE* f = fopen(pages[pageID].file.c_str(), "r+");
    fseek(f, PAGESIZE * pages[pageID].blockID, SEEK_SET);//根据blockID偏移
    fwrite(pages[pageID].buffer, PAGESIZE, 1, f);//写入硬盘
    fclose(f);
    pages[pageID].dirty = false;//更新dirty状态 
}

void BufferManager::flushAllPage(){
    for (int i=0; i<MAXPAGENUM; i++) flushPage(i);
}