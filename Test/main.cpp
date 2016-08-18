#include <string>
#include <map>
#include <set>
#include <algorithm>
#include "sort.h"
#include <mutex>
#include <thread>
using namespace std;

/**
 * �ַ�������ת - ��������ʱ�临�Ӷ�ΪO(m*n)
 */

void rotate1(string& str,const int& shift_n) noexcept
{
    int len = str.size();
    if(len == 0 || shift_n < 0)
        return;
    int shift = shift_n % len;

    for (int i = 0; i < shift; ++i)
    {
        char tmp = str[0];
        for (int j = 0; j < len-1; ++j)
            str[j] = str[j+1];

        str[len - 1] = tmp;
    }
}
/**
 * use the member function "substr" of the std::string
 */
void rotate2(string& str,const int& shift_n) noexcept
{
    int len = str.size();
    if(len == 0 || shift_n < 0)
        return;
    int shift = shift_n % len;
    string tmp = str.substr(shift,len - shift) + str.substr(0,shift);
    str = tmp;
}

/**
 * ��������ת���ַ����������ƣ�ʱ�临�Ӷ�ΪO(n)
 * abc edf ghi j
 * edf abc ghi j
 * edf ghi abc j
 * edf ghi jabc
 */
void rotate3(string& str,const int& shift_n) noexcept
{
    int len = str.size();
    if(len == 0 || shift_n < 0)
        return;

    int shift = shift_n % len;
    int p1 = 0;
    int p2 = p1 + shift;
    while(p2 + shift <= len)
    {
        for (int i = 0; i < shift; ++i)
        {
            std::swap(str[p1++],str[p2++]);
        }
    }

    /**
     * β���Ӵ�����С����λ���ȣ��赥������
     */
    while(p2 < len)
    {
        char tmp = str[p2];
        for (int j = p2; j >= p1; --j)
        {
            str[j] = str[j-1];
        }
        str[p1++] = tmp;
        p2++;
    }
}

/**
 * ���ȶ�����ǰ���������ٶ����к󲿷������ٶ���������ȫ������
 * ʱ�临�Ӷ���O(m/2 + (n-m)/2 + O(n)) = O(n)
 */
void reverseStr(string& str, int begin, int end)
{
    while(begin < end)
        std::swap(str[begin++], str[end--]);
}

void rotate4(string& str, const int& shift_n)
{
    int len = str.size();
    if(len == 0 || shift_n < 0)
        return;
    int shift = shift_n % len;
    reverseStr(str, 0, shift - 1);
    reverseStr(str, shift, len - 1);
    reverseStr(str, 0, len - 1);
}




/**
 * ���Լ����ŷ�����շת�����
 */

 int gcd(int m, int n)
 {
    if(m <= 0 || n <= 0)
        return 0;

    int tmp = m % n;
    while(tmp != 0)
    {
        m = n;
        n = tmp;
        tmp = m % n;
    }

    return n;
 }

/**
 * �ռ����У�STL��rotate�ķ��������������Լ����
 * �����忼�ǣ��ַ������Ƶ�ʱ��ǰ��ķ���ÿ���ַ����Ǿ������ɴ�
 * ���ƶ��ŵ���������ȷ��λ�ã�����ܹ�һ�ξ��ܽ�ĳһ���ַ�������
 * ȷ��λ�ã���ô����ֻ��ҪO(n)���ƶ���Ч������ߵġ����磺
 * abcd -> dabc, 3->0, 2->3, 1->2, 0->1 �ܹ��ƶ����ĴΡ�
 */
void rotate5(string& str, int shift_n)
{
    int n = str.length();
    if(n == 0 || shift_n < 0)
        return;
    int m = shift_n % n;

    int d = gcd(m, n);
    for (int i = 0; i < d; ++i)
    {
        char tmp = str[i];
        int hole = i;
        for (int j = (i + m) % n; j != i; j = (j + m) % n)
        {
            str[hole] = str[j];
            hole = j;
        }
        str[hole] = tmp;
    }
}

/**
 * ˼���ǣ��ڵ�һ��λ�����ڸ�����������ö���ֵΪ��ʱֵ��������λ��
 * Ѱ��Ӧ�ô���ڸö��е�����Ѱ�ҵķ������Բ���mһ������ѭ�����ң���
 * ��֮�󣬽���λ���ϵ�ֵ���붴�У������ø�λ��Ϊ�µĶ����ٴ�Ѱ�ҡ�
 * ֱ���ҵ���λ�ú�ԭʼ�Ķ���λ���غ���Ϊֹ��
 * ����֤��ԭʼ���ĸ��������ݳ��Ⱥ�����λ������С��Լ����ÿ��ԭʼ����
 * �������֮�����е����ݶ���������֮�����յ�λ�ã�ÿ����һ���ƶ���λ��
 * @param begin [description]
 * @param mid   [description]
 * @param end   [description]
 */
void rotate(char* begin, char* mid, char* end)
{
    int m = mid - begin;
    int n = end - begin;
    if(m <= 0 || n <= 0)
        return;

    int d = gcd(m, n);
    for (int i = 0; i < d; ++i)
    {
        char tmp = *(begin + i);
        int hole = i;
        /* i+mΪi����m��λ�ã�%n��ʾ��i+m>nʱ���������¿�ʼ */
        for (int j = (i + m) % n; j != i; j = (j + m) % n)
        {
            *(begin + hole) = *(begin + j);
            hole = j;
        }
        *(begin + hole) = tmp;
    }
}

void rotateRight(string& str, int shift_n)
{
    int n = str.length();
    if(n == 0 || shift_n < 0)
        return;
    int m = shift_n % n;
    m = n - m;
    rotate5(str, m);
}


inline void adjust(std::vector<int>& v)
{
    for (unsigned i = 0; i < v.size()-1; ++i)
    {
        if(v[i] > v[i+1])
            std::swap(v[i], v[i+1]);
    }
}
/**
 * ʱ�临�Ӷ�ΪO(n*k)
 */
std::vector<int> getLeastNumbers(std::vector<int>& v,const int k)
{
    std::vector<int> ret;
    int len = v.size();
    if(len == 0 || k <= 0)
        return ret;
    if(len <= k)
        return v;
    ret = std::vector<int>(v.begin(),v.begin()+k);
    adjust(ret);
    for (int i = k; i < len; ++i)
    {
        if(v[i] < ret.back())
            std::swap(ret.back(),v[i]);
        adjust(ret);
    }
    return ret;
}

std::vector<int> getLeastNumbers2(std::vector<int>& v,const int k)
{
    std::vector<int> ret;
    int len = v.size();
    if(len == 0 || k <= 0)
        return ret;
    if(len <= k)
        return v;

    std::multiset<int,std::greater<int>> mset(v.begin(),v.begin() + k);
    for (int i = k; i < len; ++i)
    {
        if(v[i] < *mset.begin())
        {
            mset.erase(mset.begin());
            mset.insert(v[i]);
        }
    }
    ret = std::vector<int>(mset.begin(),mset.end());
    return ret;
}


std::vector<int> getLeastNumbers3(std::vector<int>& v,const int k,
    std::function<bool(int,int)> compare = std::greater<int>())
{
    std::vector<int> ret;
    int len = v.size();
    if(len == 0 || k <= 0)
        return ret;
    if(len <= k)
        return v;

    ret = std::vector<int>(v.begin(),v.begin() + k);
    /*����ʱ��O(n) */
    buildHeap(ret,compare);
    for (int i = k; i < len; ++i)
    {
        if(!compare(v[i], *ret.begin()))
        {
            std::swap(v[i],*ret.begin());
            heapAdjust(ret,ret.size(),0,compare);
        }
    }

    return ret;
}

std::vector<int> getLeastNumbers4(std::vector<int>& v,const int k)
{
    std::vector<int> ret;
    int len = v.size();
    if(len == 0 || k <= 0)
        return ret;
    if(len <= k)
        return v;


    int lo = 0;
    int hi = len - 1;
    int mid = partition(v, lo, hi);
    while(mid != k - 1)
    {
        if(mid < k - 1)
        {
            lo = mid + 1;
            mid = partition(v, lo, hi);
        }
        else
        {
            hi = mid - 1;
            mid = partition(v, lo, hi);
        }
    }

    ret = std::vector<int>(v.begin(),v.begin()+k);
    return ret;
}
/**
 * ʱ�临�Ӷ�ΪO(n + k*logn)
 */
std::vector<int> getLeastNumbers5(std::vector<int>& v,const int k)
{
    std::vector<int> ret;
    int len = v.size();
    if(len == 0 || k <= 0)
        return ret;
    if(len <= k)
        return v;

    buildHeap(v,std::less<int>());
    print(v);
    ret.push_back(v[0]);
    for (int i = 1; i < k; ++i)
    {
        std::swap(v[0], v[len - i]);
        // �Ѷ�Ԫ�����ˣ��ѵĹ�ģ��1
        heapAdjust(v, len - i, 0, std::less<int>());
        ret.push_back(v[0]); // ÿ�ζ�ȡ�Ѷ�Ԫ��
    }
    return ret;
}

int main(int argc, char const *argv[])
{
    std::vector<int> v;
    for (int i = 0; i < 100; ++i)
    {
        v.push_back(rand() % 100);
    }
    print(v);
    std::vector<int> result2 = getLeastNumbers2(v, 10);
    print(result2);
    // std::vector<int> result1 = getLeastNumbers3(v, 10,std::less<int>());
    std::vector<int> result1 = getLeastNumbers5(v, 10);
    print(result1);
    // print(v);
    return 0;
}
