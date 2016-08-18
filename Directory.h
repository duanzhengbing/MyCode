// #include "FileSystem.h"
#include <iostream>
extern FileSystem fs;
class Directory
{
public:
	Directory();
	~Directory() {}
};

Directory::Directory()
{
	int disks = fs.Nums();
	std::cout << "disks = " << disks << std::endl;
}
Directory dir; 