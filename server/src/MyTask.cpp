//
// Created by Ricky on 7/7/2017.
//
#include "MyTask.h"
#include "MyCache.h"
#include "MyConf.h"
#include "MyMethod.h"
#include <functional>
#include <algorithm>
#include <string.h>

MyTask::MyTask(MyConf &conf)
    :_queryWord(""),
     _strDictPtr(&(conf._strDict)),
     _vecDictPtr(&(conf._vecDict)),
     _mapIndexPtr(&conf._mapIndex)
{
    memset(&_addr,0,sizeof(_addr));
}

MyTask::MyTask(const std::string& queryWord, const struct sockaddr_in& addr, MyConf& conf)
        :_queryWord(queryWord),
         _addr(addr),
         _strDictPtr(&(conf._strDict)),
         _vecDictPtr(&(conf._vecDict)),
         _mapIndexPtr(&conf._mapIndex)
{
    ConvertAlgo::parseUTF8String(_queryWord,_vecQueryWord);
}

//执行任务，并且将结果发回客户端
void MyTask::excute(MyCache& cache){
    //udp创建套接字连接
    _fd = socket(AF_INET,SOCK_DGRAM,0);
    std::cout << "Task excute" << std::endl;
    
    std::unordered_map<std::string,std::string>::iterator iter  = cache.isMapped(_queryWord);
    //如果queryWord在cache中可以找到，则可以直接返回结果
    if(iter != cache._hashmap.end())
    {
        std::cout << "cached !"<<std::endl;
        int iret = sendto(_fd,(iter->second).c_str(),(iter->second).size(),0,(struct sockaddr*)&_addr,sizeof(_addr));
        
        std::cout <<"send : " << iret << std::endl;
    }
        //否则在词频字典中进行计算后，返回最佳匹配的单词
    else
    {
        std::cout <<"no cached " << std::endl;
        get_result();
        if(_result.empty())//如果没有最佳匹配的单词，则返回“No answer!”
        {
            std::string res = " no answer";
            int iret = sendto(_fd,res.c_str(),res.size(),0,(struct sockaddr*)&_addr,sizeof(_addr));
            std::cout << "Send: "<<iret << std::endl;
        }
        else{
            //如果找到了最佳匹配的单词，则在保存了查询结果的优先队列中取头元素，并且返回
            MyResult res = _result.top();
            int iret = sendto(_fd,res._word.c_str(),res._word.size(),0,(struct sockaddr*)&_addr,sizeof(_addr));
            std::cout << "send: "<<iret <<std::endl;
            
            //并且将该查询词所对应的词库中的词写入到cache中，即更新当前工作线程的cache
            cache.map_to_cache((_queryWord,res._word));
        }
    }
}


//计算编辑距离
int MyTask::editDistance(const std::vector<uint32_t> &right)
{
    return ConvertAlgo::edit_distance_uint_32(_vecQueryWord,right);
}

void MyTask::satistic(std::set<int> &iset)
{
    std::set<int>::iterator iter;
    for(iter = iset.begin(); iter != iset.end(); iter++)
    {
        int dist = editDistance((*_vecDictPtr)[*iter].first);
        //如果编辑距离在3以内，则更新res的值并且将其加入到优先队列
        if(dist < 3){
            MyResult res;
            res._word = ((*_strDictPtr)[*iter]).first;//查询词的原始词（未转换成uint32_t）
            res._distance = dist;//距离差
            res._frequence = ((*_vecDictPtr)[*iter]).second;//词频
            _result.push(res);
        }
    }
}

//遍历查询词的每一个letter，经由倒排索引，统计出编辑距离在3以内的单词，并且将其放入到优先队列_result中
void MyTask::get_result()
{
    uint32_t  ch;
    for(int index = 0 ; index != _vecQueryWord.size();index++)
    {
        ch = _vecQueryWord[index];
        if((*_mapIndexPtr).count(ch))//如果含有该letter,则。。
        {
            std::cout << "map_cout return true"<<std::endl;
            satistic((*_mapIndexPtr)[ch]);
        }
    }
}