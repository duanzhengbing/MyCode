#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <bitset>
using namespace std;

bool isContainCommonLetter(string str1, string str2)
{
    std::bitset<26> bit_map1;
    std::bitset<26> bit_map2;
    for (int i = 0; i < str1.size(); ++i)
    {
        bit_map1[str1[i] - 'a'] = 1;
    }
    for (int i = 0; i < str2.size(); ++i)
    {
        bit_map2[str2[i] - 'a'] = 1;
    }

    unsigned long a = bit_map1.to_ulong();
    unsigned long b = bit_map2.to_ulong();
    if((a & b) > 0)
        return true;
    else
        return false;
}

int maxProduct(vector<string>& words) 
{
    if(words.empty())
        return 0;
    int len = words.size();
    vector<int> mask(words.size());
    int result = 0;
    for (int i = 0; i < len; ++i)
    {
        for(auto ch : words[i])
        {
            mask[i] |= 1 << (ch - 'a');
        }
        for (int j = i + 1; j < len; ++j)
        {
            if(!(mask[i] & mask[j]))
            {
                int tmp = words[i].size() * words[j].size();
                if(tmp > result)
                    result = tmp;
            }
        }
    }

    return result;        
}

bool isSubsequence(string s, string t) 
{
    int len_s = s.size();        
    int len_t = t.size();
    if(len_s > len_t)
        return false;        

    auto start = t.begin();
    for (int i = 0; i < len_s; ++i)
    {
        auto pos = find(start, t.end(), s[i]);
        if(pos == t.end())
            return false;

        start = pos + 1;
    }

    return true;
}

int foo(std::vector<int> nums, int n, int target)
{
    if(n <= 0)
        return 0;
    if(nums[0] == target)
        return 1;
    if(target < nums[0])
        return 0;

    int ret = 0;
    int i = 0;
    while(target - i * nums[n-1] > 0)
    {
        ret += foo(nums, n - 1, target - i * nums[n-1]);
        ++i;
    }

    return ret;
}

int combinationSum4(vector<int>& nums, int target) 
{
    return foo(nums, nums.size(), target);
}

int main()
{
    string s = "abc";
    string t = "adcbc";
    cout << boolalpha << isSubsequence(s,t) << endl;
    std::vector<int> v {1,2,3};
    cout << combinationSum4(v, 4) << endl;
    return 0;    
}