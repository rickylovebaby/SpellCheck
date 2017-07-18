//
// Created by Ricky on 7/7/2017.
//

#ifndef SPELLCHECK_MYCONF_H
#define SPELLCHECK_MYCONF_H
#include <map>
#include <iostream>
#include <utility>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <set>
#include <vector>
#include <stdint.h>

class MyConf{

public:
	MyConf(const std::string& name);

	void setIndex(int vecDictIndex);
	
	std::map<std::string, std::string>& getMapConf();

private:
	std::ifstream m_infile;
	std::map<std::string,std::string> _mapConf;//配置文件

	std::vector<std::pair<std::string,int>> _strDict;//未转换为uint32_t的词频字典

	std::vector<std::pair<std::vector<uint32_t>,int>> _vecDict;//转换后的词频字典

	std::map<uint32_t,std::set<int>> _mapIndex; //字母索引


	friend class MyTask;
	friend class MySocket;
	friend class ThreadPool;
};



#endif