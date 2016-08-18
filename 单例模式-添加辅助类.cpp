#include <iostream>
#include <string>
using namespace std;

/*关于单例对象销毁问题，我觉得以内部类的静态对象触发销毁动作的方式主要还是为了防范用户程序猿抽风，
  把单例当一般对象使用，在其上调用delete，然后又用getInstance，以为自己得到了全新的对象。这样做是危险的，因此，
  最好的办法是不给程序猿犯错的机会，将delete操作变得非法，也就是在添加内部类的同时，将单例的析构函数设置为private。*/
/*多线程可能造成创建多个实例的情况只在懒汉模式下才会有，因为该情况下的实例是在需要时才创建，饿汉模式下则在进入主函数之前就已经创建了实例，因此不存在
  多线程会创建多个实例的情况，而且饿汉模式往往效率较高，因为没有加锁解锁操作。*/
/*创建实例的方法有new和直接构造一个static的实例，其中静态初始化static实例的方式保证了线程的安全性，在性能要求比较高的时候可以采用这种方式，避免了频繁
  加锁解锁造成的资源浪费。*/
/*关于实例的销毁：在实际项目中，特别是客户端开发，其实是不在乎这个实例的销毁的。因为，全局就这么一个变量，全局都要用，它的生命周期伴随着软件的生命周期
  软件结束了，它就自然而然的结束了，因为一个程序关闭之后，它会释放它占用的内存资源的，所以，也就没有所谓的内存泄漏了。但是，有以下情况，是必须需要进行
  实例销毁的：
  1.在类中，有一些文件锁了，文件句柄，数据库连接 等等，这些随着程序的关闭而不会立即关闭的资源，必须要在程序关闭前，进行手动释放；
  2.具有强迫症的程序员。*/

/*class ASingleton						//Boost库中的实现方式，这种方式下可以创建一个static的实例，也可以采用new的方式创建实例。这两种情况下都不需要考
{										//虑多线程的问题以及手动销毁实例的问题，因为这里用了一个辅助类，并创建了该辅助类的一个对象，在创建该辅助类的对
public:									//象的构造函数当中调用了getInstance来创建实例，这一切都是在进入主函数之前完成的。至于释放实例的问题，对于实例
	static ASingleton * getInstance()	//是static的情况下不用考虑释放，出了主函数的作用域，系统会自动释放静态变量，对于new创建的实例，可以在辅助类的
	{									//析构函数里添加delete实例指针的操作来释放销毁实例。
		static ASingleton instance;
		return &instance;
	}
	void print()
	{
		cout << str << endl;
	}
protected:
	struct Create_Object
	{
		Create_Object()
		{
			cout << "Create_Object constructor" << endl;
			ASingleton::getInstance();
		}
		~Create_Object()
		{
			cout << "Create_Object deconstructor" << endl;
		}
	};
	static Create_Object _create_object;
	string str = "hello world";

	ASingleton(const ASingleton &) = delete;
	ASingleton & operator=(const ASingleton &) = delete;
	ASingleton() { cout << "ASingleton constructor" << endl; }
	~ASingleton() { cout << "ASingleton deconstructor" << endl; }
};
ASingleton::Create_Object ASingleton::_create_object;*/

/*class ASingleton			//懒汉模式，缺点是线程不安全，如果多个线程同时进入if(!m_data)判断，则会创建多个实例
{
public:
	static ASingleton * getInstance()
	{
		if(!m_data)
		{
			static ASingleton instance;
			//ASingleton instance;		//static是必须的，如果instance不是static的，则出了这个getInstance这个函数的作用域之后，该对象就会被析构
			m_data = &instance;
		}
		return m_data;
	}
	void print()
	{
		cout << str << endl;
	}

private:
	static ASingleton * m_data;
	string str = "hello world";
	ASingleton()
	{
		cout << "ASingleton constructor" << endl;
	}
	~ASingleton()
	{
		cout << "ASingleton deconstructor" << endl;
	}
	ASingleton(const ASingleton &) = delete;
	ASingleton & operator=(const ASingleton &) = delete;
};
ASingleton * ASingleton::m_data = nullptr;*/

/*class ASingleton							//饿汉模式，如果是多线程则需要用锁来保证互斥
{
public :
	static ASingleton * getInstance()
	{
		return &m_data;
	}
	void print()
	{
		cout << str << endl;
	}
private:
	string str = "hello world";
	static ASingleton m_data;
	ASingleton() {};
	ASingleton(const ASingleton &) = delete;
	ASingleton & operator=(const ASingleton &) = delete;
};
ASingleton ASingleton::m_data;*/

class ASingleton
{
public:
	static ASingleton * getInstance()
	{
		if(m_data == nullptr)			//两次判断，避免每一次获取实例都要枷锁解锁操作。如果有多个线程同时进入这条判断语句，则只有一个线程获得锁并
		{								//构造实例，完了之后解锁，m_data不为nullptr，之后其余线程将不会进入第二条判断语句。
			//Lock();					//多线程情况下加锁保证值只生成一个实例
			if(m_data == nullptr)
				m_data = new ASingleton();
			//Unlock();					//解锁
		}
		return m_data;
	}

	static void DestoryInstance()		//用一个函数来销毁实例，这是一种方法
	{
		if(m_data != nullptr)
		{
			delete m_data;
			m_data = nullptr;
		}
	}

	class Garbo							//添加辅助类的方式来进行自动释放掉new创建的实例，这是另一种方法
	{
	public:
		Garbo()
		{
			cout << "Garbo constructor" << endl;
		}
		~Garbo()			//当static的gc出了函数作用域时，gc会被自动销毁，此时会调用其析构函数，而它的析构函数里又调用了delete释放ASingleton的实例
		{
			cout << "Garbo deconstructor" << endl;
			if(ASingleton::m_data != nullptr)
			{
				cout << "delete single instance" << endl;
				delete ASingleton::m_data;
				ASingleton::m_data = nullptr;
			}
		}
	};
	static Garbo gc;

	void print()
	{
		cout << m_test << "  " << str << endl;
		m_test++;
	}
private:
	ASingleton() = default;
	~ASingleton() { cout << "ASingleton deconstructor" << endl; };
	ASingleton(const ASingleton &) = delete;
	ASingleton & operator=(const ASingleton &) = delete;

	string str = "hello world";
	static ASingleton * m_data;
	int m_test = 1;
};
ASingleton * ASingleton::m_data = nullptr;
ASingleton::Garbo ASingleton::gc;

int main(int argc, char * argv[])
{
	ASingleton * a1;
	a1 = ASingleton::getInstance();
	cout << a1 << endl;
	a1->print();

	ASingleton * a2;
	a2 = ASingleton::getInstance();
	cout << a2 << endl;
	a2->print();

	//ASingleton::DestoryInstance();
/*	ASingleton a3(*a1);		//如果析构函数是private的，则无法创建对象。
	cout << &a3 << endl;
	a3.print();*/

	return 0;
}