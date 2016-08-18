#include <iostream>
#include <type_traits>

template<typename T>
void swap(T&& v1, T&& v2)
{
	typedef typename std::remove_reference<T>::type A;

	A tmp = v1;
	v1 = v2;
	v2 = tmp;
}


int main()
{
	int a = 1, b = 2;
	int& ra = a;
	swap(ra, b);
	std::cout << a << ' ' << b << std::endl;
	
	return 0;
}