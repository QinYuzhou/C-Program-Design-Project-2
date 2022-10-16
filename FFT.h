#pragma once

long long qpow(long long x, int y); //计算x的y次方

void init(int x, int &n, int *&rev, long long *&cof_to_val, long long *&val_to_cof); //对于结果长度小于等于x的数组预处理，其中n为数组长度，rev为二进制下反转的结果，cof_to_val为系数表达式转为点值表达式时需要用到的数组，val_to_cof为点值表达式转化为系数表达式时需要用到的数组

void coefficient_to_value(long long *v, int n, const int *rev, const long long *cof_to_val); //将数组v从系数表达式转化为点值表达式

void value_to_coefficient(long long *v, int n, const int *rev, const long long *val_to_cof); //将数组v从点值表达式转化为系数表达式

void mul(const int *num1, int len1, const int *num2, int len2, int *&ret, int &ret_len); //将num1与num2相乘的结果存储在ret中返回
