#include "FileSystem.h"

FileSystem::FileSystem(int val) : nums(val) {}

int FileSystem::Nums() const
{
	return nums;
}

FileSystem fs(1000);

// extern FileSystem fs;
// int main(int argc, char const *argv[])
// {
	
// 	return 0;
// }