#include <iostream>
#include <vector>
using namespace std;
template<class T>
class point
{
public:
	point() {};
	point(T _x,T _y):x(_x),y(_y) {};
	friend ostream& operator<<(ostream& os,const point<T>& rhs);
	point operator+(point<T>& rhs)
	{
		T tmp_x = this->x + rhs.x;
		T tmp_y = this->y + rhs.y;
		point<T> tmp(tmp_x,tmp_y);
		return tmp;
	}
public:
	T x {0};
	T y {1};
	
};

ostream& operator<<(ostream& os,const point<>& rhs)
{
	os << "x = " << rhs.x << endl;
	os << "y = " << rhs.y << endl;
	return os;
}

int main(int argc, char const *argv[])
{
	point<int> a;
	point<int> b(10,12);
	cout << b;
	cout << a;

	point<int> c = b+a;
	cout << c;
	return 0;
}