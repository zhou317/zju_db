#include <bits/stdc++.h>
#include "bufferManager.h"
#include "BPTree.h"
#include <ctime>
#include "catalogManager.h"
#include "IndexManager.h"

BufferManager buffer_manager;
CatalogManager catalog;
IndexManager idxMana(catalog);
using namespace std;

//todo:索引管理测试

int main(){

    return 1;
}




////todo:catalog 测试
//int main()
//{
//    CatalogManager catalog;
//
//    //展示操作
//    catalog.show_Table();
//    catalog.show_Index();
//
//    //是否存在
//    cout << catalog.has_Table("student") << "    " << catalog.has_Table("book") << endl;
//    //前者存在，后者不存在
//
//    cout << catalog.has_Index("student", "id") << "    " << catalog.has_Index("index_id") << endl;
//    //两者都存在
//
//    cout << catalog.has_Attribute("student2", "id") << endl;
//    //存在
//
//    Attribute A("class", 1, 10, 0, 0);
//
//    //建立操作
//    try {
//        catalog.create_Attribute("student", A);
//    }
//    catch (TableExpection e) {
//        cout << e.what() << endl;
//    }
//    //catalog.show_Table(); 成功插入
//
//    cout<<" ================ "<<endl;
//    catalog.show_Table();
//
//
//    vector<Attribute> car;
//
//    Attribute B("id", 1, 10, 1, 1); car.push_back(B);
//    Attribute C("onwer", 2, 10, 0, 0); car.push_back(C);
//    Attribute D("price", 3, 10, 0, 0); car.push_back(D);
//    Table Car("car", 3, 20, car);
//
//    try {
//        catalog.create_Table(Car);
//    }
//    catch (TableExpection e) {
//        cout << e.what() << endl;
//    }
//    //成功插入
//    cout<<" ================ table car created"<<endl;
//    catalog.show_Table();
//
//    try {
//        catalog.create_Table(Car);
//    }
//    catch (TableExpection e) {
//        cout << e.what() << endl;
//    }
//    //再次尝试插入，报错，表重复
//
//    Index bid("book", "id", "index_bid", 30);
//
//    try {
//        catalog.create_Index(bid);
//    }
//    catch (TableExpection e) {
//        cout << e.what() << endl;
//    }
//    cout<<"=============create a new index on book"<<endl;
//    catalog.show_Index();
//    //更新操作
//
//    Attribute E("name", 2, 44, 0, 0);
//
//    try {
//        catalog.update_Attribute("student", E);
//    }
//    catch (TableExpection e) {
//        cout << e.what() << endl;
//    }
//    //成功更新
//    cout<<" ================ update tabel "<<endl;
//    catalog.show_Table();
//
//
//    car.push_back(E);
//    Table car_new("car", 4, 20, car);
//
//    try {
//        catalog.update_Table(car_new);
//    }
//    catch (TableExpection e) {
//        cout << e.what() << endl;
//    }
//    //成功更新
//    cout<<" ================ update a table "<<endl;
//    catalog.show_Table();
//
//    Index index_ID("student", "id", "index_id", 21);
//    try {
//        catalog.update_Index(index_ID);
//    }
//    catch (TableExpection e) {
//        cout << e.what() << endl;
//    }
//    cout<<"=============create a new index"<<endl;
//    catalog.show_Index();
//    //删除操作
//
//    try {
//        catalog.drop_Table("car");
//    }
//    catch (TableExpection e) {
//        cout << e.what() << endl;
//    }
//    //成功删除
//
//    try {
//        catalog.drop_Table("car");
//    }
//    catch (TableExpection e) {
//        cout << e.what() << endl;
//    }
//    //再次尝试删除，报错，表不存在
//
//
//    try {
//        catalog.drop_Atttribute("student","class");
//    }
//    catch (TableExpection e) {
//        cout << e.what() << endl;
//    }
//    //成功删除
//
//    try {
//        catalog.drop_Index("index_bid");
//    }
//    catch (TableExpection e) {
//        cout << e.what() << endl;
//    }
//
//
//    //查询操作
//    try {
//        Table p = catalog.get_Table("student");
//        cout << p.name << endl;
//    }
//    catch (TableExpection e) {
//        cout << e.what() << endl;
//    }
//    try {
//        Attribute q = catalog.get_Attribute("student", "id");
//        cout << q.name << endl;
//    }
//    catch (TableExpection e) {
//        cout << e.what() << endl;
//    }
//
//    try {
//        Index o = catalog.get_Index("student","id");
//        cout << o.index_name << endl;
//    }
//    catch (TableExpection e) {
//        cout << e.what() << endl;
//    }
//
//    return 0;
//}



//todo:B+树磁盘读取测试
//
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
//    intBPTree->scan_leaf();
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
//    for(int i = 2;i < 10000;i+=2){
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