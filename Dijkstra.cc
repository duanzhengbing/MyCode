#include <iostream>
#include <deque>
#include <vector>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <stack>
#include <queue>
#include <climits>
using namespace std;

using VertexType = int;
using EdgeType = int;
using GraphType = map<VertexType, map<VertexType, EdgeType>>;
const int INFINITY = INT_MAX;
/**
 * 单源最短路径，Dijkstra算法，既适用于无向图也适用有向图
 * 顶点集合为V，已经计算出最短路径的集合为S
 * 
 */
void ShortestPath_Dij(GraphType Graph, VertexType v0, map<VertexType, EdgeType>& Dij)
{
    int vexnum = Graph.size();	//顶点个数
    unordered_set<VertexType> vertexs; // 顶点集合V

    unordered_set<VertexType> known;   // 已经计算出最短路径的顶点S
    map<VertexType, VertexType> path;  // 顶点最短路径的上一个顶点

    //初始化最短路径为无穷大
    for(auto i = Graph.begin(); i != Graph.end(); ++i)
    {
    	vertexs.insert(i->first);
		Dij[i->first] = INFINITY;    	
    }

    Dij[v0] = 0;	// v0到自己的路径长度为零
    path[v0] = v0;	// 记录当前顶点在最短路径上的前一个顶点，为输出路径适用

    /* 每一次循环可以最终确定一个顶点的最短路径 */
    for(int k = 1; k < vexnum; ++k)
    {
	    VertexType min_v;
	    int min_dis = INFINITY;
	    // 查找尚未计算最短路径并且到v0最短的顶点
	    // 而不是广度优先遍历每一个邻接顶点
	    for(auto v : vertexs) 
	    {
	    	if(known.count(v) == 0 && Dij[v] < min_dis)
	    	{
	    		min_v = v;
	    		min_dis = Dij[v];
	    	}
	    }

	    known.insert(min_v);

	    for(auto i : Graph[min_v])
	    {
	    	if(known.count(i.first) == 0 && i.second + Dij[min_v] < Dij[i.first])
	    	{
	    		Dij[i.first] = i.second + Dij[min_v];
	    		path[i.first] = min_v;
	    	}
	    }
    	
    }

}



int main(int argc, char const *argv[])
{
	// map<VertexType, map<VertexType, EdgeType>> graph
 //    {
 //        {0, {{2, 10}, {4, 30}, {5, 100}}}, 
 //        {1, {{2, 5}}}, 
 //        {2, {{3, 50}}}, 
 //        {3, {{5, 10}}}, 
 //        {4, {{3, 20}, {5, 60}}}, 
 //    };

    map<VertexType, map<VertexType, EdgeType>> graph
    {
        {0, {{1, 1}, {2, 5}}}, 
        {1, {{0, 1}, {4, 5}, {3, 7}, {2, 3}}}, 
        {2, {{0, 5}, {1, 3}, {4, 1}, {5, 7}}}, 
        {3, {{1, 7}, {4, 2}, {6, 3}}}, 
        {4, {{1, 5}, {2, 1}, {3, 2}, {5, 3}, {6, 6}, {7, 9}}}, 
        {5, {{2, 7}, {4, 3}, {7, 5}}}, 
        {6, {{3, 3}, {4, 6}, {7, 2}, {8, 7}}}, 
        {7, {{4, 9}, {5, 5}, {6, 2}, {8, 4}}},
        {8, {{6, 7}, {7, 4}}} 
    };
    map<VertexType, EdgeType> Dij;
    ShortestPath_Dij(graph, 0, Dij);
    for(auto i : Dij)
    {
    	cout << i.first << " " << i.second << endl;
    }
	return 0;
}