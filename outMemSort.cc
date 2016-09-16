#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <bitset>
#include <algorithm>
using namespace std;

constexpr int length = 103;
constexpr int mem_size  = 10; //内存容量

void writeTofile(const char* filename)
{
    FILE* f = fopen(filename, "w");
    srand((unsigned)time(NULL));
    for (int i = 0; i < length; ++i)
    {
        int tmp = (rand() * RAND_MAX + rand()) % length;
        cout << tmp << " ";  
        fprintf(f, "%d ", tmp); 
    }

    fclose(f);
    cout << "\n-------write end---------"<< endl;
}

void readFromFile(const char* filename)
{
    FILE* f = fopen(filename, "r");
    int tmp = 0;
    
    while(fscanf(f, "%d", &tmp) != EOF)
    {
        cout << tmp << " ";       
    }

    fclose(f);
    cout << "\n-------read end----------" << endl;
}

/**
 * 假设内存中每次最多只能排序5个元素，那么20个元素将分成
 * 4个有序临时文件
 */
/**
 * 从文件中读取数据
 * @param  f     [description]
 * @param  array [读入数据保存在数组array中]
 * @param  n     [description]
 * @return       [返回读取的数据个数]
 */
int read_data(FILE* f, int array[], int n)
{
    int i = 0;
    while(i < n && (fscanf(f, "%d", &array[i]) != EOF))
        i++;
    printf("read:%d integer\n", i);
    return i;
}

/**
 * 将数组中的前n个数写入文件f中
 * @param f     [description]
 * @param array [description]
 * @param n     [写入数据的个数]
 */
void write_data(FILE* f, int array[], int n)
{
    for(int i = 0; i < n; ++i)
        fprintf(f, "%d ", array[i]);
}
/* 生成临时文件名字，文件名和文件索引有关 */
string temp_filename(int index)
{
    char temp_name[20];
    sprintf(temp_name, "temp%d.txt", index);
    return temp_name;
}

int memory_sort(const char* in_file)
{
    FILE* fin = fopen(in_file, "r");
    int file_count = 0;
    int* array = new int[mem_size];
    int num = 0;

    while((num = read_data(fin, array, mem_size)) > 0)
    {
        std::sort(array, array + num);
        string temp_name = temp_filename(file_count++);
        FILE* temp_file = fopen(temp_name.c_str(), "w");

        write_data(temp_file, array, num);
        fclose(temp_file);
    }

    delete[] array;
    fclose(fin);

    return file_count;
}

void merge_sort(int file_count)
{
    if(file_count <= 0)
        return;

    std::vector<FILE*> farray(file_count,nullptr);
    for(int i = 0; i < file_count; ++i)
    {
        string fileName = temp_filename(i);
        farray[i] = fopen(fileName.c_str(), "rt");
    }

    std::vector<int> data(file_count, 0);
    //存储每个文件当前的一个数字
    std::vector<bool> finish(file_count, true);
    for(int i = 0; i < file_count; ++i)
    {
        //读每个文件的第一个数到data数组
        if(fscanf(farray[i], "%d", &data[i]) == EOF)
            finish[i] = false;
    }

    FILE* fout = fopen("sorted_file.txt", "wt");
    while(true)
    {
        //如果文件中没有数据可以取了则退出循环
        int idx = 0;
        /* 找到第一个还有数据的有序文件 */
        while(idx < file_count && !finish[idx])
            ++idx;
        if(idx >= file_count)
            break;

        /* 查找内存中最小的元素，并用idx记录对应文件索引 */
        int min = data[idx];
        for (int i = idx + 1; i < file_count; ++i)
        {
            if(finish[i] && data[i] < min)
            { 
                min = data[i];
                idx = i;
            }
        }
        /* 将最小元素输出到磁盘文件中，并读入下一个元素 */
        fprintf(fout, "%d ", min);
        if(fscanf(farray[idx], "%d", &data[idx]) == EOF)
            finish[idx] = false;

    }

    //关闭打开的临时文件和输出文件
    for (int i = 0; i < file_count; ++i)
        fclose(farray[i]);

    fclose(fout);
}

// template <typename T>
void CalcAllPermutation(string& str, int num)
{
    if (num < 1)
        return;
    while (true) {
        int i;
        for (i = num - 2; i >= 0; --i) {
            if (str[i] < str[i + 1])
                break;
        }
        if (i < 0)
        {
            cout << str << " ";
            break; // 已经找到所有排列
        }
        int k;
        for (k = num - 1; k > i; --k) {
            if (str[k] > str[i])
                break;
        }
        swap(str[i], str[k]);
        reverse(&str[0] + i + 1, &str[0] + num);
    }
}


void permute(char *a, int i, int n)
{
   int j;
   if (i == n)
     printf("%s\n", a);
   else
   {
        for (j = i; j <= n; j++)
       {
          if(a[i] == a[j] && j != i)  //为避免生成重复排列，当不同位置的字符相同时不再交换
            continue;
          swap(a[i], a[j]);
          permute(a, i+1, n);
          swap(a[i], a[j]); //backtrack
       }
   }
}

void permute(string& str, int start, int length)
{
    if(start >= length || start < 0 || length > str.size())
        return;

    if(start == length - 1)
    {
        std::cout << str << " ";
        return;
    }

    for (int i = start; i < length; ++i)
    {
        if(str[i] == str[start] && i != start)
            continue;
        swap(str[i], str[start]); // 字符交换
        permute(str, start + 1, length); // 对[start + 1, end)进行全排列
        swap(str[i], str[start]); // 字符换回
        // swap()
    }
}


void oddsort(std::vector<int>& v)
{
    int i = 0;
    int j = v.size() - 1;
    while(i < j)
    {
        while(v[i] % 2 == 1)
            ++i;
        while(v[j] % 2 == 0)
            --j;
        if(i < j)
            std::swap(v[i++], v[j--]);
    }
}

int max1(std::vector<int> v, int begin, int end)
{
    int max_1 = 0;
    for (int i = begin; i < end; ++i)
    {
        int tmp = v[i + 1] - v[i];
        if(tmp > 0)
        {
            max_1 += tmp;
        }
    }
}

int calmax(std::vector<int> v)
{
    int len = v.size();
    if(len < 2)
        return 0;

    int max_1 = 0;
    // if(v[1] - v[0] > 0)
    //     max_1 = v[1] - v[0];

    int allmax = max_1;

    for (int k = 1; k < len; ++k)
    {
        for (int i = 0; i < k; ++i)
        {
            if(v[k] - v[i] > max_1)
                max_1 = v[k] - v[i];
        }
        int max_2 = 0;
        for (int j = k + 1; j < len; ++j)
        {
            for (int m = j + 1; m < len; ++m)
            {
                if(v[m] - v[j] > max_2)
                    max_2 = v[m] - v[j];

            }
        }

        if(allmax < max_1 + max_2)
            allmax = max_1 + max_2;
    }

    return allmax;

}
#include <climits>
int findNumsAppearHalf(std::vector<int> v)
{
    int invalid  = INT_MIN;
    if(v.empty())
        return invalid;

    int candidate = v[0];
    int ntimes = 1;
    for (int i = 1; i < v.size(); ++i)
    {
        if(ntimes == 0)
        {
            candidate = v[i];
            ntimes = 1;
            continue;
        }

        if(v[i] == candidate)
            ++ntimes;
        else
            --ntimes;

    }
    
    ntimes = 0;
    for (int i = 0; i < v.size(); ++i)
    {
        if(v[i] == candidate)
            ntimes++;
    }

    if((ntimes << 1) > v.size())
        return candidate;
    else
        return invalid;
}

bool isFibonacciNum(int num)
{
    if(num < 0)
        return false;
    int fibona[11] = {0,1,2,3,5,8,13,21,34,55,89};
    for (int i = 0; i < 11; ++i)
    {
        if(fibona[i] == num)
            return true;
    }

    return false;
}

void fibonacciStr(string str)
{
    std::vector<string> ret;
    int len = str.size();
    for (int i = 0; i < len; ++i)
    {
        for (int j = 1; len - i >= j; ++j)
        {
            string tmp = str.substr(i, j);
            int diff_ch = 0;
            int a[26] = {0};
            // cout << "tmp : "<< tmp << endl;
            for (int k = 0; k < tmp.size(); ++k)
            {
                a[tmp[k] - 'a']++;
            }
            for (int k = 0; k < 26; ++k)
            {
                if(a[k] > 0)
                    ++diff_ch;
            }
            if(isFibonacciNum(diff_ch))
            {
                if(find(ret.begin(),ret.end(),tmp) == ret.end())
                    ret.push_back(tmp);
            }
        }
    }

    std::sort(ret.begin(), ret.end());
    for(string i : ret)
        cout << i << endl;
}

// std::pair<int,int> pos(int num)
// {
//     assert(num >= 0 && num < 10)
//     if(num == 0)
//         return std::pair<int, int>(4,2);
//     else if(num < 4)
//         return std::pair<int, int>(1, num);
//     else if(num < 7)
//         return std::pair<int, int>(2, num - 3);
//     else
//         return std::pair<int, int>(3, num - 6);
// }

// string produceNumLessThanK(string str)
// {
//     int len = K.size();
//     bool ischanged = false;
//     for (int i = 1; i < len; ++i)
//     {
//         //可达
//         if(pos(str[i] - '0').first > pos(str[i-1]- '0').first 
//             && pos(str[i] - '0').second > pos(str[i-1]- '0').second)
//         {

//         }
//         else
//         {
//             if(ischanged)
//             {
//                 for (int j = i ; j < len; ++j)
//                 {
//                     str[j] = str[i - 1];
//                 }
//                 return str;
//             }
//             else
//             {
//                 if(i - 2 < 0)
//                 {
//                     --str[i - 1];
//                 }
//                 for (char k = str[i-1] - 1; k >= '0' ; --k)
//                 {
//                      if(pos(k - '0').first > pos(str[i-1]- '0').first 
//                         && pos(k - '0').second > pos(str[i-1]- '0').second)
//                     {
//                         str[i] = k;
//                     }
//                 }
//             }
//         }
//     }
// }

char toNum(char a, char b, bool& flag)
{
    if(a == '1')
        return b;
    else if(a == '2')
    {
        if(b == '1')
            return '0';
        else if(b == '4' || b == '7')
        {
            flag = true;
            return b - 1;
        }
        else
            return b;
    }
    else if(a == '3')
    {
        if(b == '6' || b == '9')
            return b;
        else if(b == '1' || b == '0'|| b == '2')
            return 0;
        else
            return a;
    }
    else if(a == '6')
    {
        if(b == '9')
            return b;
        else if( b == '7' || b == '8')
        {
            flag = true;
            return a;
        }
        else
            return 0;
    }
    else if(a == '4' || a == '7')
    {
        if(b >= a)
            return b;
        else
        {
            flag = true;
            return '0';
        }
    }

    else if(a == '5')
    {
        if(b == '7')
        {
            flag = true;
            return '6';
        }
        else if(b < '5')
        {
            flag = true;
            return '0';
        }
        else
            return b;
    }
    else if(a == '8')
    {
        if(b >= a)
            return b;
        else
        {
            flag = true;
            return '0';
        }
    }
    // else if(a == '9')
    // {
    //     if(b == '9')
    //         return '9'
    //     else

    // }
    else
    {
        flag = true;
        return 0;
    }
}

string foo(string str)
{
    int len = str.size();
    bool flag = false;
    for (int i = 0; i < len - 1; ++i)
    {
        char tmp = toNum(str[i], str[i+1], flag);
        if(tmp > 0)
        {
            if(flag)
            {
                str[i+1] = tmp;
                if(tmp == '0')
                {
                    for (int j = i + 1; j < len; ++j)
                    {
                        str[j] = '0';
                    }
                    return str;
                    
                }
                else
                {
                    for (int j = i + 1; j < len; ++j)
                    {
                        str[j] = '9';
                    }
                    return str;
                }
            }
        }
        else
        {
            if(i - 1 < 0)
                str[i]--;
            else
            {
                for (int n = i-1; n >= 0; --n)
                {
                    bool f = false;
                    char temp2 =  toNum(str[n], str[n+1]-1, f);
                    if(temp2)
                    {
                        if(f)
                        {
                            str[i] = temp2;
                            for (int k = i+1; k < len; ++k)
                            {
                                str[k] = '9';
                            }
                            return str;
                        }
                        else
                        {
                            str[i] = temp2;
                            for (int j = i + 1; j < len; ++j)
                            {
                                str[j] = '9';
                            }
                            return str;
                        }
                    }
                    
                }
            }
        }

    }

    return str;
}


int InversePairsCore(std::vector<int>& copy, std::vector<int>& data, int start, int end)
{
    if(start == end)
    {
        copy[start] = data[start];
        return 0;
    }
    int center = (end + start) / 2;

    int left = InversePairsCore(copy, data, start, center);
    int right = InversePairsCore(copy, data, center + 1, end);

    int i = center;
    int j = end;

    int indexCopy = end;
    int count = 0;
    while(i >= start && j >= center + 1)
    {
        if(data[i] > data[j])
        {
            copy[indexCopy--] = data[i--];
            count += j - center;
        }
        else
        {
            copy[indexCopy--] = data[j--];
        }
    }

    for (; i >= start; --i)
    {
        copy[indexCopy--] = data[i];
    }
    for (; j >= center + 1; --j)
    {
        copy[indexCopy--] = data[j];
    }
    for (int i = end; i >= start; --i)
    {
        data[i] = copy[i];
    }

    return left + right + count;
}

int InversePairs(vector<int> data) 
{
    constexpr int CONST_NUM = 1000000007;
    int len = data.size();
    long long count = 0;
    std::vector<int> copy = data;

    count = InversePairsCore(copy, data, 0, len - 1);


    return count % CONST_NUM;
}



int main(int argc, char const *argv[])
{
    // cout << INT_MAX << endl;
    std::vector<int> v{1,2,3,4,5,6,7,0};
    cout << InversePairs(v) << endl;
    return 0;
}