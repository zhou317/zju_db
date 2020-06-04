# ifndef _BUFFERMANAGER_H_
# define _BUFFERMANAGER_H_ 1

#include "config.h"
#include <string>
using namespace std;

class Page{
    public:
        char* buffer;
        //page的buffer指针
        string file;
        //page对应的file
        int blockID;
        //page对应的file+blockID
        bool dirty;
        //page是否被修改过,true->dirty
        bool avaliable;
        //page是否被使用,true->在被使用
        bool pin;
        //page是否被锁住,true->被锁住
        bool clock;
        //page是否被使用过,true->在一个时钟周期内被使用过

        Page();
        //构造函数
        ~Page();
        //析构函数
        void initialize();
        //初始化函数,dirty=avaliable=pin=clock=false
        void setAll(string file, int blockID, bool dirty, bool avaliable, bool pin, bool clock);
        //设置所有成员变量
};

class BufferManager{
    private:
        Page* pages;
        //page指针数组
        int pageNum;
        //page数量=1000
        int clockPosition;
        //时钟替换策略现在位置

        bool loadPage(string file, int blockID, int pageID);
        //将file+blockID的数据读入到对应的page中
        //false表示未找到对应file
        int requestNewPage();
        //根据时钟替换策略寻找新的page

    public:
        BufferManager();
        //构造函数
        ~BufferManager();
        //析构函数
        int getPageID(string file, int blockID);
        //获取file+blockID对应的pageID,标号为0-999
        //-2表示没有可以释放的page,-1表示未找到对应file
        char* getPageBuffer(int pageID);
        //获取page的buffer指针,长度为char[4096]
        void setPageDirty(int pageID, bool dirty);
        //设置page是否dirty,true代表dirty
        void setPagePin(int pageID, bool pin);
        //设置page是否pin,true代表已被钉住
        void flushPage(int pageID);
        //刷新page并设置dirty为false
        void flushAllPage();
        //刷新所有page并设置dirty为false
};

#  endif