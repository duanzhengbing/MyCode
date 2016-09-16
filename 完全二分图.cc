#include <iostream>
#include <vector>

using namespace std;

int main()
{
	int M;
	while (cin >> M)
	{
		vector<vector<int> > result;
		for (int i = 0; i < M; ++i)
		{
			int N, S;
			cin >> N >> S;
			std::vector<int> ret(N, -1);
			for(int j = 0; j < N - 1; ++j)
			{
				int a, b;
				cin >> a >> b;
				ret[b-1] = a;
			}
			result.push_back(ret);
		}

		for(int i = 0; i < result.size(); ++i)
		{
			for(int j = 0; j < result[0].size(); ++j)
				cout << result[i][j] << " ";
			cout << endl;
		}	
	}

	return 0;
}