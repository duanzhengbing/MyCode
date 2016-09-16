#include <iostream>
#include <cstring>
 
using namespace std;
 
int main()
{
	[]{};		//最简lambda函数
	int a = 3;
	int b = 5;
	[=] {return a + b;};  //省略了参数列表和返回类型,返回类型由编译器自动推断为int
	auto fun1 = [&](int c){b = a + c;};  //省略的返回类型,无返回值
	auto fun2 = [=,&a](int c)->int { return a = b + c;};  //各部分都很完整的lanbda函数
        auto fun3 = [=](){a = 1;};  //编译错误!!按值传递方式捕捉的变量是lambda函数中不可更改的常量.
	fun1(20);
	std::cout << "a =   " << a << std::endl;  //3
	std::cout << "b =   " << b << std::endl;  //23
	int y = fun2(10);
	cout << "y =   " << y << endl;  //15,not 33因为lambda函数运算是基于初始状态的
	std::cout << "a =   " << a << std::endl;  //15
	std::cout << "b =   " << b << std::endl;  //23
	return 0;
}