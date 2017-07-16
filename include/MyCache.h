//
// Created by Ricky on 7/7/2017.
//


#ifndef SPELLCHECK_MYCACHE_H
#define SPELLCHECK_MYCACHE_H
#include <unordered_map>
#include <stdexcept>
#include <string>
#include <utility>
#include <fstream>
#include "MyLock.h"
#include <mutex>

using It = std::unordered_map<std::string,std::string>::iterator;

class MyCache{
public:
	std::unordered_map<std::string, std::string> _hashmap;

	/*
	RAII类型的map_to_cache
	void map_to_cache(std::string& key, std::string& value){
		std::lock_guard<std::mutex> guard(mutex);
		_hashmap[key]=value;
	}
	 
	 工作线程与扫描线程应该互斥的访问工作线程的内存cache
	*/
	void map_to_cache(std::string& key, std::string& value){
		_hashmapLock.lock();
		_hashmap[key]=value;
		_hashmapLock.unlock();
	}
	It isMapped(const std::string& word){
		return _hashmap.find(word);
	}

	//将内存cache写入到硬盘
	void write_to_file(std::ofstream& outfile){
		_hashmapLock.lock();
		for(It iter = _hashmap.begin();iter!= _hashmap.end();++iter){
			outfile << iter->first << "\t"<<iter->second << std::endl;
		}
		_hashmapLock.unlock();
	}

	void read_from_file(const std::string& fileName){
		std::ifstream infile(fileName.c_str());
		if(!infile)
		{
			std::cout << "cache file: "<<fileName <<std::endl;
			throw std::runtime_error("open cache file failed!");
		}
		std::string query, result;
		while(infile >> query >>result)
		{
			_hashmap.insert(std::make_pair(query,result));
		}
		infile.close();
	}



private:

	//std::mutex _mutex;
	MyLock _hashmapLock;
};

#endif