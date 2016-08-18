#include <iostream>
#include <algorithm>
#include <string>
using namespace std;

class Test
{
public:
    Test() 
    { 
        cout << "default constructor" << endl;
        // throw 1;
    }
    ~Test(){ cout << "destructor" << endl;}
public: 
    /* 第一个参数必须是size_t(unsigned int) */
    // void* operator new(size_t size, const std::nothrow_t& nothrow_value)  
    // {  
    //     std::cout << "call Test::operator new nothrow" << std::endl;  
    //     return ::operator new(size);  
    // }   

    // void* operator new(size_t size)  
    // {  
    //     std::cout << "call Test::operator new" << std::endl;  
    //     return ::operator new(size);  
    // } 

    void* operator new(unsigned size, const std::string& info)
    {
        std::cout << info << std::endl;
        return ::operator new(size);
    }
    //参数必须是void* 返回值为void
    void operator delete(void* pointer, const std::string& info)
    {
        std::cout << "call operator delete" << std::endl;
        ::operator delete(pointer);
    }

    void printValue()
    {
        std::cout << d_value << std::endl;
    }

private:
    int d_value {10};
};

int main()
{
	// new一个Test类对象出错，operator new函数为private
    // Test* pt = new Test();
    // pt->printValue();
    /* std::nothrow是std::nothrow_t类型的常量，只用用于重载operator new时指明不抛出异常 */
    // Test* pt2 = new(std::nothrow) Test();
    // pt2->printValue();

    Test* pt3 = new("duanzhengbing") Test();
    pt3->printValue();


    // delete pt;
    // delete pt2;
    delete("duanzhengbing") pt3;

    return 0;
}
// class X
// {
// public:
//     X() { cout<<"constructor of X"<<endl; }
//     ~X() { cout<<"destructor of X"<<endl;}

//     void SetNum(int n)
//     {
//         num = n;
//     }

//     int GetNum()
//     {
//         return num;
//     }

// private:
//     int num;
// };

// int main()
// {
//     char* buf = new char[sizeof(X)];
//     /* 在已经分配的内存buf上构造对象 */
//     X *px = new(buf) X;
//     px->SetNum(10);
//     cout << px->GetNum() << endl;

//     px->~X(); //手动调用析构函数
//     delete []buf;

//     return 0;
// }