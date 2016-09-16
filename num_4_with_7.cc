#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <math.h>
//#include <iomanip>
#include <map>
#include <set>
#include <algorithm>
using namespace std;


string&& doThings(long long K)
{
	// idx_l = ceil(log2(K+1) - 1)
	int idx_l = int(log(double(K + 1)) / log(2.0) - 1);
	int idx_r = idx_l + 1;
	string str_ret(idx_r, '4');
	K -= pow(2, idx_l + 1) - 1;
	for (int i = idx_r - 1; K != 0 && i >= 0; --i)
	{
		bool tail = K & 1;
		K >>= 1;
		str_ret[i] = (tail == 1 ? '7' : '4');
	}
	return std::move(str_ret);
}

vector<string>&& foo(vector<int>& vec_int)
{
	vector<string> vec_str(vec_int.size());
	for (int i = 0; i < vec_int.size(); ++i)
	{
		string str(doThings(vec_int[i]));
		vec_str[i] = str;
	}
		
	return std::move(vec_str);
}




int main()
{
	// int n, num;
	// cin >> n;
	// vector<int> vec_int(n);
	// for (int i = 0; i < n; ++i)
	// {
	// 	cin >> num;
	// 	vec_int[i] = num;
	// }
	// vector<string>&& vec_str = foo(vec_int);
	// for (int i = 0; i < vec_str.size(); ++i)
	// 	cout << vec_str[i] << endl;
	return 0;
}