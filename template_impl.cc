#include "template.h"

template<typename T>
void print(const T& t)
{
	std::cout << t << std::endl;
}

void fun()
{
	print(10);
}

// int main(int argc, char const *argv[])
// {
	
// 	return 0;
// }