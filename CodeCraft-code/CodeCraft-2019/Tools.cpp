#include "define.h"
#include "Tools.h"
#include <iostream>

/*********************************************************************************
 *example:
 * 输入：(10000, 15, 35, 6, 1) 字符串
 * 返回：<10000,15,35,6,1>  vector向量
********************************************************************************/
std::vector<std::string> Tools::split(std::string str,std::string pattern) {  //pattern所要分割的符号  本题为','

    std::string::size_type  pos; //用来保存string vector 类型对象的长度
    std::vector<std::string> result;
    str=str.substr(0,str.length()-2) + pattern;  //(10000, 15, 35, 6, 1,

    int size = str.size();
    for(int i=0; i<size; ++i){
        pos = str.find(pattern,i);  //从 i开始寻找pattern的位置

        if(pos<size){
            std::string s = str.substr(i+1,pos-i-1);  //获取i+1 到pos-i-1 位置的字符串
            result.push_back(s);
            i=pos;
        }
    }
    return result;
}


/**********************计算两条路的相似度**********************************************/

int Tools::Cal_similarity(std::vector<int> car1, std::vector<int> car2) {    //car1长  car2短
    int counter = car1.size();
    std::unordered_set<int> mSet;
    for(int i=0; i<car2.size() ; i++){  //遍历较短的vector存进HashSet
        mSet.insert(car2[i]);
    }
    //遍历较长的vector
    for(int j=0; j<car1.size(); j++){
        if(mSet.end() == mSet.find(car1[j]))  //注意调用set的find方法，这个方法会通过计算Hash值来比较
            counter--;
    }
    return counter;
}

//int Tools::Duplicate_judgment(std::vector<int> roadcar) {
//    int specifyCountTimes = 0;
//    for(int i=0; i<roadcar.size();i++){
//        for(int j=0 ;j<roadcar.size();j++){
//
//
//
//        }
//
//
//    }
//
//}

