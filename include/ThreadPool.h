//
// Created by Ricky on 7/7/2017.
//

#ifndef SPELLCHECK_THREADPOOL_H
#define SPELLCHECK_THREADPOOL_H

#include "MyThread.h"
#include "MyLock.h"
#include "MyTask.h"
#include <vector>
#include <string>
#include <queue>
#include <map>
#include "MyConf.h"
#include "MyCacheThread.h"
#include "MyCondition.h"


class ThreadPool{
public:
    friend class MyCacheThread;
    
    ThreadPool(MyConf& conf, int size = 12)
            :_vecThread(size),
             _queueTaskLock(),
             _queueTasksCond(_queueTaskLock),
             _isStarted(false),
             _conf(conf),
             _cacheThread(size)
    {
        std::vector<MyThread>::iterator iter;
        for(iter = _vecThread.begin();iter != _vecThread.end();++iter)
        {
            iter->get_related(this);//使线程池中的每一个工作线程都持有线程池对象的指针
        }
        _cacheThread.get_related(this);//使线程池中的扫描线程持有线程池对象的指针
    
    }
    
    //开启线程池
    void on(){
        if(_isStarted)
            return;
        _isStarted = true;
        std::vector<MyThread>::iterator iter;
        for(iter = _vecThread.begin(); iter != _vecThread.end();++iter)
        {
            iter->start();  //开启工作现场
        }
        _cacheThread.start(); //开始扫描线程
    }
    
    //关闭线程池
    void off(){
        if(_isStarted)
        {
            _isStarted = false;
            _queueTasksCond.broadcast();
            while(!_queueTasks.empty())
            {
                _queueTasks.pop();
            }
        }
    }
    //分配任务
    void allocate_task(MyTask& task)
    {
        _queueTaskLock.lock();
        std::cout << "Add task" << std::endl;
        _queueTasks.push(task);
        _queueTaskLock.unlock();
        _queueTasksCond.broadcast();
    }
    
    //获取任务
    bool get_task(MyTask& task){
        _queueTaskLock.lock();
        while(_isStarted && _queueTasks.empty())
        {
            _queueTasksCond.wait();
        }
        if(!_isStarted){
            _queueTaskLock.lock();
            _queueTasksCond.broadcast();
            return false;
        }
        
        task = _queueTasks.front();
        _queueTasks.pop();
        _queueTaskLock.unlock();
        _queueTasksCond.broadcast();
        std::cout << "get task"<<std::endl;
        
        return true;
    }
    
    
    MyConf& _conf;//配置对象的引用
            
private:
    //禁止复制和赋值
    ThreadPool(const ThreadPool& obj);
    ThreadPool& operator=(const ThreadPool& obj);
    
    std::vector<MyThread> _vecThread;//存放工作线程的容器
    std::queue<MyTask> _queueTasks;//存放任务的队列
    
    MyLock _queueTaskLock;
    MyCondition _queueTasksCond;
    
    bool _isStarted; //用于标识线程池是否开启
    
    MyCacheThread _cacheThread;         //定时扫描内存cache的线程
    
    
};



#endif //SPELLCHECK_THREADPOOL_H
