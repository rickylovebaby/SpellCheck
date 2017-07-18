//
// Created by Ricky on 7/7/2017.
//
#include "MyConf.h"


MyConf::MyConf(const std::string &name)
    : _infile(name.c_str())
{
    //处理配置文件
    if(!_infile)
    {
        std::cout << __DATE__<<" "__TIME__" "
                  <<__FILE__<<" "__LINE__": "
                  <<"open failed!" <<std::endl;
        exit(-1);
    }
    std::string line1;
    while(getline(_infile,line1)){
        std::istringstream instream(line1);
        std::string key,value;
        instream>>key>>value;
        _mapConf.insert(make_pair(key,value));
    }
    _infile.close();
    
    //处理字典
    _infile.open(_mapConf["dict"].c_str());
    if(!_infile){
        std::cout << __DATE__<<" "__TIME__" "
                  <<__FILE__<<" "__LINE__": "
                  <<"open failed!" <<std::endl;
        exit(-1);
    }
    std::string line2;
    while(getline(_infile,line2))
    {
        std::istringstream instream(line2);
        std::string word;
        int frequency;
        instream >> word >> frequency;
        
        std::vector<uint32_t> vecWord;
        //parseUTF8String(word,vecWord);
        ConvertAlgo::parseUTF8String(word,vecWord);
        
        _strDict.push_back(make_pair(word, frequency));
        _vecDict.push_back(make_pair(vecWord,frequency));
        
    }
    _infile.close();
    
    //依次为每一个单词（vector<uint32_t>）的每一个字母所对应的索引
    for(int vecDictIndex = 0; vecDictIndex != _vecDict.size();++vecDictIndex)
    {
        setIndex(vecDictIndex);
    }
}

void MyConf::setIndex(int vecDictIndex)
{
    std::vector<uint32_t> vecWord;//单词
    uint32_t  letter;//以及包含的每一个字母
    /*
     * 根据词典的下标来找到每一个单词
     *
     * 倒排索引的含义
     *
     * */
    vecWord = _vecDict[vecDictIndex].first;
    
    for(int index = 0; index != vecWord.size();++index){
        letter = vecWord[index];                    //获取每一个字母
        _mapIndex[letter].insert(vecDictIndex);     //将每一个字母所在的vector的下标插入到set中，
                                                    // 表示这个字母的倒排索引
    }
}

std::map<std::string, std::string>& MyConf::getMapConf()
{
    return _mapConf;
}