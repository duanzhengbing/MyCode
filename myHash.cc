#include "one_way_hash.h"
#include <iostream>
#include <cassert>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
using namespace std;

/**
 * 堆排序涉及堆的下滤调整操作，对第pos个节点下滤
 */
static void heapAdjust(std::vector<std::pair<string,int>>& v,int length, int pos)
{
    auto tmp = v[pos]; // 保存需要调整的值
    int hole = pos; // 初始化洞为当前待调整的位置
    while(2*hole + 1 < length) // 洞节点存在左孩子
    {
        // 找到最小的孩子节点
        int child = 2*hole + 1;
        if(child + 1 < length && v[child+1].second < v[child].second)
            ++child;
        // child指向最小孩子的位置，最小孩子补洞同时更新洞
        if(v[child].second < tmp.second)
        {
            v[hole] = v[child];
            hole = child;
        }
        else  // 找到待调整值的正确位置
            break;
    }
    v[hole] =  tmp;
}
/**
 * 弗洛伊德建堆算法，自下而上的下滤操作，时间复杂度为O(nlogn)
 * @param v [description]
 */
static void buildHeap(std::vector<std::pair<string,int>>& v)
{
    int length =  v.size();
    // 最后一个有孩子节点的内部节点(length-1)/2
    for (int i = (length-1)/2; i >= 0; --i)
    {
        heapAdjust(v,length,i);
    }
}

constexpr int HASHLEN {2807303};
constexpr int WORDLEN {30};

// 结点指针
typedef struct hash_node *hash_node_ptr;

struct hash_node
{
	string str;
	int count;
	hash_node *next;
	hash_node():
		str(""),count(0),next(nullptr) {}
	hash_node(string _str):
		str(_str),count(1),next(nullptr) {}
};
vector<hash_node_ptr> hash_table(HASHLEN, nullptr);


int hash_function(string str)
{
	unsigned long value = 0;
	value = HashString(const_cast<char *>(str.c_str()),0);
	if(value > HASHLEN)
		value = value % HASHLEN;
	return value;
}

/**
 * 将一个字符串加入哈希表，如果冲突则利用开链法
 * @param str [description]
 */
void append_word(string str)
{
	int index = hash_function(str);
	assert(index > 0);
	hash_node_ptr p = hash_table[index];

	while(p != nullptr)
	{
		if(p->str == str)
		{
			(p->count)++;
			return;
		}
		p = p->next;

	}

	hash_node_ptr q = new hash_node(str);
	q->next = hash_table[index];
	hash_table[index] = q;
}


std::vector<std::pair<string, int>>
calTopKInFile(const char* file,const int k)
{
    if(k < 1)
        return std::vector<std::pair<string, int>>();

    std::ifstream ifs("data.txt");
    std::string tmp;
    while(ifs >> tmp)
    {
        append_word(tmp);
    }
    ifs.close();

    /* 哈希表转换为vector并建立大顶堆计算TopK */
    std::vector<std::pair<string, int>> v;
 	for (int i = 0; i < HASHLEN; ++i)
 	{
 		hash_node_ptr p;
 		for (p = hash_table[i]; p != nullptr ; p = p->next)
 		{
 			v.push_back({p->str, p->count});
 		}
 	}
 	cout << v.size() << endl;
    std::vector<std::pair<string, int>> result(v.begin(), v.begin() + k);
    buildHeap(result);
    for (int i = k; i < v.size(); ++i)
    {
        if(v[i].second > result.begin()->second)
        {
            result[0] = v[i];
            heapAdjust(result, k, 0); 
        }
    }
    return result;
}

int main(int argc, char const *argv[])
{

	const char* file = "data.txt";
    constexpr int k = 10;
    std::vector<std::pair<string, int>> topK = calTopKInFile(file, k);
    for(auto i : topK)
    {
        cout << left; // 左对齐
        cout << setw(10) << i.first << setw(3) << i.second << endl;
    }
	return 0;
}