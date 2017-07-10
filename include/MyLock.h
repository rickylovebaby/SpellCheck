#ifndef SPELLCHECK_MYLOCK_H
#define SPELLCHECK_MYLOCK_H
#include <pthread.h>
#include <iostream>
#include <stdlib.h>
class MyLock{
public:
	MyLock(){
		//初始化成功后返回0
		if(pthread_mutex_init(&m_mutex,NULL)){
			std::cout <<__DATA__ <<" "<<__TIME__<<" "
					  <<__FILE__ <<" "<<__LINE__<<":"
					  <<"pthread mutex init failed!"<<std::endl;
			exit(-1);
		}
	}
	void lock(){
		pthread_mutex_lock(&m_mutex);
	}
	void unlock(){
		pthread_mutex_unlock(&m_mutex);
	}
	~MyLock(){
		pthread_mutex_destory(&m_mutex);

	}

private:
	pthread_mutex_t m_mutex;
	friend class MyCondition;
};