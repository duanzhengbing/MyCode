#include <string>
#include <map>
#include <set>
#include <list>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <iomanip>
#include "sort.h"

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
        // i+m为i右移m的位置，%n表示当i+m>n时，从左重新开始
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
    for (int i = 0; i < v.size()-1; ++i)
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
    // 建堆时间O(n)
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

double sqrt2()
{
    double num = 2.0;
    double lo = 1.0;
    double hi = num;
    double epsinon = 0.000000000000001;
    while(lo < hi)
    {
        double mid = (lo + hi) / 2.0;
        double tmp = mid * mid;
        if(abs(num - tmp) < epsinon)
            return mid;
        if(tmp > num)
            hi = mid;
        else
            lo = mid;
    }

}


std::pair<int, int> findSumOfTwoNumber(const std::vector<int>& v,const int& sum)
{
    unsigned length = v.size();
    if(length < 2)
        return std::pair<int,int>();

    unordered_map<int, int> hash;
    for (int i = 0; i < length; ++i)
    {
        hash[v[i]]++;
    }
    for (int i = 0; i < length; ++i)
    {
        int tmp = sum - v[i];
        if(hash.count(tmp) && (hash[tmp] > 1 || tmp != v[i]))
        {
            return std::pair<int,int>(v[i], tmp);                
        }
    }

    return std::pair<int,int>();
}

std::pair<int,int> findSumOfTwoNumberII(std::vector<int>& v, const int sum)
{
    int length = v.size();
    if(length < 2)
        return std::pair<int,int>();
    sort(v.begin(), v.end());

    int i = 0, j = length - 1;
    while(i < j)
    {
        if(v[i] + v[j] > sum)
            j--;
        else if(v[i] + v[j] < sum)
            i++;
        else
            return std::pair<int,int>(v[i], v[j]);
    }
    return std::pair<int,int>();
}

list<int> nums;
void find_factor(int n, int sum, std::vector<vector<int>>& ret)
{
    // 递归出口
    if(n <= 0 || sum <= 0 || n*(n+1)/2 < sum)
        return;

    // 找到的结果
    if(sum == n)
    {
        // 反转list
        // nums.reverse();
        std::vector<int> tmp;
        for(auto iter = nums.begin(); iter != nums.end(); iter++)
            tmp.push_back(*iter);
        tmp.push_back(n);
        ret.push_back(tmp);
        // nums.reverse();
    }

    nums.push_back(n); //典型的01 背包问题
    find_factor(n - 1, sum - n, ret); //放n，n-1 个数填满sum-n
    nums.pop_back();
    find_factor(n - 1, sum, ret); //不放n，n-1 个数填满sum
}

template<typename InputIterator, typename Con>
void longest_common_seq_recur(InputIterator a_first, InputIterator a_last,
                   InputIterator b_first, InputIterator b_last,
                   Con& ret)
{
    if(a_first == a_last || b_first == b_last)
        return;
    if(*(a_last - 1) == *(b_last - 1))
    {
        longest_common_seq_recur(a_first, a_last - 1, b_first, b_last - 1, ret);
        ret.push_back(*(a_last - 1));
    }
    else
    {
        Con tmpa = ret;
        Con tmpb = ret;
        longest_common_seq_recur(a_first, a_last, b_first, b_last - 1, tmpa);
        longest_common_seq_recur(a_first, a_last - 1, b_first, b_last, tmpb);
        if(tmpa.size() > tmpb.size())
            ret = tmpa;
        else
            ret = tmpb;
    }
}

template<typename Con>
Con longest_common_seq(Con A, Con B)
{
    Con ret;
    longest_common_seq_recur(A.begin(), A.end(), B.begin(), B.end(), ret);
    return ret;
}

template<typename Con>
Con longest_inc_seq(Con C)
{
    Con ret;
    Con tmp = C;
    sort(C.begin(),C.end());
    longest_common_seq_recur(C.begin(), C.end(), tmp.begin(),tmp.end(),ret);
    return ret;
}

int main(int argc, char const *argv[])
{
    std::vector<int> v;
    int count = 10;
    for (int i = 0; i < count; ++i)
    {
        v.push_back(rand() % count);
    }
    print(v);


    // std::vector<std::vector<int>> ret;
    // find_factor(10, 20, ret);
    // for (int i = 0; i < ret.size(); ++i)
    // {
    //     print(ret[i]);
    // }
    
    std::vector<int> v1{1,2,3,5,6,8};
    std::vector<int> v2{8,2,3,4,5,6};
    string str1 = "BDCABA";
    string str2 = "ABCBDAB";
    // std::vector<int> ret = longest_common_seq(v1, v2);
    // string retStr = longest_common_seq(str1, str2);
    // print(ret);
    // print(retStr);

    string v3 = longest_inc_seq(str2);
    print(v3);

    return 0;
}