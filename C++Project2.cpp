#include <iostream>
#include <stack>
#include <map>
#include "BigNumber.h"

std::string formula;
int *match;
std::map<std::string, Big_number> variables;

Big_number calculate(int st, int ed) //这个函数会返回formula中下标从st到ed的字串计算的结果
{
    if (match[ed] == st)
        return calculate(st + 1, ed - 1);
    int now = ed;
    while (now >= st) //判断是否有加号或减号
    {
        if (formula[now] == ')')
        {
            now = match[now];
        }
        if (formula[now] == '+')
        {
            return calculate(st, now - 1) + calculate(now + 1, ed);
        }
        if (formula[now] == '-')
        {
            return calculate(st, now - 1) - calculate(now + 1, ed);
        }
        now--;
    }
    now = ed;
    while (now >= st) //判断是否有乘号或除号
    {
        if (formula[now] == ')')
        {
            now = match[now];
        }
        if (formula[now] == '*')
        {
            return calculate(st, now - 1) * calculate(now + 1, ed);
        }
        if (formula[now] == '/')
        {
            return calculate(st, now - 1) / calculate(now + 1, ed);
        }
        now--;
    }
    //不存在加减乘除,这是一个数字或变量
    if (variables.count(formula.substr(st, ed - st + 1)))//这是一个变量
        return variables.find(formula.substr(st, ed - st + 1))->second;
    Big_number result;
    if (result.change_to(formula.substr(st, ed - st + 1)))//这是一个数字
        return result;
    return Big_number(0);
}

bool parentheses_match() //返回括号匹配的结果，若formula[i]是右括号,match[i]为其对应的左括号位置
{
    std::stack<int> stk;
    if(match!=NULL)
        delete[] match;
    match = new int[formula.size()];
    memset(match, -1, sizeof(int) * formula.size());
    for (int i = 0; i < formula.size(); i++)
    {
        if (formula[i] == ')')
        {
            if (stk.empty())
                return false;
            match[i] = stk.top();
            stk.pop();
        }
        else if (formula[i] == '(')
        {
            stk.push(i);
        }
    }
    if (stk.empty())
        return true;
    return false;
}

bool variable_define() //判断这是否是一个赋值语句,如果是的话返回true并进行赋值,否则返回false
{
    for (int i = 0; i < formula.size(); i++)
        if (formula[i] == '=')
        {
            Big_number value = calculate(i + 1, formula.size() - 1);
            variables.insert_or_assign(formula.substr(0, i), value);
            return true;
        }
    return false;
}

int main()
{
    while (true)
    {
        std::cin >> formula;
        if (!parentheses_match())
            std::cout << "Invalid format" << std::endl;
        if (variable_define())
            continue;
        Big_number ans = calculate(0, formula.size() - 1);
        std::cout << ans << std::endl;
    }
    return 0;
}