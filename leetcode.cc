#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <cassert>
#include <bitset>
using namespace std;

bool isSelfCrossing(vector<int>& x) 
{
	x.insert(x.begin(), 4, 0);

    int len = x.size();
    int i = 4;

    // outer spiral
    for (; i < len && x[i] > x[i - 2]; i++);

    if (i == len) return false;

    // check border
    if (x[i] >= x[i - 2] - x[i - 4])
    {
        x[i - 1] -= x[i - 3];
    }

    // inner spiral
    for (i++; i < len && x[i] < x[i - 2]; i++);

    return i != len;
}



class leetcode
{
public:
	leetcode() {}
	void print()
	{
		cout << "sss" << endl;
	}
	~leetcode() {}
	
};


int main(int argc, char const *argv[])
{
	leetcode l;
	l.print();
	return 0;
}