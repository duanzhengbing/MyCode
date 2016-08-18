#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <functional>
using namespace std;

static void print(const std::vector<int>& v)
{
    for (unsigned i = 0; i < v.size(); ++i)
    {
        cout << v[i] << " ";
    }
    cout << endl;
}

/**
 * 直接插入排序，时间复杂度O(N*N)
 */

static void insertSort(std::vector<int>& v)
{
    int length = v.size();
    for (int i = 1; i < length; ++i)
    {
        if(v[i] < v[i-1])
        {
            int tmp = v[i]; //设置一个哨兵保存待排序的数
            int j = i-1;
            // 后移比哨兵大的数，找到哨兵应该存放的位置j++
            while(j >= 0 && v[j] > tmp)
            {
                v[j+1] = v[j];
                j--;
            }
            v[j+1] = tmp;
        }
    }
}

/**
 * shell插入排序,当步长d为1时变为直接插入排序
 */
static void shellInsertSort(std::vector<int>& v, int d)
{
    int length = v.size();
    for (int i = d; i < length; ++i)
    {
        if(v[i] < v[i-d])
        {
            int tmp = v[i]; //设置一个哨兵保存待排序的数
            int j = i-d;
            // 后移比哨兵大的数，找到哨兵应该存放的位置j++
            while(j >= 0 && v[j] > tmp)
            {
                v[j+d] = v[j];
                j -= d;
            }
            v[j+d] = tmp;
        }
    }
}
/**
 * shell排序，先按增量n/2排序，直至增量为1
 * @param v [description]
 */
static void shellSort(std::vector<int>& v)
{
    for (int i = v.size()/2; i > 0 ; i = i/2)
    {
        shellInsertSort(v,i);
    }
}

/**
 * 简单选择排序
 */
static void selectSort(std::vector<int>& v)
{
    for (unsigned i = 0; i < v.size()-1; ++i)
    {
        /* 查找剩余元素的最小值与前一个元素交换 */
        unsigned minIdx = i;
        for (unsigned j = i+1; j < v.size(); ++j)
        {
            if(v[j] < v[minIdx])
                minIdx = j;
        }
        if(minIdx != i)
            std::swap(v[minIdx],v[i]);
    }
}
/**
* 二元选择排序
*/
static void selectSortII(std::vector<int>& v)
{
    unsigned i = 0, j = v.size() - 1;
    while (i < j)
    {
        /* 查找剩余元素的最小值和最大值 */
        unsigned min = i;
        unsigned max = i;
        for (unsigned k = i; k <= j; ++k)
        {
            if (v[k] < v[min])
                min = k;
            if (v[k] > v[max])
                max = k;
        }
        if (min != i)
            std::swap(v[min], v[i]);
        // 可能上一步最小值交换导致了最大值位置转移了
        if (v[max] < v[min]) 
            std::swap(v[min], v[j]);
        else
            std::swap(v[max], v[j]);

        i++;
        j--;
    }
}

/**
 * 堆排序涉及堆的下滤调整操作，对第i个节点下滤
 */
static void heapAdjust(std::vector<int>& v,int length, int pos,std::function<bool(int,int)> cmp = std::less<int>())
{
    int tmp = v[pos]; // 保存需要调整的值
    int hole = pos; // 初始化洞为当前待调整的位置
    while(2*hole + 1 < length) // 洞节点存在左孩子
    {
        // 找到最大的孩子节点
        int child = 2*hole + 1;
        if(child + 1 < length && cmp(v[child+1], v[child]))
            ++child;
        // child指向最大孩子的位置，最大孩子补洞同时更新洞
        if(cmp(v[child], tmp))
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
 * 弗洛伊德建堆算法，自下而上的下滤操作，时间复杂度为O(n)
 * @param v [description]
 */
static void buildHeap(std::vector<int>& v)
{
    int length =  v.size();
    // 最后一个有孩子节点的内部节点(length-1)/2
    for (int i = (length-1)/2; i >= 0; --i)
    {
        heapAdjust(v,length,i);
    }
}


static void heapSort(std::vector<int>& v)
{
    int length = v.size();
    buildHeap(v);
    for (int len = length-1; len >= 1 ; --len)
    {
        std::swap(v[len],v[0]);
        heapAdjust(v,len,0); // 每次都是对堆顶元素调整，堆的规模减1
    }
}


static void bubbleSort(std::vector<int>& v)
{
    int length = v.size();
    int low = 0;
    int high = length-1;
    while(low < high)
    {
        for (int j = low; j < high; ++j)
        {
            if(v[j] > v[j+1])
            {
                std::swap(v[j],v[j+1]);
            }
        }
        --high;
        for (int j = high; j > low; --j)
        {
            if(v[j] < v[j-1])
            {
                std::swap(v[j],v[j-1]);
            }
        }
        ++low;
    }
}

inline static median(std::vector<int>& v,int low,int high)
{
    int mid = (low + high) / 2;
    if(v[low] < v[mid])
        std::swap(v[low], v[mid]);
    if(v[mid] < v[high])
        std::swap(v[mid], v[high]);
    if(v[low] < v[high])
        std::swap(v[low], v[high]);

    std::swap(v[low], v[high]);
}

static int partition(std::vector<int>& v, int low, int high)
{
    median(v,low,high);
    std::swap(v[low],v[high]);
    int tmp = v[low];
    int i = low;
    int j = high;
    while(i < j)
    {
        while(i < j && v[j] >= tmp)
            j--;
        if(i < j)
            v[i] = v[j];
        while (i < j && v[i] < tmp)
            i++;
        if(i < j)
            v[j] = v[i];
    }
    v[i] = tmp;
    return i;
}

static int partition2(std::vector<int>& v, int low, int high)
{
    median(v,low,high);
    int pivot = v[high];
    int first = low;
    int last = high - 1;
    while(first < last)
    {
        while(v[first] < pivot && first < last) 
            ++first;

        while(v[last] >= pivot && first < last) 
            --last;

        if(first < last)
            std::swap(v[first++], v[last--]);
    }
    std::swap(v[first], v[high]);
    return first;
}
static int partition3(std::vector<int>& v, int low, int high)
{
    median(v,low,high);
    int pivot = v[high];
    int first = low;
    int last = high - 1;
    while(true)
    {
        while(v[first] < pivot) 
            ++first;

        while(v[last] >= pivot) 
            --last;

        if(first < last)
            std::swap(v[first], v[last]);
        else
            break;
    }
    std::swap(v[first], v[high]);
    return first;
}


static void qSort(std::vector<int>& v, int low, int high, int k)
{
    static int i = 0;
    cout << "第" << i << "趟 ";
    i++;
    print(v);
    if(high-low > k)
    {
        int mid = partition3(v, low, high);
        qSort(v, low, mid-1, k);
        qSort(v, mid+1, high, k);
    }
    else
    {
        insertSort(v);
    }
}

static void quickSort(std::vector<int>& v)
{
    int length = v.size();
    int k = 2;
    qSort(v, 0, length-1,k);   
}

static void sortStr(string& str)
{
    int len = str.size();
    int help[26] = {0};
    string help_str = str;
    /* 统计每个字母出现的次数 */
    for (int i = 0; i < len; ++i)
    {
        help[str[i] - 'a']++;
    }
    /* 计算每个字母存放的位置 */
    for (int i = 1; i < 26; ++i)
    {
        help[i] += help[i - 1];
    }

    int index = 0;
    for (int i = 0; i < len; ++i)
    {
        index = str[i] - 'a';
        help_str[help[index] - 1] = str[i];
        help[index]--;
    }
    str = help_str;
}

static void sortByAge(std::vector<int>& v)
{
    int len = v.size();
    int help[100] = {0};
    std::vector<int> tmp(len,0);
    for (int i = 0; i < len; ++i)
    {
        help[v[i]]++;
    }
    for (int i = 1; i < 100; ++i)
    {
        help[i] += help[i-1];
    }

    int sortedIdx = 0;
    for (int i = 0; i < len; ++i)
    {
        sortedIdx = help[v[i]];
        tmp[sortedIdx - 1] = v[i];
        help[v[i]]--;
    }
    print(tmp);
    v = tmp;
}


int main(int argc, char const *argv[])
{
    std::vector<int> v {49,38,65,97,76,13,27,49};
    // std::vector<int> v {49,38};
    // cout << partition2(v,0,v.size()-1) << endl;
    // print(v);
    // std::vector<int> v {9,8,6,7,7,3,2,4};
    // insertSort(v);
    // shellInsertSort(v,2);
    // shellSort(v);
    // selectSortII(v);
    // heapAdjust(v,1);
    // buildHeap(v);
    // heapSort(v);
    
    // bubbleSort(v);
    quickSort(v);
    print(v);
    // 
    // string str = "abcdacfdghjikcnbasdfhjcksassadf";
    // sortStr(str);
    // sortByAge(v);
    // cout <<  << endl;
    // print(v);

    return 0;
}
