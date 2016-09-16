#include <iostream>
#include <string>
#include <vector>
using namespace std;


int next_pos[] = {0,0,0,0,0,1,2};

// vector<int> creatNext(string t)
// {
// 	int len = t.size();
// 	std::vector<int> v(len, 0);
// 	int i = 0;
// 	int j = 1;
// 	while(j < len)
// 	{

// 	}
// }

std::vector<int> buildMatchTable(string str)
{
    std::vector<int> table;
    int length = str.size();
    table.resize(length,0);

    int i = 1;
    int j = 0;
    while(i < length)
    {
	    /* 如果后面的某个字符与第一个字符匹配成功，则i，j携手共进，继续匹配
		 * 并更新i的匹配值为前一匹配值加1
		 */
        if(str[i] == str[j])
        {
            table[i] = table[i-1] + 1;
            ++i;
            ++j;
        }
        else /* 否则当前字符必须从字符串的起始位置重新匹配，此时匹配值不做更新，即为初始值零 */
        {
            j = 0;
            ++i;
        }
    }

    return table;
}
bool KMP(string s, string t)
{
	int len_s = s.size();
	int len_t = t.size();

	std::vector<int> v = buildMatchTable(t);

	int i = 0;
	int j = 0;
	while(i < len_s && j < len_t)
	{
		if(s[i] == t[j])
		{
			++i;
			++j;
		}
		else
		{
			if(j == 0)
				++i;
			else
				// j = next_pos[j-1];
				j = v[j];
		}
	}
 
	if(j == len_t)
		return true;
	else
		return false;
}

int main(int argc, char const *argv[])
{
	string t = "ABCDABD";
	string s = "BABFAABCDABCDABDE";

	std::vector<int> v = buildMatchTable(t);

	for(auto i : v)
		cout << i << " ";
	cout << endl;

	cout << KMP(s, t) << endl;
	return 0;
}