#include <bits/stdc++.h>
#include "bufferManager.h"
#include "BPTree.h"
#include<ctime>

BufferManager buffer_manager;
using namespace std;

//int main(){
//    clock_t start,end;
//    start=clock();
//    BPTree<int> *intBPTree =
//            new BPTree<int>("test.idx",sizeof(int),8);
//    end=clock();		//程序结束用时
//    double endtime=(double)(end-start)/CLOCKS_PER_SEC;
//    cout<<"Total time:"<<endtime<<endl;		//s为单位
//    cout<<"Total time:"<<endtime*1000<<"ms"<<endl;	//ms为单位
////    intBPTree->debug_print();
////    intBPTree->scan_leaf();
//}

//
// //todo:B+树测试
//int main(){
//    clock_t start,end;
//    start=clock();
//    BPTree<int> *intBPTree =
//            new BPTree<int>("test.idx",sizeof(int),8);
//    for(int i = 0;i < 10000;i++){
//        intBPTree->insertKey(i,i);
//    }
////    intBPTree->debug_print();
////    //如果输出是线性的，基本上b+树没有问题
////    intBPTree->scan_leaf();
//    int tmp = 5;
//    cout<<"test for search value from key(5):"<<intBPTree->searchVal(tmp)<<endl;
//    for(int i = 0;i < 10000;i+=2){
//        intBPTree->deleteKey(i);
//    }
////    intBPTree->debug_print();
////    intBPTree->scan_leaf();
//    intBPTree->writtenBackDisk();
//    end=clock();		//程序结束用时
//    double endtime=(double)(end-start)/CLOCKS_PER_SEC;
//    cout<<"Total time:"<<endtime<<endl;		//s为单位
//    cout<<"Total time:"<<endtime*1000<<"ms"<<endl;	//ms为单位
////    output:
////    test for search value from key(5):5
////    Total time:0.034
////    Total time:34ms
//}


//todo: buffermanager 测试
//int getPageID(string file, int blockID);
//获取file+blockID对应的pageID,标号为0-999
//-2表示没有可以释放的page,-1表示未找到对应file

//char* getPageBuffer(int pageID);
//获取page的buffer指针,长度为char[4096]

//void setPageDirty(int pageID, bool dirty);
//设置page是否dirty,true代表dirty

//void setPagePin(int pageID, bool pin);
//设置page是否pin,true代表已被钉住

//void flushPage(int pageID);
//刷新page并设置dirty为false

//void flushAllPage();
//刷新所有page并设置dirty为false
//int main(){
//    int res;
//    int page1,page2;
//    char *buffer1, *buffer2;
//    BufferManager bufferManager;
//
//    res = bufferManager.getPageID("notsample", 0);
//    cout << "result of get page with wrong file: " << res << endl;
//
//
//    for (int i=0;i<MAXPAGENUM;i++) {
//        bufferManager.getPageID("..\\input\\buffer_test.txt", i+2);
//        bufferManager.setPagePin(i,true);
//    }
//    res = bufferManager.getPageID("..\\input\\buffer_test.txt", 0);
//    cout << "result of get page when all pages pin: " << res << endl;
//
//    bufferManager.setPagePin(5,false);
//
//    page1 = bufferManager.getPageID("..\\input\\buffer_test.txt", 0);
//    cout<<"get page: "<<page1<<endl;
//    buffer1 = bufferManager.getPageBuffer(page1);
//    strcpy(buffer1,"hello, Im buffer1.");
//    bufferManager.setPageDirty(page1,true);
//
//    page2 = bufferManager.getPageID("..\\input\\buffer_test.txt", 1);
//    cout<<"get page: "<<page2<<endl;
//    buffer2 = bufferManager.getPageBuffer(page2);
//    strcpy(buffer2,"hello, Im buffer2.");
//    bufferManager.setPageDirty(page2,true);
//
//    bufferManager.flushAllPage();
//    cout << "two modify with one page can be used" << endl << "check sample after flush" << endl;
//
//    return 0;
//}