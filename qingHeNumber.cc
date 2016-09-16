/**
 * 计算500万以内的亲和数，使用伴随数组
 * 数组中保存的是下标对应的真因子的和
 */
#include <iostream>
#include <cstdio>
using namespace std;

constexpr long long scale = 5000000;
/* 需要的辅助内存太大，不能放在栈中，只能放在全局静态存储区 */
long long sum[scale + 10]; //为防越界

void amicable_number()
{
    /* 统计亲和数的规模 */
    for (long long i = 0; i < scale; ++i)
    {
        sum[i] = 1;
    }
    for (long long i = 2; i + i < scale; ++i)
    {
        /* i是所有i的倍数的真因子 */
        for(long long j = i+i; j < scale; j += i)
        {
            sum[j] += i;
        }
    }
    /* 扫描保存的真因数之和的数组，查找数组元素值相等的两个下标*/
    for (long long i = 1; i < scale; ++i)
    {
        /* sum[i] > i是为了保证不重复计算 */
        if(sum[i] > i && sum[i] < scale && sum[sum[i]] == i)
        {
            // std::cout << i << " " << sum[i] << std::endl;
            // 打印long long类型必须使用%lld
            printf("%lld %lld\n", i, sum[i]);
        }
    }

}

int main()
{
    amicable_number();
    return 0;
}