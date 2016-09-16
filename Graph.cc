#include <iostream>
#include <deque>
#include <vector>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <stack>
#include <climits>
using namespace std;

/**
 * 邻接矩阵的广度优先搜索，顶点可以是任何类型，只需要修改访问顶点的接口即可
 */
using VertexType = int;
using EdgeType = int;
using GraphType = map<VertexType, set<VertexType>>;

inline void visit(const VertexType& v)
{
    cout << v << " ";
}

/**
 * 广度优先搜索，Breadth-First Search
 */
void BFS(map<VertexType, set<VertexType>>& graph)
{
    int vertexs = graph.size();
    if(vertexs == 0)
        return;
    //对每个顶点设置一个标志位，记录该顶点是否被访问过
    std::unordered_map<VertexType, bool> visited;
    for (auto iter = graph.begin(); iter != graph.end(); ++iter)
    {
        visited[iter->first] = false;
    }

    std::deque<VertexType> que;
    //考虑非连通图的情况，必须遍历每一个连通子图
    for (auto iter = graph.begin(); iter!= graph.end(); ++iter)
    {
        if(!visited[iter->first])
        {
            que.push_back(iter->first);
            while(!que.empty())
            {
                VertexType cur_vertex = que.front();
                que.pop_front();
                //访问当前节点并将当前节点设置为已访问状态
                if(!visited[cur_vertex])
                {
                    visit(cur_vertex);
                    visited[cur_vertex] = true;
                }

                for(auto it = graph[cur_vertex].begin(); it != graph[cur_vertex].end(); ++it)
                {
                    //邻接顶点没有访问过，则入队列
                    if(!visited[*it])
                        que.push_back(*it);
                }

            }
        }
    }
}

/**
 * 从vex顶点出发递归的深度优先遍历图G
 * @param graph   [description]
 * @param visited [记录某个顶点是否已经访问过]
 * @param vex     [当前访问的顶点]
 */
void dfs_search(GraphType graph, std::unordered_map<VertexType, bool>& visited, VertexType vex)
{
    visit(vex);
    visited[vex] = true;
    //对当前顶点的每一个邻接顶点递归深度优先搜索
    for (auto i = graph[vex].begin(); i != graph[vex].end() ; ++i)
    {
        if(visited[*i] == 0)
            dfs_search(graph, visited, *i);
    }
}

void DFS(GraphType graph)
{
    int vertexs = graph.size();
    if(vertexs == 0)
        return;

    //对每个顶点设置一个标志位，记录该顶点是否被访问过
    std::unordered_map<VertexType, bool> visited;
    for (auto iter = graph.begin(); iter != graph.end(); ++iter)
    {
        visited[iter->first] = false;
    }
    /**
     * 如果图是非连通的，存在多个连通子图，那么从一个顶点出发是无法遍历完全的
     */
/*    for (auto iter = visited.begin(); iter != visited.end(); ++iter)
    {
        if(!iter->second)
            dfs_search(graph, visited, iter->first);
    } */   
    for (auto iter = graph.begin(); iter != graph.end(); ++iter)
    {
        if(!visited[iter->first])
            dfs_search(graph, visited, iter->first);
    }
}

/**
 * The basic idea is "keep deleting leaves layer-by-layer, until reach the root."
 * Specifically, first find all the leaves,
 * then remove them. After removing, 
 * some nodes will become new leaves. 
 * So we can continue remove them. 
 * Eventually, there is only 1 or 2 nodes left. 
 * If there is only one node left, it is the root. 
 * If there are 2 nodes, either of them could be a possible root.
 */
vector<int> findMinHeightTrees(int n, vector<pair<int, int>>& edges) 
{
    std::vector<int> roots;
    if(edges.size() == 0)
    {
        roots.push_back(0);
        return roots;
    }

    unordered_map<int, unordered_set<int>> graph;
    for (int i = 0; i < edges.size(); ++i)
    {
        graph[edges[i].first].insert(edges[i].second);
        graph[edges[i].second].insert(edges[i].first);
    }        

    //find all leaves
    std::vector<int> leaves;
    for (auto i = graph.begin(); i != graph.end(); ++i)
    {
        if(i->second.size() == 1)
            leaves.push_back(i->first);
    }
    
    //remove leaves layer-by-layer
    while(!leaves.empty())
    {
        std::vector<int> tmp; //临时保存一层的顶点
        for(int cur_vertex : leaves)
        {
            for(int inter_vertex : graph[cur_vertex])
            {
                graph[inter_vertex].erase(cur_vertex);
                if(graph[inter_vertex].size() == 1)
                    tmp.push_back(inter_vertex);
            }
        }
        roots = leaves;
        leaves = tmp;
    }

    return roots;
}

bool TopologicalSort(GraphType graph)
{
    std::vector<VertexType> sequence;
    map<VertexType, int> indegree;
    for(auto iter = graph.begin(); iter != graph.end(); ++iter)
    {
        for(auto it = iter->second.begin(); it != iter->second.end(); ++it)
        {
            indegree[*it]++;
        }
    }

    deque<VertexType> que; //保存入度为零的顶点
    for(auto i : graph)
    {
        //考虑非连通图的一种情况
        if(indegree.count(i.first) == 0)
        {
            indegree[i.first] = 0;
            que.push_back(i.first);
        }
    }

    while(!que.empty())
    {
        VertexType tmp = que.front();
        que.pop_front();
        sequence.push_back(tmp);
        cout << tmp << " ";
        for(auto iter = graph[tmp].begin(); iter != graph[tmp].end(); ++iter)
        {
            --indegree[*iter];
            if(indegree[*iter] == 0)
                que.push_back(*iter);
        }
    }

    //所有的顶点已全部输出则不存在环路
    if(sequence.size() == indegree.size())
        return true;
    else
        return false;
}

bool canFinish(int numCourses, vector<pair<int, int>>& prerequisites) 
{
    map<int, set<int>> graph;
    for (auto i : prerequisites)
    {
        graph[i.first].insert(i.second);
    }
    for (int i = 0; i < numCourses; ++i)
    {
        if(graph.count(i) == 0)
            graph[i] = set<int>();
    }        

    return TopologicalSort(graph);

}


/**
 * 单源最短路径，Dijkstra算法
 * 
 */
void ShortestPath_Dij(GraphType Graph, int v0, std::vector<int>& D)
{
    int vexnum = Graph.size();
    
}
int main(int argc, char const *argv[])
{
    // map<VertexType, set<VertexType>> graph
    // {
    //     {1, {2, 3, 4}}, 
    //     {3, {2, 5}}, 
    //     {4, {5}}, 
    //     {6, {4, 5}},
    // };
    // cout << "DFS : ";
    // DFS(graph);
    // cout << endl << "BFS : ";
    // BFS(graph);
    // cout << endl;
    // vector<pair<int, int>> v{{1,0}};
    // cout << boolalpha << canFinish(3, v);
    


    return 0;
}