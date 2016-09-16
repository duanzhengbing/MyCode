
/**
 * author: duanzhengbing
 * date: 2016/8/22
 * copyright@duan
 */
#include <iostream>
#include <cstring>
#include <string>
#include <vector>

using namespace std;

/**
 * 在字符串中查找ch，并且位置不是pos，返回正确的位置
 */
int findCharInHeadSeq(char ch, char* arr, int length, int pos)
{
    if(ch == 0)
        return -1;
    for (int i = 0; i < length; ++i)
    {
        if(arr[i] == ch && i != pos)
            return i;
    }
    return -1;
}

int wordsArangeList(const vector<string>& arr)
{
    int len = arr.size();
    if(len == 1)
        return 1;

    if(len == 2)
    {
        if(arr[0][0] == arr[1][arr[1].size()-1]
            || arr[0][arr[0].size()-1] == arr[1][0])
            return 1;
        else
            return -1;
    }
    else
    {
        char head[200] = {0}; //字符串头部字符组成的数组
        char tail[200] = {0}; //字符串尾部字符组成的数组

        for (int i = 0; i < len; ++i)
        {
            head[i] = arr[i][0];
            tail[i] = arr[i][arr[i].size()-1];
        }

        int count = 0;	//作为尾部的字符串个数
        int access = 0; //已经遍历的尾部字符个数
        int i  = 0;
        int visit[200] = {0}; //尾部字符串是否已经访问过
        while(access <= len && i < len)
        {
        	visit[i] = 1;
            int idx = findCharInHeadSeq(tail[i], head, len, i);
            if(idx < 0) //在头部序列中没有找到当前尾部字符
            {
                count++;
                if(count > 1)
                    return -1;
                int j = 0;
                for(; j < len; j++)
                {
                    if(visit[j] == 0)
                    {
                        i = j;
                        break;
                    }
                }
                if(j == len)
                	return 1;
            }
            else
            {
                head[idx] = 0; //清除之前找到的头部
                if(visit[idx] == 0) //下一个访问的尾部字符没有被访问
                {
                    i = idx;
                    access++;
                }
                //否则，在头部字符串中继续查找其他尾部字符
            }
        }
        return 1;
    }
}

int main()
{
	int num = 0;
    vector<string> v;
    vector<int> ret;
    while(cin >> num)
    {
        string tmp;
        v.clear();
        for(int i = 0; i < num; ++i)
        {
            cin >> temp;
            v.push_back(tmp);
        }

        ret.push_back(wordsArangeList(v));
    }

    for(auto i : v)
    {
    	if( i == 1)
    		cout << "Yes" << endl;
    	else
    		cout << "No" << endl;

    }


    return 0;
}

