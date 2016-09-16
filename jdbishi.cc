#include <iostream>
#include <iterator>
#include <algorithm>
#include <deque>
using namespace std;

int main(int argc, char const *argv[])
{

	ostream_iterator<int> outIter(cout, ", "); //输出迭代器，以空格分隔
	deque<int> id{ 1, 2, 3, 4, 5, 6 };
	/* 打印id中的数据至标准输出 */
	copy(id.begin(), id.end(), outIter);
	cout << endl;

	/* 从标准输入读数据，然后再标准输出打印数据 */
	istream_iterator<int> inIter(cin);
	istream_iterator<int> end_of_stream;// win7下是ctrl D
	copy(inIter, end_of_strea m, outIter);

	system("pause");
	return 0;
}