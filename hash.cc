#include <iostream>

#include <vector>
#include <fstream>
#include <unordered_map>
#include <set>
#include <map>
#include <algorithm>
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


static void heapSort(std::vector<std::pair<string,int>>& v)
{
    int length = v.size();
    buildHeap(v);
    for (int len = length-1; len >= 1 ; --len)
    {
        std::swap(v[len],v[0]);
        heapAdjust(v,len,0); // 每次都是对堆顶元素调整，堆的规模减1
    }
}

std::vector<std::pair<string, int>>
calTopKInFile(const char* file,const int k)
{
    if(k < 1)
        return std::vector<std::pair<string, int>>();
    /* 哈希表计算文件中每个字符串出现的次数 */
    std::unordered_map<string, int> hash_map;
    std::ifstream ifs("data.txt");
    std::string tmp;
    while(ifs >> tmp)
    {
        hash_map[tmp]++;
    }
    ifs.close();

    /* 哈希表转换为vector并建立大顶堆计算TopK */
    // 哈希表的迭代器一次只能增1
    std::vector<std::pair<string, int>> v(hash_map.begin(), hash_map.end());
    cout << "v.size() = " << v.size() << endl;
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
