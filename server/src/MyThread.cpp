//
// Created by Ricky on 7/7/2017.
//

#include <iostream>
#include "MyThread.h"
#include "ThreadPool.h"

void MyThread::run()
{
    std::cout << "run "<<std::endl;
    //当工作线程启动时，会将硬盘的缓存文件更新到工作线程的内存cache中
    _cache.read_from_file((_threadPoolPtr->_conf).getMapConf()["cache"].c_str());
    //从任务队列中取任务，然后执行
    while(true){
        MyTask task(_threadPoolPtr->_conf);
        if(!(_threadPoolPtr->get_task(task)))
        {
            break;
        }
        task.excute(_cache);//执行任务，先在cache中寻找，未找到再去词频字典查询最佳匹配单词
    }
}