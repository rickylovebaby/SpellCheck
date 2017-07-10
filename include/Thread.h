//
// Created by Ricky on 7/7/2017.
//
/*
 * 实现复用，所以先定义Thread类，后面的工作线程和内存扫描线程分别继承该Thread类
 * */
#ifndef SPELLCHECK_THREAD_H
#define SPELLCHECK_THREAD_H
#include <iostream>
#include <stdlib.h>
#include <pthread.h>

class Thread{

public:

	Thread():
		_threadId(0),_isRunning(false)
		{
			if(pthread_attr_init(&threadAttr)){
				std::cout << __DATE__<<" "<<__TIME__<<" "
						  << __FILE__<<" "<<__LINE__<<":"
						  <<"pthread_attr_init"<<std::endl;

				exit(-1);
			}
		}
		~Thread(){
			pthread_attr_destory(&threadAttr);
		}

		void start(void *arg = NULL)
		{
			if(_isRunning)
				return;

			m_arg = arg;
			_isRunning = true;

			//将线程设置为脱离线程
			if(pthread_attr_setdetachstate(&threadAttr,PTHREAD_CREATE_DETACHED))
			{
				std::cout << __DATE__<<" "<<__TIME__<<" "
						  << __FILE__<<" "<<__LINE__<<":"
						  <<"pthread_attr_setdetachstate"<<std::endl;

				exit(-1);
			}
			//创建线程
			if(pthread_create(&_threadId,Thread::runInThread,this))
			{

				std::cout << __DATE__<<" "<<__TIME__<<" "
						  << __FILE__<<" "<<__LINE__<<":"
						  <<"pthread_create"<<std::endl;

				exit(-1);
			}

		}
protected:
	void* m_arg;


private:
	static void* runInThread(void* arg)
	{
		Thread* p = (Thread*)arg;
		p->run();
		return NULL;
	}

    //纯虚函数，派生类应该实现该方法
	virtual void run() = 0;

	bool _isRunning;
	pthread_t _threadId;
	pthread_attr_t threadAttr;

};

#endif




