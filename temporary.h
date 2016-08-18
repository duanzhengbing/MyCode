class temporary
{
public:
	temporary() {}
public:
	/* int型（bool,char,int,short int, long int）静态常量可以在类内进行初始化 */
	static const int Num_int = 5;
	static const int Num_long_int = 10;
	static const char Num_char = 65;
	static const bool Num_bool = true;
	/**
	 * const 修饰的double类型静态常量不能再类内初始化
	 * constexpr 修饰的double静态常量则必须在类内初始化
	 */
	static constexpr double Num_double = 10;
};

const char* const str = "duanzhengbing";