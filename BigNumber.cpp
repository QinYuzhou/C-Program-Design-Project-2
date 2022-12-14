#include "BigNumber.h"

bool get_E_location(std::string str, int &e) //找到'e'在字符串中的位置,并储存在e中,如果多于1个就返回false,如果不存在则使e=-1
{
    e = -1;
    for (int i = 0; i < str.size(); i++)
    {
        if (str[i] == 'e')
        {
            if (e == -1)
            {
                e = i;
            }
            else
            {
                return false;
            }
        }
    }
    return true;
}

bool get_point_location(std::string str, int &p) //找到'.'在字符串中的位置,并储存在p中,如果多于1个就返回false,如果不存在则使p=-1
{
    p = -1;
    for (int i = 0; i < str.size(); i++)
    {
        if (str[i] == '.')
        {
            if (p == -1)
            {
                p = i;
            }
            else
            {
                return false;
            }
        }
    }
    return true;
}

bool isDigit(std::string str) //判断是否全由数字组成
{
    for (char c : str)
    {
        if (c < '0' || c > '9')
            return false;
    }
    return true;
}

bool to_int(std::string str, int &num) //将字符串转化为数字并储存在num里,若字符串格式不对则返回false
{
    if (!str.size())
    {
        return false;
    }
    bool is_positive = true;
    if (str[0] == '-')
    {
        is_positive = false;
        str = str.substr(1);
        if (!str.size())
        {
            return false;
        }
    }
    if (!isDigit(str))
        return false;
    num = 0;
    for (int i = 0; i < str.size(); i++)
    {
        num *= 10;
        num += str[i] - '0';
    }
    if (!is_positive)
    {
        num *= -1;
    }
    return true;
}

Big_number::Big_number() //构造函数,默认初值为0
{
    positive = true;
    length = 1;
    power = 0;
    num = new int[1];
    num[0] = 0;
}

Big_number::Big_number(long long number) //将long long转化为Big_Number
{
    //判断number的正负性
    if (number < 0)
    {
        number *= -1;
        positive = false;
    }
    else
    {
        positive = true;
    }
    //去除number的后导0
    power = 0;
    while (number && number % 10 == 0)
    {
        number /= 10;
        power++;
    }
    length = 0;
    long long x = number;
    while (x) //计算位数
    {
        x /= 10;
        length++;
    }
    if (!length)
    {
        num = new int[1];
        length = 1;
    }
    else
    {
        num = new int[length];
        for (int i = 0; i < length; i++)
        {
            num[i] = number % 10;
            number /= 10;
        }
    }
}

Big_number::Big_number(const Big_number &number) //将Big_Number赋值给当前数
{
    positive = number.positive;
    length = number.length;
    num = new int[length];
    for (int i = 0; i < length; i++)
        num[i] = number.num[i];
    power = number.power;
}

Big_number::Big_number(Big_number &&number) //将右值深拷贝
{
    positive = number.positive;
    length = number.length;
    num = new int[length];
    for (int i = 0; i < length; i++)
        num[i] = number.num[i];
    power = number.power;
}

Big_number::~Big_number() //析构函数
{
    if (num != NULL)
        delete[] num;
}

void Big_number::clone(const Big_number &number) //将number复制
{
    positive = number.positive;
    length = number.length;
    if (num != NULL)
        delete[] num;
    num = new int[length];
    for (int i = 0; i < length; i++)
        num[i] = number.num[i];
    power = number.power;
    return;
}

void Big_number::clear() //将数字归零
{
    positive = true;
    length = 1;
    power = 0;
    delete[] num;
    num = new int[1];
    num[0] = 0;
    return;
}

void Big_number::fixed() //若数字的有效数字大于100位，则将其改为100位
{
    if (length <= 100)
        return;
    if (num[length - 101] >= 5)
    {
        num[length - 100]++;
        int now = length - 100;
        while (now < length - 1 && num[now] == 10)
        {
            num[now + 1] += num[now] / 10;
            num[now] %= 10;
            now++;
        }
        if (num[now] == 10)
        {
            power += length;
            delete[] num;
            num = new int[1];
            length = 1;
            num[0] = 1;
            return;
        }
    }
    int new_len = 100;
    while (new_len > 0 && num[length - new_len] == 0) //去除后导0
    {
        new_len--;
    }
    if (!new_len)
    {
        clear();
        return;
    }
    int *digits = new int[new_len];
    for (int i = 0; i < new_len; i++)
    {
        digits[i] = num[length - new_len + i];
    }
    power += length - new_len;
    length = new_len;
    delete[] num;
    num = digits;
    return;
}

bool Big_number::change_to(std::string str) //将string str转化为数字,若格式不对返回false
{
    int e_location, p_location, pointer;  // e_location:字符'e'所在的位置,如果不存在'e'时为-1;p_location:字符'.'所在的位置,如果不存在'.'时为-1
    if (!get_E_location(str, e_location)) //如果有多个'e',格式错误
    {
        goto FORMAT_ERROR;
    }
    if (e_location != -1) //有字'e',是科学计数法,先处理科学计数法下10的幂
    {
        if (!to_int(str.substr(e_location + 1), power)) //字符'e'后面的不是整数
        {
            goto FORMAT_ERROR;
        }
        str = str.substr(0, e_location);
    }
    else
    {
        power = 0;
    }
    //继续处理科学计数法中前面的数字
    if (str[0] == '-') //先判断该数的正负性
    {
        positive = false;
        str = str.substr(1);
    }
    else
    {
        positive = true;
    }
    if (!str.size()) // str长度为0,格式错误
    {
        goto FORMAT_ERROR;
    }
    if (!get_point_location(str, p_location)) //如果有多个小数点,格式错误,将数字设为0,返回false
    {
        goto FORMAT_ERROR;
    }
    if (p_location != -1) //存在小数点,将该数字改为整数乘十的幂,并更新power
    {
        if (p_location == 0 || p_location == str.size() - 1) //小数点在该数的第一位获最后一位,格式错误,将数字设为0,返回false
        {
            goto FORMAT_ERROR;
        }
        power -= str.size() - p_location - 1;                         //小数点在第p位,将该数乘以10^(l-p-1),并更新power
        str = str.substr(0, p_location) + str.substr(p_location + 1); //更新str
    }
    if (!isDigit(str)) // str中有数字之外的字符,格式错误,将数字设为0,返回false
    {
        goto FORMAT_ERROR;
    }
    //去除该数字的前导0,pointer是该数最高的非0位
    pointer = 0;
    while (pointer < str.size() && str[pointer] == '0')
        pointer++;
    if (pointer == str.size()) //该数所有数字都是0,将该数设为0,返回true
    {
        clear();
        return true;
    }
    str = str.substr(pointer); //更新str
    //去除该数字末尾的0,并更新power,pointer是该数最小的非0位
    pointer = str.size() - 1;
    while (pointer >= 0 && str[pointer] == '0')
        pointer--;
    power += str.size() - pointer - 1; //更新power
    str = str.substr(0, pointer + 1);  //更新str
    //存储这个数字
    length = str.size();
    delete[] num;
    num = new int[length];
    for (int i = 0; i < length; i++)
        num[i] = str[length - i - 1] - '0';
    return true;
FORMAT_ERROR: //该数格式错误,将该数清零并返回false
    clear();
    return false;
}

Big_number &Big_number::operator=(const Big_number &number) //深度拷贝
{
    if (this == &number)
        return (*this);
    clone(number);
    return (*this);
}

Big_number &Big_number::operator=(Big_number &&number) //右值拷贝时也进行深度拷贝
{
    if (this == &number)
        return (*this);
    clone(number);
    return (*this);
}

bool Big_number::operator<(const Big_number &number) const //比较两个数的大小关系,若前者小于后者,返回true
{
    if (positive != number.positive) //一正一负
    {
        return !positive; //若左侧为负数，则返回true，否则返回true
    }
    if (positive) //全为正数
    {
        if (length == 1 && num[0] == 0)
            return true;
        if (number.length == 1 && number.num[0] == 0)
            return false;
        if (length + power == number.length + number.power) //最高位数相同
        {
            for (int i = 1; i <= length && i <= number.length; i++) //从最高位依次比较，若不相等则返回大小关系
            {
                if (num[length - i] == number.num[number.length - i])
                    continue;
                return num[length - i] < number.num[number.length - i];
            }
            return length < number.length;
        }
        return length + power < number.length + number.power;
    }
    else //全为负数
    {
        if (length + power == number.length + number.power) //最高位数相同
        {
            for (int i = 1; i <= length && i <= number.length; i++) //从最高位依次比较，若不相等则返回大小关系
            {
                if (num[length - i] == number.num[number.length - i])
                    continue;
                return num[length - i] > number.num[number.length - i];
            }
            return length > number.length;
        }
        return length + power > number.length + number.power;
    }
}

bool Big_number::operator>(const Big_number &number) const //比较两个数的大小关系,若前者大于后者,返回true
{
    return number < (*this);
}

bool Big_number::operator<=(const Big_number &number) const //比较两个数的大小关系,若前者小于等于后者,返回true
{
    return !((*this) > number);
}

bool Big_number::operator>=(const Big_number &number) const //比较两个数的大小关系,若前者大于等于后者,返回true
{
    return !((*this) < number);
}

bool Big_number::operator==(const Big_number &number) const //比较两个数的大小关系,若前者等于后者,返回true
{
    return !(((*this) > number) || ((*this) < number));
}

Big_number Big_number::operator+(const Big_number &number) const //进行加法计算
{
    Big_number result;
    if (positive == number.positive) //符号相同,将数字位相加
    {
        result.positive = positive;
        int *digits;                                                                                                       //存储加起来之后的数字位结果
        int len;                                                                                                           // digits数组的长度
        int min_digit = std::min(power, number.power), max_digit = std::max(length + power, number.length + number.power); //计算出加法后最低位和最高位的位置
        len = max_digit - min_digit + 1;                                                                                   //计算出数位加法后的位数
        digits = new int[len];
        memset(digits, 0, len * sizeof(int)); //将digits清零
        for (int i = 0; i < length; i++)
        {
            digits[power + i - min_digit] += num[i];
        }
        for (int i = 0; i < number.length; i++)
        {
            digits[number.power + i - min_digit] += number.num[i];
        }
        for (int i = 0; i < len - 1; i++)
        {
            digits[i + 1] += digits[i] / 10;
            digits[i] %= 10;
        }
        result.power = 0;
        while (result.power < len && digits[result.power] == 0) //去除后导0
        {
            result.power++;
        }
        if (result.power == len) // result为0
        {
            result.clear();
            return result;
        }
        while (digits[len - 1] == 0) //去除前导0
        {
            len--;
        }
        result.length = len - result.power;
        delete[] result.num;
        result.num = new int[result.length];
        for (int i = 0; i < result.length; i++)
        {
            result.num[i] = digits[i + result.power];
        }
        result.power += min_digit;
        delete[] digits;
    }
    else //符号位相反
    {
        int *digits;                                                                                                       //存储加起来之后的数字位结果
        int len;                                                                                                           // digits数组的长度
        int min_digit = std::min(power, number.power), max_digit = std::max(length + power, number.length + number.power); //计算出加法后最低位和最高位的位置
        len = max_digit - min_digit + 1;                                                                                   //计算出数位加法后的位数
        digits = new int[len];
        memset(digits, 0, len * sizeof(int)); //将digits清零
        for (int i = 0; i < length; i++)
        {
            digits[power + i - min_digit] += num[i] * (positive ? 1 : -1);
        }
        for (int i = 0; i < number.length; i++)
        {
            digits[number.power + i - min_digit] += number.num[i] * (number.positive ? 1 : -1);
        }
        for (int i = 0; i < len - 1; i++)
        {
            if (digits[i] < 0)
            {
                digits[i] += 10;
                digits[i + 1]--;
            }
        }
        result.power = 0;
        while (result.power < len && digits[result.power] == 0) //去除后导0
        {
            result.power++;
        }
        if (result.power == len) // result为0
        {
            result.clear();
            return result;
        }
        while (digits[len - 1] == 0) //去除前导0
        {
            len--;
        }
        result.positive = true;
        if (digits[len - 1] < 0) //该数为负数,将每一位反转
        {
            result.positive = false;
            for (int i = result.power; i < len; i++)
                digits[i] *= -1;
            for (int i = 0; i < len - 1; i++)
            {
                if (digits[i] < 0)
                {
                    digits[i] += 10;
                    digits[i + 1]--;
                }
            }
        }
        while (digits[len - 1] == 0) //去除前导0
        {
            len--;
        }
        result.length = len - result.power;
        delete[] result.num;
        result.num = new int[result.length];
        for (int i = 0; i < result.length; i++)
        {
            result.num[i] = digits[i + result.power];
        }
        result.power += min_digit;
        delete[] digits;
    }
    return result;
}

Big_number Big_number::operator-(const Big_number &number) const //进行减法计算,a-b就是a+(-b)
{
    Big_number copy;
    copy.clone(number);
    copy.positive = !copy.positive;
    return (*this) + copy;
}

Big_number Big_number::operator*(const Big_number &number) const //进行乘法计算
{
    Big_number result;
    int *digits;
    int len;
    result.positive = !(positive ^ number.positive);
    mul(num, length, number.num, number.length, digits, len);
    for (int i = 0; i < len - 1; i++)
    {
        digits[i + 1] += digits[i] / 10;
        digits[i] %= 10;
    }
    result.power = 0;
    while (result.power < len && digits[result.power] == 0) //去除后导0
    {
        result.power++;
    }
    if (result.power == len)
    {
        result.clear();
        return result;
    }
    while (digits[len - 1] == 0) //去除前导0
    {
        len--;
    }
    result.length = len - result.power;
    delete[] result.num;
    result.num = new int[result.length];
    for (int i = 0; i < result.length; i++)
    {
        result.num[i] = digits[i + result.power];
    }
    delete[] digits;
    result.power += power + number.power;
    return result;
}

Big_number Big_number::half() const //返回该数除2的结果(除2就是乘0.5)
{
    Big_number result;
    result.positive = positive;
    if (num[0] & 1)
    {
        result.power = power - 1;
        if (num[length - 1] == 1)
        {
            delete[] result.num;
            result.num = new int[length];
            result.length = length;
            for (int i = 1; i < length; i++)
            {
                result.num[i] = (num[i] & 1) * 5;
                result.num[i] += num[i - 1] / 2;
            }
            result.num[0] = 5;
        }
        else //末尾5首位多1位
        {
            delete[] result.num;
            result.num = new int[length + 1];
            result.length = length + 1;
            for (int i = 1; i <= length - 1; i++)
            {
                result.num[i] = (num[i] & 1) * 5;
                result.num[i] += num[i - 1] / 2;
            }
            result.num[0] = 5;
            result.num[length] = num[length - 1] / 2;
        }
    }
    else
    {
        result.power = power;
        if (num[length - 1] == 1)
        {
            delete[] result.num;
            result.num = new int[length - 1];
            result.length = length - 1;
            for (int i = 0; i < length - 1; i++)
            {
                result.num[i] = (num[i + 1] & 1) * 5;
                result.num[i] += num[i] / 2;
            }
        }
        else
        {
            delete[] result.num;
            result.num = new int[length];
            result.length = length;
            for (int i = 0; i < length - 1; i++)
            {
                result.num[i] = num[i] / 2;
                result.num[i] += (num[i + 1] & 1) * 5;
            }
            result.num[length - 1] = num[length - 1] / 2;
        }
    }
    return result;
}

Big_number Big_number::multiply() const //返回该数乘2的结果
{
    Big_number result;
    result.positive = positive;
    int *digits = new int[length + 1];
    digits[length] = 0;
    for (int i = 0; i < length; i++)
        digits[i] = num[i] * 2;
    for (int i = 0; i < length; i++)
    {
        digits[i + 1] += digits[i] / 10;
        digits[i] %= 10;
    }
    result.length = length + 1;
    while (result.length > 0 && digits[result.length - 1] == 0)
    {
        result.length--;
    }
    if (!result.length)
    {
        result.clear();
        return result;
    }
    result.power = 0;
    while (digits[result.power] == 0)
        result.power++;
    result.length -= result.power;
    result.num = new int[result.length];
    for (int i = 0; i < result.length; i++)
        result.num[i] = digits[i + result.power];
    result.power += power;
    delete[] digits;
    return result;
}

Big_number Big_number::operator/(const Big_number &number) const //通过牛顿迭代实现
{
    Big_number result=Big_number(0);
    Big_number new_result=Big_number(1);
    new_result.power=-(number.power+number.length);
    Big_number eps;
    eps.change_to("1e-105");
    while(abs(result-new_result)>eps)
    {
        result=new_result;
        new_result=result.multiply()-number*(result*result);
    }
    result=new_result*(*this);
    result.fixed();
    return result;
}

Big_number Big_number::sqrt() const //返回开根的结果
{
    Big_number l, r,eps;
    l = Big_number(0);
    r = Big_number(1);
    eps.change_to("1e-105");
    if ((*this) < Big_number(0))
        return Big_number(0);
    while ((r * r) < (*this))
    {
        l = r;
        r = (r * 2);
    }
    int cnt = 0;
    while ((r-l)>eps)
    {
        Big_number mid = (l + r);
        mid = mid.half();
        if ((*this) < (mid * mid))
        {
            r = mid;
        }
        else
        {
            l = mid;
        }
        cnt++;
    }
    l.fixed();
    return l;
}

Big_number Big_number::pow(const Big_number &number) const //返回该数的number次方
{
    bool flag = false;
    Big_number result = Big_number(1);
    Big_number index = Big_number(1);
    Big_number eps;
    eps.change_to("1e-105");
    Big_number now;
    now = (*this);
    Big_number copy;
    copy = number;
    if (!number.positive)
    {
        flag = true;
        copy.positive = true;
    }
    while (index < number)
    {
        index = index.multiply();
        now = (now * now);
    }
    index.fixed();
    now.fixed();
    while (copy>eps)
    {
        if (index <= copy)
        {
            copy = (copy - index);
            result = (result * now);
        }
        index = index.half();
        now = now.sqrt();
    }
    if (flag)
        return Big_number(1) / result;
    return result;
}

Big_number max(const Big_number &number1, const Big_number &number2) //返回较大值
{
    if (number1 < number2)
        return number2;
    return number1;
}

Big_number min(const Big_number &number1, const Big_number &number2) //返回较小值
{
    if (number1 < number2)
        return number1;
    return number2;
}

Big_number abs(const Big_number &number) //返回绝对值
{
    Big_number result;
    result.clone(number);
    result.positive = true;
    return result;
}

std::ostream &operator<<(std::ostream &out, const Big_number &num) //重载输出流
{
    if (!num.positive) //如果是负数,则先输出负号
        out << "-";
    if (num.length >= 30 || num.power + num.length >= 30 || num.power <= -30) //该数的位数比20大时,使用科学计数法
    {
        out << num.num[num.length - 1];
        if (num.length > 30)
        {
            out << ".";
            for (int i = 1; i <= 29; i++)
                out << num.num[num.length - 1 - i];
        }
        else if (num.length > 1)
        {
            out << ".";
            for (int i = num.length - 2; i >= 0; i--)
                out << num.num[i];
        }
        else
        {
            out << ".0";
        }
        if (num.power + num.length - 1)
        {
            out << "e";
            out << num.power + num.length - 1;
        }
    }
    else if (num.length + num.power <= 0) //判断该数是否小于1
    {
        out << "0.";
        for (int i = num.length + num.power; i < 0; i++)
            out << "0";
        for (int i = num.length - 1; i >= 0; i--)
            out << num.num[i];
    }
    else if (num.power < 0) //判断该数是否为小数
    {
        for (int i = num.length - 1; i >= -num.power; i--)
            out << num.num[i];
        out << ".";
        for (int i = -num.power - 1; i >= 0; i--)
            out << num.num[i];
    }
    else //该数为整数
    {
        for (int i = num.length - 1; i >= 0; i--)
            out << num.num[i];
        for (int i = 1; i <= num.power; i++)
            out << "0";
    }
    return out;
}
