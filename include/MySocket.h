//
// Created by Ricky on 7/7/2017.
//

#ifndef SPELLCHECK_MYSOCKET_H
#define SPELLCHECK_MYSOCKET_H
#include <string>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <iostream>
#include <afxres.h>
#include "MyConf.h"

class MySocket{
public:
	MySocket(MyConf& conf):_conf(conf){

		std::cout << _conf._mapConf["ip"] << std::endl;
		_fd = socket(AF_INET,SOCK_DGRAM,0);
		if(_fd == -1)
		{
			std::cout <<__DATE__<<" "<<__TIME__<<" "<<__FILE__<<" "__LINE__<<": "
						
						<<"socket"<<std::endl;
			
			exit(-1);
		}
		
		_addrLen =sizeof(_addr);
		_addr.sin_family = AF_INET;
		_addr.sin_port = htons(atoi(_conf._mapConf["port"].c_str()));
		_addr.sin_addr.s_addr = inet_addr(_conf._mapConf["ip"].c_str());
		
		if(bind(_fd,(struct sockaddr*)&_addr, sizeof(_addr)) == -1)
		{
			std::cout <<__DATE__<<" "<<__TIME__ << " "<<__FILE__<<" "<<__LINE__<<": "<<"bind"<<std::endl ;
			exit(-1);
		}
	}
	
	int send_message(void* buf,int len)
	{
		return sendto(_fd,buf,len,0,(struct sockaddr*)&_addr, sizeof(_addr));
	}
	int recv_message(void* buf, int len)
	{
		std::cout <<"socket:: recv ..."<<std::endl;
		return recvfrom(_fd,buf,len,0,(struct sockaddr*)&_addr,&_addrLen);
		
	}
	const struct sockaddr_in get_addr() const {
		return _addr;
	}
	~MySocket()
	{
		close(_fd);
	}


private:
	int _fd;                        //用于标识socket的描述符

	struct sockaddr_in _addr;		//用于保存服务器端或者客户端ip和端口号信息

	socklen_t _addrLen;				//用于保存struct sockaddr_in的长度

	MyConf& _conf; 					//用户配置对象的引用
};


#endif