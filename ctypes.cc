#include <iostream>
#include <cctype>
#include <iomanip>
using namespace std;

int main(int argc, char const *argv[])
{
	char num = '1';
	char ch  = 'a';
	/* 参数如果是字母则返回2，是数字则返回4，否则返回0 */
	cout << std::boolalpha << isalnum(num) << endl; // 4
	cout << std::boolalpha << isalnum(ch)  << endl; // 2
	cout << std::boolalpha << isalnum('c') << endl; // 2
	/* 参数如果是字母则返回2，否则返回0 */
	cout << std::boolalpha << isalpha(ch)  << endl;
	/* 参数如果是数字则返回2，否则返回0 */
	cout << std::boolalpha << isdigit(ch)  << endl;
	/* 参数如果是除空格之外的可打印字符则返回2 */
	cout << std::boolalpha << isgraph(ch)  << endl;

	/* 参数如果是标点符号则返回2 */
	cout << std::boolalpha << ispunct(ch)  << endl;


	/* 参数如果是大写字母则返回2 */
	cout << std::boolalpha << isupper(ch)  << endl;
	/* 参数如果是小写字母则返回2 */
	cout << std::boolalpha << islower(ch)  << endl;
	/* 参数如果是大写字母则返回其小写 */
	cout << tolower(ch)  << endl;
	/* 参数如果是小写字母则返回其大写 */
	cout << toupper(ch)  << endl;
	return 0;
}