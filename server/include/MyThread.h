//
// Created by Ricky on 7/7/2017.
//

/*
 * 工作线程
 *
 * 1.工作线程持有线程池对象的指针，以及自身的内存cache：
 *      工作线程的任务就是不断地从线程池的任务队列中取出任务，当工作线程持有线程池的指针时，就可以调用线程池对象的get_task()方法
 *      然后取出里面的任务，持有自身的内存cache是为了当工作线程进行查询匹配时，首先会从自身的内存cache中进行查找
 *
 * */

#ifndef SPELLCHECK_MYTHREAD_H
#define SPELLCHECK_MYTHREAD_H


class ThreadPool;

//继承抽象类Thread

class MyThread : public Thread{
public:
    //与线程池对象关联
    void get_related(ThreadPool* p)
    {
        _threadPoolPtr = p;
    }
    
private:
    void run();
    
    ThreadPool* _threadPoolPtr;
    
    MyCache _cache;
    
    friend class MyCacheThread;
};

#endif //SPELLCHECK_MYTHREAD_H
