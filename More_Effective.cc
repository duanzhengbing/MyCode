#include <iostream>
#include <vector>
#include <list>
#include <exception>
using namespace std;

class BST
{
public:
	BST() : val(0) {}
	BST(int _val) : val(_val) {}
	int getVal() const { return val; }
	virtual ~BST() {cout << "base destructor" << endl; }
private:
	int val;
};

ostream& operator<<(ostream& os, const BST& b)
{
	os << b.getVal() << " ";
	return os;
}

class BalanceBST : public BST
{
public:
	BalanceBST() : BST() {}
	BalanceBST(int _val) : BST(_val) {}
	int getNum() const { return num; }
	virtual ~BalanceBST() { cout << "derived destructor" << endl; }
private:
	int num {999};
};

ostream& operator<<(ostream& os, const BalanceBST& bb)
{
	os << bb.getVal() << " " << bb.getNum()<< " ";
	return os;
}

void printBSTArray(ostream& s, const BST array[], int numElements)
{
	for (int i = 0; i < numElements; ++i) 
	{
		s << array[i]; 
	}  
	s << endl;
} // for BST objects

// delete an array, but first log a message about its
// deletion
void deleteArray(ostream& logStream, BST array[])
{
	logStream << "Deleting array at address " 
	<< static_cast<void*>(array) << "\n";
	delete [] array;
}


class rational
{
public:
	rational(int _a, int _b) : a(_a), b(_b) {}
	operator double() const
	{
		double tmpa = static_cast<double>(a);
		double tmpb = static_cast<double>(b);
		return tmpa / tmpb;
	}

	~rational() {}

private:
	int a;
	int b;

};


class Array 
{
public:
	class ArraySize 
	{ // 这个类是新的
	public:
		ArraySize(int numElements): theSize(numElements) 
		{
			std::cout << "ArraySize Constructor" << std::endl;
		}
		int size() const { return theSize; }
	private:
		int theSize;
	};

	Array(int lowBound, int highBound) {}
	Array(ArraySize size) {}
};

class MyException
{
public:
	MyException()
	{
		cout << "default Constructor" << endl;
	}
	MyException(const MyException& rhs)
	{
		cout << "copy    Constructor" << endl;
	}
	~MyException()
	{
		cout << "Destructor" << endl;
	}

	int val {18};
};

void funct()
{
	static MyException ex;
	throw &ex;
}


int main(int argc, char const *argv[])
{
	try
	{
		funct();
	}
	catch(MyException* e)
	{
		cout << e->val;
	}

	return 0;
} 