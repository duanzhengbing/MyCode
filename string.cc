#include <string>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <climits>
#include <vector>
#include <sstream>
using namespace std;


// 复制构造函数参数为const左值引用，移动构造函数参数为右值引用。触发移动构造函数的调用是临时变量或者将亡值的复制过程。
/**
 * 当类中含有指针成员时,应该手动定义复制构造函数和赋值函数
 * 编译选项 g++ -std=c++11 move.cc -fno-elide-constructors
 * 指明编译器不要对构造函数进行优化
 */
#include <iostream>
#include <string>
class A
{
public:
    A(int n):m(new int(n))
    {
        std::cout << "default construct" << ++n_cstr << std::endl;
    }
    /**
     * 如果默认编译器自动生成的复制构造函数的话,运行会有double free错误
     * 因为默认的是指针的拷贝过程,新对象和旧对象都指向了同一块内存,销毁两个对象时
     * 相当于销毁了内存两次.
     */
    A(const A& a) : m(new int(*(a.m)))
    {
        std::cout << "copy construct" << ++n_cptr << std::endl;
    }
    /**
     * 移动构造函数，该函数不能抛出异常，因为移动构造还没完成有可能造成悬挂指针
     */
    A(A&& a) noexcept : m(a.m)
    {
        std::cout << "move construct" << std::endl;
        a.m = nullptr;
    }
    /**
     * 赋值函数
     */
    const A& operator=(const A& a)
    {
        std::cout << "对象赋值..." << std::endl;
        *m = *(a.m);
        return *this;
    }
    ~A()
    {
        std::cout << "destruct" << ++n_dstr << std::endl;
        delete m;
    }
public:
    int* m;
    static int n_cstr;
    static int n_dstr;
    static int n_cptr;
};
int A::n_cstr = 0;
int A::n_dstr = 0;
int A::n_cptr = 0;
/**
 * a是local variable，即a是将亡值，在函数调用结束之后将销毁
 * 将亡值的复制过程将会调用移动构造函数，由定义可知，将亡值的指针
 * 在复制之后设为了nullptr，调用析构函数不会释放指针指向的内存
 *
 * 将亡值是需要被移动的对象：返回右值引用的函数返回值，std::move的返回值
 */
A getA()
{
    A a(10);
    std::cout << std::hex << a.m << std::endl;
    return a;
}
struct st
{
    int a:10;
    int b:5;
    st():a(16),b(20) {}
};

double sqrt3()
{
    double x = 3.1415926*2/double(3);
    double x2 = x*x;
    double sum = x;
    double tmp = x;
    double tmp2 = 1;
    for (int i = -1; i < 4; ++i)
    {
        tmp *= x2;
        tmp2 = tmp2*(2*i+4)*(2*i+5);
        if((i+1) % 2 == 0)
            sum -= tmp/tmp2; 
        else
            sum += tmp/tmp2;

    }
    return sum*2;
}

int main()
{
    cout << sqrt3() << endl;
    return 0;
}

