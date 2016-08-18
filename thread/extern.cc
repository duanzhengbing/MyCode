#include "extern.h"
#include <cctype>
void printUpper(const std::string& str)
{
	std::string tmp;
	for(auto i : str)
		tmp.push_back(toupper(i));
	std::cout << tmp << std::endl;
}
//float num = 10.02;
//const int SIZE = 100;
//int main(int argc, char const *argv[])
//{
//	std::string str = "duanzhegnb";
//	printUpper(str);
//	return 0;
//}
