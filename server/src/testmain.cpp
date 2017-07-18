//
// Created by Ricky on 7/7/2017.
//

#include "ThreadPool.h"
#include "MySocket.h"
#include "MyConf.h"
#include "MyCache.h"

int main(int argc, char* argv[]){
    
    /*
     * 初始化配置文件，MyConf会根据读入的配置文件，生成词频字典以及倒排索引
     *
     * */
    MyConf conf(argv[1]);
    
    //初始化线程池对象
    ThreadPool apool(conf);
    
    //初始化用于UDP通信的socket对象
    MySocket socket(conf);
    
    //开启线程中的工作线程以及cache扫描线程
    apool.on();
    
    
    const int len  = 1024;
    char buf[len];
    int iret;
    
    //不断接收客户端UDP的数据
    while(true){
        memset(buf,0,len);
        iret = socket.recv_message(buf,len);
        std::cout << "main: "<< buf<<"len: "<<iret<<std::endl;
        
        //将客户端的查询词和地址封装成task放入线程中的任务队列
        //工作线程将会从任务队列中取出任务行，执行完直接由工作线程返回结果给客户端
        MyTask task(buf,socket.get_addr(),conf);
        apool.allocate_task(task);
    }
    apool.off();
    return 0;
}