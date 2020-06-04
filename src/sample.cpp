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

#include <bits/stdc++.h>
#include "bufferManager.h"
using namespace std;

int main(){
    vector<int> v;
    int num_to_find=25;
    //要查找的元素,类型要与vector<>类型一致
    //
    for(int i=0;i<10;i++)v.push_back(i*2);
    cout<<lower_bound(v.begin(),v.end(),12) - v.begin() <<endl;
    v.insert(lower_bound(v.begin(),v.end(),12) + 1,11);

    for (int j = 0; j < 11; ++j) {
        cout<<v[j]<<" ";
    }
}


//todo: buffermanager 测试
//
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