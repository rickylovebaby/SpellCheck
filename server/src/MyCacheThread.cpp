//
// Created by Ricky on 7/7/2017.
//

#include "MyCacheThread.h"
#include "ThreadPool.h"
#include "MyThread.h"
#include "MyCache.h"
#include <unistd.h>
#include <fstream>

void MyCacheThread::run()
{
    while(true)
    {
        /*
         * 扫描线程每过60s就一次同步工作线程的内存cache与硬盘cache
         * */
        sleep(60);
        scan_cache();
        std::cout << "Scan cache" << std::endl;
        
    
    }
}

//扫描线程对象持有线程池对象的指针
/*
 * 线程池初始化时，会调用get_related（）function ，使该扫描线程对象持有
 * 线程池对象的指针，并使得该扫描对象持有线程池中的所有工作对象的指针
 *
 * */
void MyCacheThread::get_related(ThreadPool* threadPoolPtr)
{
    _threadPoolPtr = threadPoolPtr;
    std::vector<MyThread>::iterator iter1 = (_threadPoolPtr->_vecThread).begin();//存储线程池对象中的工作线程对象的容器迭代器
    std::vector<MyThread*>::iterator iter2 = _vecWorkThreadPtr.begin();//指向工作线程指针的容器迭代器
    
    
    while(iter2 != _vecWorkThreadPtr.end() && iter1 != (_threadPoolPtr->_vecThread).end())
    {
        *iter2 = &(*iter1);
        iter1++;
        iter2++;
    }
    
}


//同步每一个工作线程的内存cache与硬盘cache
void MyCacheThread::scan_cache()
{
    std::vector<MyThread*>::iterator iter = _vecWorkThreadPtr.begin();
    for(; iter != _vecWorkThreadPtr.end();++iter) {
        ((*iter)->_cache).read_from_file((_threadPoolPtr->_conf).getMapConf()["cache"].c_str());
    
        std::ofstream outfile((_threadPoolPtr->_conf).getMapConf()["cache"].c_str());
    
        if (!outfile) {
            throw std::runtime_error("Scan cache: open cache failed");
        }
    
        ((*iter)->_cache).write_to_file(outfile);
    
        outfile.close();
    }
}