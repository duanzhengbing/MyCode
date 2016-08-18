#include <string>
#include <map>
#include <set>
#include <algorithm>
#include "sort.h"
#include <mutex>
#include <thread>
using namespace std;

/**
 * 字符串左旋转 - 暴力法，时间复杂度为O(m*n)
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
 * 将待左旋转的字符串依次左移，时间复杂度为O(n)
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
     * 尾部子串长度小于移位长度，需单独处理
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
 * 首先对序列前部分逆序，再对序列后部分逆序，再对整个序列全部逆序
 * 时间复杂度是O(m/2 + (n-m)/2 + O(n)) = O(n)
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
 * 最大公约数，欧几里德辗转相除法
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
 * 终极大招，STL中rotate的方法，利用了最大公约数。
 * 从整体考虑，字符串右移的时候，前面的方法每个字符都是经过若干次
 * 的移动才到达最终正确的位置，如果能够一次就能将某一个字符放在正
 * 确的位置，那么最终只需要O(n)次移动，效率是最高的。比如：
 * abcd -> dabc, 3->0, 2->3, 1->2, 0->1 总共移动了四次。
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
 * 思想是：在第一个位置上挖个洞，并保存该洞的值为临时值，从其他位置
 * 寻找应该存放在该洞中的数，寻找的方法是以步长m一次向右循环查找，找
 * 到之后，将该位置上的值填入洞中，并设置该位置为新的洞，再次寻找。
 * 直至找到的位置和原始的洞的位置重合了为止。
 * 可以证明原始洞的个数是数据长度和左移位数的最小公约数。每个原始洞都
 * 遍历完成之后，所有的数据都在它左移之后最终的位置，每个数一步移动到位。
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
        /* i+m为i右移m的位置，%n表示当i+m>n时，从左重新开始 */
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
 * 时间复杂度为O(n*k)
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
    /*建堆时间O(n) */
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
 * 时间复杂度为O(n + k*logn)
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
        // 堆顶元素下滤，堆的规模减1
        heapAdjust(v, len - i, 0, std::less<int>());
        ret.push_back(v[0]); // 每次都取堆顶元素
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
