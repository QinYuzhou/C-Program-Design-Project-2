#pragma once
#include <iostream>
#include "FFT.h"

bool get_E_location(std::string str, int &e); //找到'e'在字符串中的位置,并储存在e中,如果多于1个就返回false,如果不存在则使e=-1

bool get_point_location(std::string str, int &p); //找到'.'在字符串中的位置,并储存在p中,如果多于1个就返回false,如果不存在则使p=-1

bool isDigit(std::string str); //判断是否全由数字组成

bool to_int(std::string str, int &num); //将字符串转化为数字并储存在num里,若字符串格式不对则返回false

struct Big_number
{
    bool positive; //该数字的正负性,positive=true则为正数,反之为负数
    int length;    //数字的位数/长度
    int *num;      //数字的每一位,其中num[0]代表各位,num[length-1]代表最高位
    int power;     //科学计数法之后10的幂
    //该数字为(positive)(num)*(10^power)

    Big_number(); //构造函数,默认初值为0

    Big_number(long long number); //将long long转化为Big_Number
    
    void clear(); //将数字归零

    void fixed(); //若数字的有效数字大于100位，则将其改为100位

    bool change_to(std::string str); //将string str转化为数字,若格式不对返回false
    
    bool operator<(const Big_number number); //比较两个数的大小关系
    
    Big_number operator+(Big_number number); //进行加法计算
    
    Big_number operator-(Big_number number); //进行减法计算,a-b就是a+(-b)

    Big_number operator*(Big_number number); //进行乘法计算
    
    Big_number half(); //返回该数除2的结果(除2就是乘0.5)
   
    Big_number operator/(Big_number number); //进行除法计算 (a*10^k)/(b*10^m)=(a/b)*10^(k-m),通过二分答案实现

};

std::ostream &operator<<(std::ostream &out, Big_number &num); //重载输出流
