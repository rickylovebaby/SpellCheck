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

//ConvertAlgo命名空间，存放辅助函数，用于将string格式的查询词转换为vector<uint32_t>来存储，以便正确计算编辑距离

namespace ConvertAlgo{
	
	int getLenOfUTF8(unsigned char c)
	{
		int cnt = 0;
		while(c & ( 1 << (7 -cnt)))
			++cnt;
		return cnt;
	}
	
	/*
     * 每一个vector代表一个word，把字符串解析成uint32_t的数组
     *
     * */
	void parseUTF8String(const std::string& s, std::vector<uint32_t>& vec)
	{
		vec.clear();
		for (std::string::size_type ix = 0; i < s.size(); ++ix) {
			int len = getLenOfUTF8(s[ix]);
			uint32_t  t= (unsigned char)s[ix];
			if(len > 1)
			{
				--len;
				while(len--){
					t = (t << 8) + (unsigned char)s[++ix];
				}
			}
			vec.push_back(t);
		}
	}
}

#endif