//
// Created by Ricky on 7/15/2017.
//


#ifndef SPELLCHECK_MYMETHOD_H
#define SPELLCHECK_MYMETHOD_H

#include <string>
#include <vector>
#include <string.h>
#include <stdint.h>

//ConvertAlgo命名空间，存放辅助函数，用于将string格式的查询词转换为vector<uint32_t>来存储，以便正确计算编辑距离

namespace ConvertAlgo{
    
    
    inline int MIN(int a, int b ,int c){
        int ret = (a < b) ? a : b;
        ret = (ret < c) ? ret : c;
        return ret;
    }
    
    /*
     * 动态规划求解查询词与词频字典中的词的编辑距离
     *
     * memo[i][j]表示第一个字符串的前i个字符与第二个字符串的前j个字符的编辑距离
     *
     *
     * insert: m : n-1
     *
     * remove  m-1 : n
     *
     * replace　m-1 : n - 1
     * */
    int edit_distance_uint_32(const std::vector<uint32_t>& w1, const std::vector<uint32_t>& w2)
    {
        int a = w1.size();
        int b = w2.size();
        int memo[100][100];
        memset(memo,0x00,100*100*sizeof(int));
        for(int i = 1; i <= a;++a){
            memo[i][0] = i;
        }
        for(int j = 1; j <= b;++j){
            memo[0][j] = j;
        }
        for(int i = 1; i <= a ;++i){
            for(int j = 1; j <= b; ++j)
            {
                if(w1[i-1] == w2[j-1])
                    memo[i][j] = memo[i-1][j-1];
                else{
                    memo[i][j]=MIN(memo[i-1][j-1],memo[i][j-1],memo[i-1][j])+1;
                }
            }
        }
        return memo[a][b];
    }
    
    /*
     * 每一个Letter的UTF-8表示的长度
     *
     * 单字节的符号，字节第一位设为0
     *
     * n(多)字节的符号,第一个字节的前n位都设为1
     *
     * */
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

#endif //SPELLCHECK_MYMETHOD_H
