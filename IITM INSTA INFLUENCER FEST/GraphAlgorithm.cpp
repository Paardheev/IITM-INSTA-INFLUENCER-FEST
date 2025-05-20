#include<vector>
#include<unordered_map>
#include<set>
#include<queue>

class GraphAlgorithm
{ // base class for graph algorithms (for queries)
public:
    GraphAlgorithm() {}
    GraphAlgorithm(std::vector<std::set<int>> &d) { dependencies = d; }
    virtual void query() = 0; // virtual function since function implementation is different in each derived classes
    void makedfs();
    void dfs(std::set<int> &s, int u);
    void dfs(int u);
    void findscc();
    void topsort();

public:
    std::vector<std::set<int>> dependencies; // map for dependencies
    std::vector<char> color;                 // vector for colors
    bool iscycle;                            // bool value to store computed value
    std::vector<int> toporder;               // vector to value to store computed values
    std::vector<std::set<int>> scc;
    std::unordered_map<int, int> sccmap;
    std::vector<std::set<int>> transpose;
    std::vector<std::set<int>> scc_dependencies;
    std::vector<int> hypescores;
    std::vector<int> scc_hypescores;
    std::vector<int> validtoporder;
};

void GraphAlgorithm::makedfs()
{
    int n = dependencies.size();
    iscycle = false;
    color.resize(n, 'w');
    for (int i = 1; i < n; i++)
    {
        if (color[i] == 'w')
        {
            dfs(i);
        }
    }
}

void GraphAlgorithm::dfs(int u)
{
    color[u] = 'g';
    for (auto &x : dependencies[u])
    {
        if (color[x] == 'g')
            iscycle = true;
        else if (color[x] == 'w')
        {
            dfs(x);
        }
    }
    color[u] = 'b';
    toporder.push_back(u);
}

void GraphAlgorithm::dfs(std::set<int> &s, int u)
{
    color[u] = 'g';
    for (auto &x : transpose[u])
    {
        if (color[x] == 'g')
            iscycle = true;
        else if (color[x] == 'w')
        {
            dfs(s, x);
        }
    }
    color[u] = 'b';
    s.insert(u);
}

void GraphAlgorithm::findscc()
{
    int n = dependencies.size();
    transpose.resize(n, {});
    for (int i = 1; i < n; i++)
    { // transpose works fine
        for (auto &y : dependencies[i])
        {
            transpose[y].insert(i);
        }
    }
    iscycle = false;
    for (int i = 0; i < n; i++)
    {
        color[i] = 'w';
    }
    scc.push_back({});
    for (int i = n - 2; i >= 0; i--)
    {
        if (color[toporder[i]] == 'w')
        {
            std::set<int> s;
            dfs(s, toporder[i]);
            scc.push_back(s);
        }
    }
}

void GraphAlgorithm::topsort()
{
    int n = dependencies.size();
    std::vector<int> indegree(n, 0);
    for (int i = 1; i < n; i++)
    {
        for (int x : dependencies[i])
        {
            indegree[x]++;
        }
    }
    std::priority_queue<int, std::vector<int>, std::greater<int>> pq;
    for (int i = 1; i < n; i++)
    {
        if (indegree[i] == 0)
        {
            pq.push(i);
        }
    }
    while (!pq.empty())
    {
        int u = pq.top();
        pq.pop();
        validtoporder.push_back(u);
        for (int v : dependencies[u])
        {
            if (--indegree[v] == 0)
            {
                pq.push(v);
            }
        }
    }
}
