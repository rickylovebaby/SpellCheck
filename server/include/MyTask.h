//
// Created by Ricky on 7/7/2017.
//

/*
 * Task类
 *1.工作线程从线程池中的任务队列取出的人物是一个“任务对象”，之后执行任务task.excute(_cache);
 *
 *2.计算逻辑以及返回给客户端的结果，都是task对象进行的
 *
 *3.工作线程中持有自身内存的cache，在执行任务时，需要将工作线程cache的引用传给任务的excute()方法
 *
 *4.任务对象在进行计算编辑距离时，需要词频字典、倒排索引，所以初始化一个任务对象，需要传入配置文件对象的引用
 *
 *
 * */

#ifndef SPELLCHECK_MYTASK_H
#define SPELLCHECK_MYTASK_H

#include "MyConf.h"
#include "MyResult.h"
#include "MyCompare.h"
#include <iostream>
#include <string>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <map>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <queue>
#include <set>
#include <winsock.h>

class MyCache;

class MyTask{
public:
    MyTask(MyConf& conf);
    
    MyTask(const std::string& queryWord, const struct sockaddr_in& addr,MyConf& conf);
    
    //执行函数，需要传递一个MyCache对象
    void excute(MyCache& cache);
    
    //计算查询词的长度
    int length(const std::string& str){
        int len;
        for (int index = 0;  index != str.size() ; ++index) {
            if(str[index] & (1 << 7))
            {
                index++;
            }
            len++;
        }
        return len;
    }
    ~MyTask(){
        close(_fd);
    }
    
    //计算_vecDictPtr指向的vector中下标在iset中的词与用户输入词的编辑距离
    void satistic(std::set<int>& iset);
    
    
private:
    std::string _queryWord;                 //用户查询词
    std::vector<uint32_t> _vecQueryWord;    //经过转换后的用户的查询词
    struct sockaddr_in _addr;               //用于保存用户端地址和端口信息
    int _fd;                                //用于用户端通信的socket描述符
    
    std::vector<std::pair<std::vector<uint32_t>,int>>  *_vecDictPtr; //指向保存数据词典的指针(转换后)
    
    std::vector<std::pair<std::string,int>> *_strDictPtr;   //指向保存数据词典的指针(未转换前)
    
    std::map<uint32_t,std::set<int>> *_mapIndexPtr; //指向词典索引的指针
    
    std::priority_queue<MyResult,std::vector<MyResult>,MyCompare> _result;  //用于保存查询结果的优先级队列
    
    void  get_result();     //根据用户的查询词获取最终结果。最终结果将放在优先级队列里
    
    int editDistance(const std::vector<uint32_t> &right);   //计算right与用户输入词的编辑距离
    
    int triple_min(const int& a, const int& b, const int& c){
        return a < b ?(a < c ? a : c) : (b < c ? b : c);
    }
    
};



#endif //SPELLCHECK_MYTASK_H
