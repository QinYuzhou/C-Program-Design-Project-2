#pragma once

const int mod = 998244353;

long long qpow(long long x, int y) //计算x的y次方
{
    long long ret = 1;
    while (y)
    {
        if (y & 1)
            ret = ret * x, ret %= mod;
        x *= x, x %= mod;
        y >>= 1;
    }
    return ret;
}

void init(int x, int &n, int *&rev, long long *&cof_to_val, long long *&val_to_cof) //对于结果长度小于等于x的数组预处理，其中n为数组长度，rev为二进制下反转的结果，cof_to_val为系数表达式转为点值表达式时需要用到的数组，val_to_cof为点值表达式转化为系数表达式时需要用到的数组
{
    n = 1;
    while (n < x)
        n <<= 1;
    rev = new int[n];
    rev[0] = 0;
    for (int i = 1; i < n; i++)
        rev[i] = ((rev[i >> 1] >> 1) | ((n >> 1) * (i & 1)));
    int half = (n >> 1);
    cof_to_val = new long long[n];
    val_to_cof = new long long[n];
    cof_to_val[half] = val_to_cof[half] = 1;
    long long y[2];
    y[0] = qpow(3, (mod - 1) / n);
    y[1] = qpow(y[0], mod - 2);
    for (int i = 1; i < half; i++)
    {
        cof_to_val[i + half] = (cof_to_val[i + half - 1] * y[0]) % mod;
        val_to_cof[i + half] = (val_to_cof[i + half - 1] * y[1]) % mod;
    }
    for (int i = half - 1; i > 0; i--)
    {
        cof_to_val[i] = cof_to_val[i << 1];
        val_to_cof[i] = val_to_cof[i << 1];
    }
    return;
}

void coefficient_to_value(long long *v, int n, const int *rev, const long long *cof_to_val) //将数组v从系数表达式转化为点值表达式
{
    for (int i = 0; i < n; i++)
        if (rev[i] < i)
            std::swap(v[rev[i]], v[i]);
    for (int i = 1; i < n; i <<= 1)
        for (int j = 0; j < n; j += (i << 1))
            for (int k = 0; k < i; k++)
            {
                long long x = v[j + k], y = v[j + k + i] * cof_to_val[i + k] % mod;
                v[j + k] = (x + y) % mod;
                v[j + k + i] = (x - y + mod) % mod;
            }
    return;
}

void value_to_coefficient(long long *v, int n, const int *rev, const long long *val_to_cof) //将数组v从点值表达式转化为系数表达式
{
    for (int i = 0; i < n; i++)
        if (rev[i] < i)
            std::swap(v[rev[i]], v[i]);
    for (int i = 1; i < n; i <<= 1)
        for (int j = 0; j < n; j += (i << 1))
            for (int k = 0; k < i; k++)
            {
                long long x = v[j + k], y = v[j + k + i] * val_to_cof[i + k] % mod;
                v[j + k] = (x + y) % mod;
                v[j + k + i] = (x - y + mod) % mod;
            }
    long long inv = qpow(n, mod - 2);
    for (int i = 0; i < n; i++)
        v[i] *= inv, v[i] %= mod;
    return;
}

void mutiply(const int *num1, int len1, const int *num2, int len2, int *&ret, int &ret_len)
{
    long long *cof_to_val;
    long long *val_to_cof;
    int *rev;
    int n;
    init(len1 + len2 - 1, n, rev, cof_to_val, val_to_cof);
    long long *A = new long long[n];
    long long *B = new long long[n];
    for (int i = 0; i < len1; i++)
        A[i] = num1[i];
    for (int i = len1; i < n; i++)
        A[i] = 0;
    for (int i = 0; i < len2; i++)
        B[i] = num2[i];
    for (int i = len2; i < n; i++)
        B[i] = 0;
    coefficient_to_value(A, n, rev, cof_to_val);
    coefficient_to_value(B, n, rev, cof_to_val);
    for (int i = 0; i < n; i++)
        A[i] = A[i] * B[i] % mod;
    value_to_coefficient(A, n, rev, val_to_cof);
    ret_len = len1 + len2 - 1;
    ret = new int[ret_len];
    for (int i = 0; i < ret_len; i++)
        ret[i] = A[i] % mod;
    delete[] rev;
    delete[] cof_to_val;
    delete[] val_to_cof;
    delete[] A;
    delete[] B;
    return;
}
