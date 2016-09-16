#include <string>
#include <cstring>
#include <map>
#include <set>
#include <list>
#include <queue>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <iomanip>
#include <climits>
#include "sort.h"
#include <cmath>

using namespace std;

template<typename Container>
void printVector(const Container& Con)
{
    for (auto it = Con.begin(); it != Con.end(); ++it)
    {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
}

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


// template<typename Con>
// Con longest_common_seq(Con A, Con B)
// {
//     Con ret;
//     int lenA = A.size();
//     int lenB = B.size();
//     if(lenA == 0 || lenB == 0)
//         return ret;


// }


template <typename T>
int longest_common_seq(const T& con1, const T& con2)
{
    int len1 = con1.size();
    int len2 = con2.size();
    if(len1 == 0 || len2 == 0)
        return 0;

    std::vector<vector<int>> v(len1 + 1, std::vector<int>(len2 + 1, 0));
    for (int i = 1; i <= len1; ++i)
    {
        for (int j = 1; j <= len2; ++j)
        {
            if(con1[i-1] == con2[j-1])
                v[i][j] = v[i-1][j-1] + 1;
            else
            {
                v[i][j] = max(v[i][j-1], v[i-1][j]);
            }
        }
    }

    return v[len1][len2];
}


/**
 * 
 */
template<typename Container>
Container longest_inc_seq(const Container& Con)
{
    Container tmp;
    int len = Con.size();
    if(len == 0)
        return tmp;

    for (auto it = Con.begin(); it != Con.end(); ++it)
    {
        auto iter = std::lower_bound(tmp.begin(), tmp.end(), *it);
        if(iter == tmp.end())
            tmp.push_back(*it);
        else
            *iter = *it;
    }

    return tmp; 
}

// std::vector<int> longest_inc_seq(const std::vector<int>& v)
// {
//     int len = v.size();
//     if(len == 0)
//         return std::vector<int>();
//     std::vector<int> tmp;
//     for (int i = 0; i < len; ++i)
//     {
//         auto iter = std::lower_bound(tmp.begin(), tmp.end(), v[i]);
//         if(iter == tmp.end())
//             tmp.push_back(v[i]);
//         else
//             *iter = v[i];
//     }

//     return tmp;
// }

int maxSumOfContinousArray(const std::vector<int>& v)
{
    int len = v.size();
    if(len == 0)
        return INT_MIN;
    int maxSum = v[0];
    int curSum = v[0];
    
    int begin = 0, end = 0;

    for (int i = 1; i < len; ++i)
    {
        if(curSum <= 0)
        {
            curSum = v[i];
            if(curSum > maxSum)
                begin = i;
        }
        else
        {
            curSum += v[i];
        }

        if(curSum > maxSum)
        {
            maxSum = curSum;
            end = i;
        }

    }

    for (int i = begin; i <= end; ++i)
    {
        cout << v[i] << " ";
    }
    cout << endl;

    return maxSum;
}

string rstr(string str)
{
    int i = 0;
    int j = str.size() - 1;
    while(i < j)
    {
        std::swap(str[i++], str[j--]);
    }

    return str;
}


void foo(std::vector<int> v, int k)
{
    int len = v.size();
    int n = len /2;
    int num =  std::pow(2, k);
    // std::vector<int> tmp(len, 0 );
    // for (int i = 0; i < n; ++i)
    // {
    //     int val = i * num;
    //     if(val >= len)
    //         val = val % (len - 1);

    //     tmp[val] = v[i]; 
    //     // tmp[len - val -1] = v[len - i - 1]; 
    // }
    // for (int i = len - 1; i >= n; --i)
    // {
    //     int val = (len - i - 1) * num;
    //     if(val >= len)
    //         val = val % (len - 1);

    //     tmp[len - val - 1] = v[i]; 
    //     // tmp[len - val -1] = v[len - i - 1]; 
    // }

    int tmp = v[1];
    int i = 1;
    for (int k = 0; k < len-1; ++k)
    {
        int idx = 0;
        int val = 0;
        if(i < n)
        {
            val = i*num;
            if(val >= len)
                val = val % (len - 1);
            idx = val;
        }
        else
        {
            val = (len - i - 1) * num;
            if(val >= len)
                val = val % (len - 1);
            idx = len - val - 1;
        }

        v[idx] = tmp;
        tmp = v[idx];
        i = idx;
    }

    for (int k = 0; k < len; ++k)
    {
        std::cout << v[k] << " ";
    }
    cout << endl;
}

void washcard(std::vector<int>& v, int k)
{
    int len = v.size();
    std::vector<int> tmp(len, 0);
    int n = len / 2;
    for (int i = 0; i < k; ++i)
    {
        int j = 0;
        int k = 0;
        while(j < len - 1)
        {
            tmp[j] = v[k];
            tmp[j+1] = v[k+n];
            j = j+2;
            k++;
        }

        v = tmp;
    }

    for (int i = 0; i < len; ++i)
    {
        /* code */
        cout << v[i] << " ";
    }
    cout << endl;
}

#include <deque>
void func(int n)
{
    std::vector<int> v(n, 0);
    int i = 0;
    int num = 1;
    int empty = 0;
    while(true)
    {
        if(i == n)
            i = 0;
        if(num > n)
            break;
        if(v[i] == 0)
            empty++;
        if(empty == 2)
        {
            v[i] = num++;
            empty = 0;
        }

        i++;
    }
    for (int j = 0; j < v.size(); ++j)
    {
        cout << v[j] << " ";
    }
    cout << endl;
}


int luckypacket(std::vector<int>& v, int i, int& sum, int& multi)
{
    if(i <= 0)
    {
        sum = 0;
        multi = 0;
        return 0;
    }
    if(i == 1)
    {
        sum = v[1];
        multi = v[1];
        return 0;
    }
    
    int tmp_sum = 0;
    int tmp_multi = 1;
    int val = luckypacket(v, i-1, tmp_sum, tmp_multi);
    if(tmp_sum + v[i-1]> tmp_multi*v[i-1])
    {
        val++;
    }
    multi = tmp_multi*v[i-1];
    sum = tmp_sum + v[i-1];

    return val;
}

int grid(int W, int H)
{
    if(W <=0 || H <= 0)
        return 0;
    int ret = 0;

    int num1  = 0, num2 = 0;
    int val = W % 4;
    num1 = (W / 4) * 2 + ((val == 3) ? 2 : val);
    num2 = W - num1;

    ret = (H / 4) * (2 * W);
    val = H % 4;
    if(val == 0)
        return ret;
    else if(val == 1 || val == 2)
        return ret + num1*val;
    else
        return ret + W + num1;

}

bool huiwen(string str)
{
    int i = 0;
    int j = str.size()-1;
    while(i < j)
    {
        if(str[i++] != str[j--])
            return false;
    }
    return true;
}

int makehuiwen(string A, string B)
{
    string tmp;
    int ret = 0;
    for (int i = 0; i <= A.size(); ++i)
    {
        tmp = A.substr(0, i) + B + A.substr(i, A.size()-i);
        // cout << tmp << endl;
        if(huiwen(tmp))
            ret++;
    }

    return ret;
}

int eatshell(long long start)
{
    long long shell = 1000000007;
    long long limit = 100000;
    long long ret = 0;
    unordered_map<long long, long int> visit;
    deque<long long> que;
    que.push_back(start % shell);
    visit[start] = 0;
    while(!que.empty())
    {
        long long pos = que.front();
        que.pop_front();
        if(pos == 0)
            return visit[pos];

        int x1 = (pos * 4 + 3) % shell;
        if(visit.count(x1) == 0)
        {
            que.push_back(x1);
            visit[x1] = visit[pos] + 1;
        }
        
        int x2 = (pos * 8 + 7) % shell;
        if(visit.count(x2) == 0)
        {
            que.push_back(x2);
            visit[x2] = visit[pos] + 1;
        }

    } 

    return -1;
}


void isSort(std::vector<string> v)
{
    std::vector<string> tmp = v;
    std::sort(tmp.begin(), tmp.end());
    bool isDict = true;
    bool isLength = true;
    
    for (int i = 0; i < v.size(); ++i)
    {
        if(v[i] != tmp[i])
        {
            isDict = false;
            break;
        }
    }
    
    std::multimap<int,string> mp;
    for (int i = 0; i < v.size(); ++i)
    {
        mp.insert(std::pair<int,string>(v[i].size(), v[i]));
    }

    int j = 0;
    for (std::multimap<int, string>::iterator i = mp.begin(); i != mp.end(); ++i)
    {
        if(i->second != v[j++])
        {
            isLength = false;
            break;
        }
    }

    if(isLength && isDict)
        std::cout<< "both" << endl;
    else if(!isDict && !isLength)
        std::cout<< "none" << endl;
    else if(isDict)
        std::cout<< "lexicographically" << endl;
    else
        std::cout<< "lengths" << endl;


}


int LastRemaining_Solution(unsigned int n, unsigned int m)
{
    queue<int> que;
    for (int i = 0; i < n; ++i)
    {
        que.push(i);
    }
    int ret = 0;
    while(!que.empty())
    {
        int count = m-1;
        while(count-- > 0)
        {
            int tmp = que.front();
            que.pop();
            que.push(tmp);
        }
        ret = que.front();
        que.pop();
    }
    return ret;
}

bool isStr1ContainStr2(string str1, string str2)
{
    int len_1 = str1.size();
    int len_2 = str2.size();

    if(len_1 < len_2)
        return false;
    std::vector<int> table(len_2, 0);
    int i = 1;
    int j = 0;
    while(i < len_2)
    {
        /* 如果后面的某个字符与第一个字符匹配成功，则i，j携手共进，继续匹配
         * 并更新i的匹配值为前一匹配值加1
         */
        if(str2[i] == str2[j])
        {
            j++;
            table[i] = table[i-1] + 1;
        }
        else /* 否则当前字符必须从字符串的起始位置重新匹配，此时匹配值不做更新，即为初始值零 */
            j = 0;

        i++;
    }

    i = 0;
    j = 0;
    while (i < len_1 && j < len_2)
    {
        if(str1[i] == str2[j])
        {
            ++i;
            ++j;
        }
        else
        {
            if(j == 0)
                ++i;
            else
                j = table[j];
        }
    }

    if(j < len_2)
        return false;
    else
        return true;
}

int main(int argc, char const *argv[])
{
    
    string s = "nowcoder";
    string t = "owcode";

    cout << boolalpha << isStr1ContainStr2(s, t) << endl;

    return 0;
}