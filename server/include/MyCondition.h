//
// Created by Ricky on 7/7/2017.
//

#ifndef SPELLCHECK_MYCONDITION_H
#define SPELLCHECK_MYCONDITION_H

#include "MyLock.h"
class MyCondition
{
public:
    MyCondition(MyLock& lock)
            :m_mutex(lock.m_mutex)
    {
        if(pthread_cond_init(&m_cond,NULL))
        {
            std::cout << __DATE__<<" "<<__TIME__<<" "
                      <<__FTILE__<<" "<<__LINE__<<":"
                      <<"pthread_cond_init failed!"<<std::endl;
            
            exit(-1);
        }
    }
    void wait(){
        pthread_cond_wait(&m_cond,&m_mutex);
        
    }
    void broadcast()
    {
        pthread_cond_broadcast(&m_cond); //将所有等待该条件变量的线程解锁而不是仅仅解锁一个线程
    }
    ~MyCondition()
    {
        pthread_cond_destroy(&m_cond);
    }
    
private:

    pthread_mutex_t  & m_mutex;
    pthread_cond_t  m_cond;
};

#endif //SPELLCHECK_MYCONDITION_H
