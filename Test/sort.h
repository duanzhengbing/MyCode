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
 * ֱ�Ӳ�������ʱ�临�Ӷ�O(N*N)
 */

static void insertSort(std::vector<int>& v)
{
    int length = v.size();
    for (int i = 1; i < length; ++i)
    {
        if(v[i] < v[i-1])
        {
            int tmp = v[i]; //����һ���ڱ�������������
            int j = i-1;
            // ���Ʊ��ڱ���������ҵ��ڱ�Ӧ�ô�ŵ�λ��j++
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
 * shell��������,������dΪ1ʱ��Ϊֱ�Ӳ�������
 */
static void shellInsertSort(std::vector<int>& v, int d)
{
    int length = v.size();
    for (int i = d; i < length; ++i)
    {
        if(v[i] < v[i-d])
        {
            int tmp = v[i]; //����һ���ڱ�������������
            int j = i-d;
            // ���Ʊ��ڱ���������ҵ��ڱ�Ӧ�ô�ŵ�λ��j++
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
 * shell�����Ȱ�����n/2����ֱ������Ϊ1
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
 * ��ѡ������
 */
static void selectSort(std::vector<int>& v)
{
    for (unsigned i = 0; i < v.size()-1; ++i)
    {
        /* ����ʣ��Ԫ�ص���Сֵ��ǰһ��Ԫ�ؽ��� */
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
* ��Ԫѡ������
*/
static void selectSortII(std::vector<int>& v)
{
    unsigned i = 0, j = v.size() - 1;
    while (i < j)
    {
        /* ����ʣ��Ԫ�ص���Сֵ�����ֵ */
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
        // ������һ����Сֵ�������������ֵλ��ת����
        if (v[max] < v[min])
            std::swap(v[min], v[j]);
        else
            std::swap(v[max], v[j]);

        i++;
        j--;
    }
}


/**
 * �������漰�ѵ����˵����������Ե�pos���ڵ�����
 */
static void heapAdjust(std::vector<int>& v,int length, int pos,
    std::function<bool(int,int)> cmp = std::less<int>())
{
    int tmp = v[pos]; // ������Ҫ������ֵ
    int hole = pos; // ��ʼ����Ϊ��ǰ��������λ��
    while(2*hole + 1 < length) // ���ڵ��������
    {
        // �ҵ����ĺ��ӽڵ�
        int child = 2*hole + 1;
        if(child + 1 < length && cmp(v[child+1], v[child]))
            ++child;
        // childָ������ӵ�λ�ã�����Ӳ���ͬʱ���¶�
        if(cmp(v[child], tmp))
        {
            v[hole] = v[child];
            hole = child;
        }
        else  // �ҵ�������ֵ����ȷλ��
            break;
    }
    v[hole] =  tmp;
}
/**
 * �������½����㷨�����¶��ϵ����˲�����ʱ�临�Ӷ�ΪO(nlogn)
 * @param v [description]
 */
static void buildHeap(std::vector<int>& v,
    std::function<bool(int,int)> cmp = std::less<int>())
{
    int length =  v.size();
    // ���һ���к��ӽڵ���ڲ��ڵ�(length-1)/2
    for (int i = (length-1)/2; i >= 0; --i)
    {
        heapAdjust(v,length,i,cmp);
    }
}


static void heapSort(std::vector<int>& v,
    std::function<bool(int,int)> cmp = std::less<int>())
{
    int length = v.size();
    buildHeap(v,cmp);
    for (int len = length-1; len >= 1 ; --len)
    {
        std::swap(v[len],v[0]);
        heapAdjust(v,len,0,cmp); // ÿ�ζ��ǶԶѶ�Ԫ�ص������ѵĹ�ģ��1
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

inline static int getAverage(const std::vector<int>& v,int low,int mid,int high)
{
    if(v[low] >= v[mid])
    {
        if(v[mid] >= v[high])
            return mid;
        else if(v[high] >= v[low])
            return low;
        else
            return high;
    }
    else
    {
        if(v[mid] < v[high])
            return mid;
        else if(v[high] < v[low])
            return low;
        else
            return high;
    }
}

static int partition(std::vector<int>& v, int low, int high)
{
    int average = getAverage(v,low,(low+high)/2,high);
    std::swap(v[low],v[average]);
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
/**
 * һ���������
 * U 49  38  65  97  76  13  27 49
 * ^ ^                           ^
 * i j                          key
 * U 49  38  65  97  76  13  27 49
 *   ^                           ^
 *  ij                          key
 * U 49  38  65  97  76  13  27 49
 *   ^   ^                       ^
 *   i   j                      key
 * U 49  38  65  97  76  13  27 49
 *       ^                       ^
 *       ij                      key
 * U 49  38  65  97  76  13  27 49
 *       ^   ^                   ^
 *       i   j                  key
 * U 49  38  65  97  76  13  27 49
 *       ^               ^       ^
 *       i               j      key
 * U 49  38  13  97  76  65  27 49
 *           ^           ^      ^
 *           i           j      key
 * U 49  38  13  27  76  65  97 49
 *               ^           ^  ^
 *               i           j  key
 * U 49  38  13  27  49  65  97 76
 *               ^   ^       ^  ^
 *               i   i+1     j  key
 */

// int partition(std::vector<int>& v, int low, int high)
// {
//     int key = v[high];
//     int i = low -1;

//     for (int j = low; j < high; ++j)
//     {
//         if(v[j] <= key)
//         {
//             i++;
//             if(i != j)
//              std::swap(v[i], v[j]);
//         }
//     }
//     std::swap(v[i+1],v[high]);

//     return i+1;
// }

static void qSort(std::vector<int>& v, int low, int high, int k)
{
    print(v);
    if(high-low > k)
    {
        int mid = partition(v, low, high);
        qSort(v, low, mid-1, k);
        qSort(v, mid+1, high, k);
    }
}

static void quickSort(std::vector<int>& v)
{
    int length = v.size();
    int k = 0;
    qSort(v, 0, length-1,k);
    // ��������ʹ�����ݻ���������ʹ�ò�������
    // print(v);
    // insertSort(v);
}

static void sortStr(string& str)
{
    int len = str.size();
    int help[26] = {0};
    string help_str = str;
    /* ͳ��ÿ����ĸ���ֵĴ��� */
    for (int i = 0; i < len; ++i)
    {
        help[str[i] - 'a']++;
    }
    /* ����ÿ����ĸ��ŵ�λ�� */
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


