#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <map>
#include <cassert>
using namespace std;

// int foo(string str)
// {
//     int ret = 1;
//     auto begin = str.rbegin();
//     auto end = str.rend();
//     for (auto i = str.begin(); i < str.end(); ++i)
//     {
        
//         char tmp = *i;
//         auto iter = find(begin++, end--, tmp);
//         if(iter != i)
//         {
//             ret++;
//         }
//     }
//     return ret;
// }

void foo(string& str)
{
    for (int i = str.size()-1; i >= 0; --i)
    {
        if(str[i] >= 'A' && str[i] <= 'Z')
        {
            for (int j = i+1; j < str.size() && (str[j] <'A' || str[j] > 'Z'); ++j)
            {
                std::swap(str[j-1],str[j]);
            }
        }
    }
}

void max_min_Pairs(std::vector<int>& v)
{
    int len = v.size();
    if(len < 2)
    {
        cout << 0 << " " << 0 << endl;
        return;
    }
    std::sort(v.begin(), v.end());
    int maxNums = 1;
    int minNums = 1;
    int maxPairs = 1;
    for (int i = 1; i < len - 1 && v[i] == v[0]; ++i)
    {
        minNums++;
    }
    for (int i = len - 2; i > 0 && v[i] == v[len -1]; --i)
    {
        maxNums++;
    }
    maxPairs = minNums*maxNums;

    map<int, int> mp;
    for (int i = 0; i < len-1; ++i)
    {
        int tmp = v[i+1] - v[i];
        if(tmp != 0)
        {
            if(mp.count(tmp))
                mp[tmp]++;
            else
                mp.insert(std::pair<int,int>(tmp, 1));
        }
        else
            tmp++;
    }

    int minPairs = mp.begin()->second;

    cout << minPairs << " " << maxPairs << endl;
}

int find_cache(std::vector<int> v, int num)
{
    for (int i = 0; i < v.size(); ++i)
    {
        if(v[i] == num)
            return i;
    }
    return -1;
}

int cacheNotHit(int max_cache_size, int* v, int len)
{
    int notHit = 0;
    std::vector<int> cache;
    for (int i = 0; i < len; ++i)
    {
        // if(find(cache.begin(), cache.end(), v[i]) == cache.end())
        if(find_cache(cache, v[i]) == -1)
        {
            notHit++;
            if(cache.size() < max_cache_size)
            {
                cache.push_back(v[i]);
            }
            else
            {
                int k = 0;
                for (; k < max_cache_size - 1; ++k)
                {
                    cache[k] = cache[k+1];
                }
                cache[k] = v[i];
            }
        }
        for (int i = 0; i < cache.size(); ++i)
        {
            cout << cache[i] << " ";
        }
        cout << endl;
    }

    return notHit;
}



char duplicateChar(string A, int n)
{
    char* tmp = new char[256]();
    for (int i = 0; i < n; ++i)
    {
        if(tmp[A[i]] == 0)
            ++tmp[A[i]];
        else
        {
            delete [] tmp;
            return A[i];
        }
    }


}

bool comp(std::pair<string, int> left, std::pair<string, int> right)
{
    if(left.second > right.second)
        return true;
    else
        return false;
}

vector<string> findCoder(vector<string> A, int n) 
{
    std::vector<std::pair<string, int>> v;
    for (int i = 0; i < n; ++i)
    {
        string tmp = A[i];
        for (int j = 0; j < tmp.size(); ++j)
        {
            tmp[j] = tolower(tmp[j]);
        }
        int pos = 0;
        int count = 0;
        while(pos < tmp.size())
        {
            pos = tmp.find("coder", pos);
            if( pos != string::npos)
            {
                count++;
                pos++;
            }
            else
                break;
        }
        if(count != 0)
            v.push_back({A[i], count});      
    }

    std::stable_sort(v.begin(), v.end(), comp);
    std::vector<string> ret;
    for (int i = 0; i < v.size(); ++i)
    {
        ret.push_back(v[i].first);
    }
    return ret;
}


int getDis(vector<int> A, int n) 
{
    int maxDis = 0;
    for (int i = 0; i < n; ++i)
    {
        for (int j = i+1; j < n; ++j)
        {
            int tmp = A[j] - A[i];
            if(tmp > maxDis)
                maxDis = tmp;
        }
    }
    
    return maxDis;
}

vector<vector<int> > flipChess(vector<vector<int> > A, vector<vector<int> > f) 
{
    std::vector<vector<int>> ret;
    int row = A.size();
    if(row <= 0)
        return ret;
    int col = A[0].size();
    int len_f = f.size();
    if(len_f == 0 || col == 0 || f[0].empty())
        return ret;

    ret = A;

    for (int i = 0; i < f.size(); ++i)
    {
        int x = f[i][0] - 1;
        int y = f[i][1] - 1;
        cout << x << " " << y << endl;
        if(x-1 >= 0 && x-1 < row && y >= 0 && y < col)
            ret[x-1][y] ^= 1;
        if(x+1 >= 0 && x+1 < row && y >= 0 && y < col)
            ret[x+1][y] ^= 1; 
        if(x >= 0 && x < row && y-1 >= 0 && y-1 < col)
            ret[x][y-1] ^= 1; 
        if(x >= 0 && x < row && y+1 >= 0 && y+1 < col)
            ret[x][y+1] ^= 1;
    }

    return ret;
}


int countArea(vector<int> A, int n) 
{
    std::vector<int> area(n, 0);
    for (int i = 0; i < n; ++i)
    {
        int first = i - 1;
        int last = i + 1;
        for (; first >= 0; --first)
        {
            if(A[first] < A[i])
                break;
        }
        ++first;
        for (; last < n; ++last)
        {
            if(A[last] < A[i])
                break;
        }
        --last;

        area[i] = (last - first + 1) * A[i];
    }

    int maxArea = 0;
    for (int i = 0; i < n; ++i)
    {
        if(area[i] > maxArea)
            maxArea = area[i];
    }
    return maxArea;
}

int strMinus(string s1, string s2)
{
    assert(s1 >= s2);
    int len = s1.size();
    if(len == 0)
        return -1;
    int ret = 0;
    bool isoverflow = false;
    int n = 1;
    for (int i = len - 1; i >= 0; --i)
    {
        int tmp = s1[i] - s2[i];
        if(isoverflow)
        {
            --tmp;
            isoverflow = false;
        }
        if(tmp < 0)
        {
            isoverflow = true;
            ret += (26 + tmp) * n;
        }
        else
            ret += tmp * n;

        n *= 26;
    }

    return ret;
}

/**
 * 将幂指数表示成二进制，假设我们想计算2^23，在这里x = 2，n = 23，
 * 这个算法首先把23表示成二进制的形式10111。
 * 扫描这个二进制数(10111)每当遇到0或1，
 * 则相应的求x的平方或者乘以x
 */
template<class T>
T power1(T x, T n) 
{
    T y = 1;

    while (true) 
    {
        T t = n % 2;
        n = floor(n/2);

        if (t == 1) 
            y = y * x;

        if (n == 0)
            break;

        x = x * x;
    }

    return y;
}

int main(int argc, char const *argv[])
{
    cout << power1(2,23) << endl;
    return 0;
}