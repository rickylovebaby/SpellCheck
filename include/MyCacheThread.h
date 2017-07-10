//
// Created by Ricky on 7/7/2017.
//
/*
 * 扫描线程持有线程池对象的指针：
 *      线程池对象拥有配置文件对象的引用，配置文件对象中拥有硬盘cache
 * 扫描线程持有每一个工作线程对象的指针：
 *      便于完成工作线程的内存cache与硬盘cache的
 *
 *
 * */
#ifndef SPELLCHECK_MYCACHETHREAD_H
#define SPELLCHECK_MYCACHETHREAD_H
#include "Thread.h"
#include "ThreadPool.h"
#include <vector>
class ThreadPool;
class MyCache;
class MyThread;

class MyCacheThread : public Thread{
public:
    MyCacheThread(const int& num = 12)
            : Thread(),_vecWorkThreadPtr(num){}
    
    void get_related(ThreadPool* threadPoolPtr);
    
    
private:
    void run();
    void scan_cache();
    
    ThreadPool* _threadPoolPtr;               //指向线程池的指针
    std::vector<MyThread*> _vecWorkThreadPtr;//含有指向工作线程指针的vector
};




#endif //SPELLCHECK_MYCACHETHREAD_H
