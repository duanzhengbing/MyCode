#ifndef COMMON_H_
#define COMMON_H_

/**
 * 以下定义都具有内部链接属性
 */
const int LENGTH = 100;
enum { type1, type2 = 2, type3 };
static void function() {}
inline void foo() {}

struct node
{
	int val;
	node* next;
}
class myClass
{
public:
	myClass() {}
	~myClass() {}
}

/**
 * 以下声明具有外部链接属性
 */
extern int num;
extern const int SIZE;
void func(void);

//类的声明
class common
{
public:
	common();
	~common();
};

#endif