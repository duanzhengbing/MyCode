#include <iostream>

extern A& Aa();

class B
{
public:
	B() : count(Aa().get_count()) {}
	~B() = default;

	int get_count()
	{
		return count;
	}
private:
	int count;
};




int main()
{
	B b();

	std::cout << b.get_count() << std::endl;
}