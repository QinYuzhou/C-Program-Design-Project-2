#include <iostream>
#include <stack>
#include <map>
#include "BigNumber.h"

std::string formula;
int *match;
bool have_problem;
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
    if (formula[ed] == ')')
    {
        std::string function_name = formula.substr(st, match[ed] - st);
        if (function_name == "sqrt")
            return calculate(match[ed], ed).sqrt();
        if(function_name=="abs")
            return abs(calculate(match[ed], ed));
        if (function_name == "pow"||function_name == "max"||function_name == "min")
        {
            now = ed - 1;
            while (now > match[ed])
            {
                if (formula[now] == ')')
                {
                    now = match[now];
                }
                else if (formula[now] == ',')
                {
                    if(function_name=="pow")
                        return calculate(match[ed] + 1, now - 1).pow(calculate(now + 1, ed - 1));
                    if(function_name=="max")
                        return max(calculate(match[ed] + 1, now - 1),(calculate(now + 1, ed - 1)));
                    if(function_name=="min")
                        return min(calculate(match[ed] + 1, now - 1),(calculate(now + 1, ed - 1)));
                }
                now--;
            }
        }
    }
    //不存在加减乘除,这是一个数字或变量
    if (variables.count(formula.substr(st, ed - st + 1))) //这是一个变量
        return variables.find(formula.substr(st, ed - st + 1))->second;
    Big_number result;
    if (result.change_to(formula.substr(st, ed - st + 1))) //这是一个数字
    {
        return result;
    }
    have_problem=true;
    return Big_number(0);
}

bool parentheses_match() //返回括号匹配的结果，若formula[i]是右括号,match[i]为其对应的左括号位置
{
    std::stack<int> stk;
    if (match != NULL)
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

bool check_legitimacy(std::string variable)//检查变量名是否符合规范
{
    if(variable[0]<'a'||'z'<variable[0])
        return false;
    for(char c: variable)
    {
        if('0'<=c&&c<='9')
            continue;
        if('a'<=c&&c<='z')
            continue;
        if('a'=='_')
            continue;
        return false;
    }
    return true;
}

bool variable_define() //判断这是否是一个赋值语句,如果是的话返回true并进行赋值,否则返回false
{
    for (int i = 0; i < formula.size(); i++)
        if (formula[i] == '=')
        {
            if(!check_legitimacy(formula.substr(0,i)))
            {
                have_problem=true;
                return true;
            }
            Big_number value = calculate(i + 1, formula.size() - 1);
            variables.insert_or_assign(formula.substr(0, i), value);
            return true;
        }
    return false;
}

void remove_space() //删除formula中所有空格
{
    std::string new_str = "";
    for (int i = 0; i < formula.size(); i++)
        if (formula[i] != ' ')
            new_str.append(1, formula[i]);
    formula = new_str;
}

int main()
{
    
    while(true)
    {
        std::string s1,s2;
        std::cin>>s1>>s2;
        Big_number a,b,c;
        a.change_to(s1);
        b.change_to(s2);
        bool flag=(a==b);
        std::cout<<flag<<std::endl;
    }
    
    while (true)
    {
        std::cin >> formula;
        have_problem=false;
        remove_space();
        if (!parentheses_match())
            std::cout << "Invalid format" << std::endl;
        if (variable_define())
            continue;
        Big_number ans = calculate(0, formula.size() - 1);
        std::cout << ans << std::endl;
    }
    return 0;
}