#ifndef SPELLCHECK_MYRESULT_H
#define SPELLCHECK_MYRESULT_H
#include <vector>
#include <string>


//用来存储程序查询结果的，该对象放在优先级对象中

class MyResult{
public:
	std::string _word;//保存词库中的词

	int _frequence;//保存词库中词的词频
	int _distance;//保存词库中的词与用户输入词的编辑距离

};
#endif