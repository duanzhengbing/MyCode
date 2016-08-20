#include <iostream>
#include <vector>
using namespace std;

class BST
{
public:
	BST() : val(0) {}
	BST(int _val) : val(_val) {}
	int get() const { return val; }
	virtual ~BST() {}
private:
	int val;
};

ostream& operator<<(ostream& os, const BST& b)
{
	os << b.get() << " ";
	return os;
}

class BalanceBST : public BST
{
public:
	BalanceBST() : BST() {}
	BalanceBST(int _val) : BST(_val) {}
	virtual ~BalanceBST() {}
private:
	int num {999};
};

ostream& operator<<(ostream& os, const BalanceBST& bb)
{
	os << bb.get() << " ";
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


int main(int argc, char const *argv[])
{
	constexpr int arrSize = 5;
	BST arr[arrSize] = {BST(1), BST(2), BST(3), BST(4)};
	printBSTArray(cout, arr, arrSize);
	

	BalanceBST arr2[arrSize] = 
	{ 	
		BalanceBST(10), 
		BalanceBST(21), 
		BalanceBST(31), 
		BalanceBST(41)
	};

	printBSTArray(cout, arr2,arrSize);


	BalanceBST *balTreeArray = new BalanceBST[50];

	deleteArray(cout, balTreeArray); // log its deletion
	return 0;
}