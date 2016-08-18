class A
{
public:
	A(int _count) 
		: count(_count) {}
	~A() = default;

	int get_count()
	{
		return count;
	}
private:
	int count;
};