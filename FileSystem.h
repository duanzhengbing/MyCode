#include <iostream>
using namespace std;
class FileSystem
{
public:
	FileSystem(int val);
	int Nums() const;

private:
	int nums;	
};


extern FileSystem fs;