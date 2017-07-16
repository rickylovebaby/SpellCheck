//
// Created by Ricky on 7/7/2017.
//

#ifndef SPELLCHECK_MYCOMPARE_H
#define SPELLCHECK_MYCOMPARE_H
#include "MyResult.h"

class MyCompare{
public:
	//重载函数调用操作符，主要用于初始化优先级队列
	bool operator()(const MyResult& left, const MyResult& right)
	{
		if(left._distance > right._distance){
			return true;
		}
		else if(left._distance == right._distance && left._frequence < right._frequence)
		{
			return true;
		}
		else if(left._distance == right._distance && left._frequence == right._frequence && left._word > right._word){
			return true;
		}
		else{
			return false;
		}	
	}
};
#endif