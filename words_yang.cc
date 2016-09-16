/**
 * author: duanzhengbing
 * date: 2016/8/22
 * copyright@duan
 */
#include <iostream>
#include <deque>
#include <string.h>
#include <string>
#include <algorithm>

using namespace std;

template<typename T, unsigned N>
void Init1D(T(&p)[N])
{
	for (int i = 0; i < N; ++i)
	{
		p[i] = T(0);
	}
}

template<typename T, unsigned N, unsigned M>
void Init2D(T(&p)[N][M])
{
	for (int i = 0; i < N; ++i)
	{
		for (int j = 0; j < M; ++j)
		{
			p[i][j] = T(0);
		}
	}
}

// 判断有向图是否弱连通
bool isConnected(int start, int vertexNum, bool (*directedGraph)[26])
{
	bool undirectedGraph[26][26];		
	Init2D(undirectedGraph);

	for (int i = 0; i < 26; i++)		// 把有向图转换成无向图
	{
		for (int j = 0; j < 26; j++)
		{
			if (directedGraph[i][j])
			{
				undirectedGraph[i][j] = true;
				undirectedGraph[j][i] = true;
			}
		}
	}

	auto deq = deque<int>();
	bool passedVertex[26];				
	Init1D(passedVertex);

	deq.push_back(start);

	// 从起点开始进行广度优先搜索，把路过的边都拆掉
	while (!deq.empty())
	{
		int currentVertex = deq.front();
		deq.pop_front();

		passedVertex[currentVertex] = true;

		for (int vertex = 0; vertex < 26; vertex++)
		{
			if (undirectedGraph[currentVertex][vertex] == 1 && passedVertex[vertex] == false)
			{
				undirectedGraph[currentVertex][vertex] = 0;
				undirectedGraph[vertex][currentVertex] = 0;
				deq.push_back(vertex);
			}
		}
	}

	int passedVertexNum = 0;
	for (int vertex = 0; vertex < 26; vertex++)
	{
		if (passedVertex[vertex])
			passedVertexNum++;
	}

	// 遍历到所有的点，证明无向图是连通的
	if (passedVertexNum == vertexNum)
		return true;
	else
		return false;
}

bool canArrangeWords(int num,  char** arr)
{
	// 26个英文字母看作26个点,用整数0-25来表示
	bool directedGraph[26][26];		Init2D(directedGraph);				// 邻接矩阵表示有向图
	int inDegree[26];				Init1D(inDegree);					// 顶点入度
	int outDegree[26];				Init1D(outDegree);					// 顶点出度
	bool hasLetter[26];				Init1D(hasLetter);					// 标记字母是否出现过
	bool hasEuler = true;												// 有狭义欧拉路径或欧拉回路标志

	for (int i = 0; i < num; ++i)
	{
		char* word = arr[i];
		char firstLetter = word[0];
		char lastLetter = word[strlen(word) - 1];
		++outDegree[firstLetter - 'a'];
		++inDegree[lastLetter - 'a'];
		directedGraph[firstLetter - 'a'][lastLetter - 'a'] = true; // 有向图
		hasLetter[firstLetter - 'a'] = true;
		hasLetter[lastLetter - 'a'] = true;
	}

	int startNum = 0;
	int endNum = 0;
	for (int vertex = 0; vertex < 26; ++vertex)
	{
		if (outDegree[vertex] - inDegree[vertex] == 1)    // 起点
			startNum++;
		if (inDegree[vertex] - outDegree[vertex] == 1)    // 终点
			endNum++;
		if (abs(inDegree[vertex] - outDegree[vertex]) > 1)
		{
			hasEuler = false;
			break;
		}
	}

	bool isEulerPath = (startNum == 1 && endNum == 1);		// 这里指狭义上的欧拉路径，不包括欧拉回路
	bool isEulerCircuit = (startNum == 0 && endNum == 0);	// 欧拉回路
	if ((!isEulerPath) && (!isEulerCircuit))				// 既不是欧拉路径也不是欧拉回路
		hasEuler = false;

	// 判断是否弱连通
	int vertexNum = 0;    // 点的数量
	for (int letter = 0; letter < 26; letter++)
	{
		if (hasLetter[letter])
			vertexNum++;
	}

	int firstWordFirstLetter = arr[0][0] - 'a';				// 以第一个单词的首字母作为起点

	return hasEuler && isConnected(firstWordFirstLetter, vertexNum, directedGraph);
}




int main()
{

	int num;
	while (cin >> num)
	{
		char** arr = new char*[num];

		for (int i = 0; i < num; ++i)
		{
			char* tmp = new char[15];
			cin >> tmp;
			arr[i] = tmp;
		}

		if (canArrangeWords(num, arr))
			cout << "Yes" << endl;
		else
			cout << "No" << endl;

		for (int i = 0; i < num; ++i)
			delete arr[i];
		delete arr;
	}



	//system("pause");
	return 0;
}
