/**
 * template learning
 * author : Cristiano-Duan
 * date : 2016/07/26
 */

#include <iostream>
#include <vector>
#include <stdexcept>
#include <set>
#include <map>
#include <deque>
#include <memory>
#include <bitset>
#include <string>
#include "extern.h"

using namespace std;



template<typename T,
        template<typename ELEM,
                 typename ALLOC = std::allocator<ELEM> >
                 class CONT = std::deque>
class Stack 
{
private:
    CONT<T> elems; 
public:
    void push(T const&);
    void pop(); 
    T top() const;
    bool empty() const
    {
        return elems.empty();
    }
};
template<typename T,
        template<typename,typename> class CONT>
void Stack<T,CONT>::push (T const& elem)
{
    elems.push_back(elem);
}

template<typename T,
        template<typename,typename> class CONT>
void Stack<T,CONT>::pop ()
{
    if (elems.empty()) 
    {
        throw std::out_of_range("Stack<>::pop: empty stack");
    }
    elems.pop_back();
}

template<typename T,
        template<typename,typename> class CONT>
T Stack<T,CONT>::top () const
{
    if (elems.empty()) 
    {
        throw std::out_of_range("Stack<>::top: empty stack");
    }
    return elems.back();
}

template<typename T1, typename T2>
class Myclass
{
public:
    Myclass(T1 t1, T2 t2):value1(t1),value2(t2) {}

public:
    T1 value1;
    T2 value2;
};

/**
 * 局部特化，两个模板参数具有相同的类型
 */
template<typename T>
class Myclass<T,T>
{
public:
    Myclass(T t1, T t2):value1(t1),value2(t2) {}

public:
    T value1;
    T value2;
};

/**
 * 局部特化，第二个模板参数为float
 */
template<typename T>
class Myclass<T,float>
{
public:
    Myclass(T t1, float t2):value1(t1),value2(t2) {}

public:
    T value1;
    float value2;
};


template<typename T>
void printcoll(const T& coll)
{
    typename T::const_iterator pos;
    typename T::const_iterator end(coll.end());

    for (pos = coll.begin(); pos != end; ++pos)
    {
        std::cout << *pos << " ";
    }
    std::cout << std::endl;
}



template<typename T>
class Base
{
public:
    Base() {}
public:
    int basefield {10};
};

template<typename T>
class DD : public Base<T>
{
public:
    DD() {}
    void foo() 
    { 
        // basefield = 100; error! 'basefield' was not declared in this scope
        // 模板类涉及继承时，必须显式的访问基类的成员，因为当基类特化时，可能不存在
        // 该基类成员，显式的访问基类成员可以编译通过，当基类特化时，确实不存在该成
        // 员，则编译器报错
        this->basefield = 100;//方式1
        Base<T>::basefield = 100; // 方式2，禁止了虚函数的调用
    }    
};

/* 基类的特化 */
template<>
class Base<bool>
{
public:
    Base() {}

public:
    enum { basefield = 42 }; //enum 一旦初始化不可修改
};

template<int N = 1000>
void foo()
{
    cout << N << endl;
}
template<float* p>
void foo()
{
    if(p != nullptr)
        cout << *p << endl;
    else
        cout << "nullptr" << endl;
}
// template<const char* p>
// void foo()
// {
//     cout << p << endl;
// }
float n = 10;
float num5 = n;

template<typename T1,typename T2,typename T3,typename T4>
class myclass;

template<typename T1,typename T2 = char,typename T3,typename T4>
void func(T1 t1,T2 t2,T3 t3,T4 t4);

extern const char* p = "duan";
int main(int argc, char const *argv[])
{
/*    Stack<int> intStack;
    intStack.push(10);
    intStack.push(100);
    intStack.push(1000);
    cout << intStack.top() << endl;
    Stack<float,std::vector> floatStack;
    floatStack.push(1.234);
    floatStack.push(3.1415);
    cout << floatStack.top() << endl;*/
    int num1 = 20;
    const int num2 = num1;
    const int num3 = 10; // num3 is a constant expresstion
    constexpr int num4 = 20;

    // foo<10>(); // fine, 10 is rvalue and is a constant expresstion
    // foo<num1>(); // error!, num1 is not a constant expresstion
    // foo<num2>(); // error!, num2 is a constant variable but not a constant expresstion
    // foo<num3>(); // fine, num3 is a constant expresstion
    // foo<num4>(); // fine, num4 is a constant expresstion
    // foo();

     //'& num5' is not a valid template argument of type 'float*' because 'num5' has no linkage
    // foo<&num5>(); //error!

    foo<nullptr>();
    foo<NULL>();
    return 0;
}
