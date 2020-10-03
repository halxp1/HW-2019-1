#ifndef TOOLS_H_
#define TOOLS_H_

#include "define.h"


class Tools{
public:
    static std::vector<std::string> split(std::string str, std::string pattern);  //字符串分割
    static int Cal_similarity(std::vector<int> car1, std::vector<int> car2);      //计算相似度
//    static int Duplicate_judgment(std::vector<int> roadcar);

};

#endif