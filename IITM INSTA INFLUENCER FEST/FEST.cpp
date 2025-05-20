#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <set>
#include <queue>

using std::cin;
using std::cout;
using std::endl;
using std::string;

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

class isCycle : public GraphAlgorithm
{ // derived class from GraphAlgorithm
public:
    isCycle(std::vector<std::set<int>> &d) : GraphAlgorithm(d)
    {
        makedfs();
    }
    void query() override
    { // this query processes if there is cycles in graph
        if (iscycle)
            cout << "YES\n";
        else
            cout << "NO\n";
    }
};

class indepComponent : public GraphAlgorithm
{ // derived class from GraphAlgorithm
public:
    indepComponent(std::vector<std::set<int>> &d) : GraphAlgorithm(d)
    {
        makedfs();
        findscc();
        indepcomponents = scc.size() - 1;
        max_no_vertices = 0;
        for (int i = 1; i <= indepcomponents; i++)
        {
            if ((int)scc[i].size() > max_no_vertices)
                max_no_vertices = (int)scc[i].size();
        }
    }
    void query() override
    { // computes cardinalities of strongly connected components
        cout << indepcomponents << " " << max_no_vertices << endl;
    }
    int indepcomponents, max_no_vertices; // int value to store computed values
};

class validOrder : public GraphAlgorithm
{ // derived class from GraphAlgorithm
public:
    validOrder(std::vector<std::set<int>> &d) : GraphAlgorithm(d)
    {
        makedfs();
        topsort();
    }
    void query() override
    { // prints topological sort of graph if "NO" cycles exist and prints no otherwise
        if (iscycle)
        {
            cout << "NO\n";
            return;
        }
        int size = validtoporder.size();
        for (int i = 0; i < size; i++)
            cout << validtoporder[i] << " ";
        cout << endl;
    }
};

class maxHype : public GraphAlgorithm
{ // derived class from GraphAlgorithm
public:
    maxHype(std::vector<std::set<int>> &d, std::vector<int> &h) : GraphAlgorithm(d)
    {
        hypescores = h;
        makedfs();
        findscc();
        findmaxhype();
    }
    void findmaxhype();
    void query() override
    { // prints maxHype score achievable in a valid path
        cout << maxhype << endl;
    }
    bool computed;         // bool value for this graph algorithm is already computed or not
    long long int maxhype; // int value to store computed values
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


void maxHype::findmaxhype()
{
    int n = scc.size();
    int scchype = 0;
    scc_hypescores.resize(n, 0);
    for (int i = 1; i < n; i++)
    {
        auto &x = scc[i];
        scchype = 0;
        for (auto &y : x)
        {
            sccmap[y] = i;
            scchype += hypescores[y];
        }
        scc_hypescores[i] = scchype;
    }
    scc_dependencies.resize(n, {});

    for (int i = 1; i < (int)dependencies.size(); i++)
    {
        auto &x = dependencies[i];
        for (auto &y : x)
        {
            if (sccmap[i] != sccmap[y])
            {
                scc_dependencies[sccmap[i]].insert(sccmap[y]); // check here
            }
        }
    }
    
    GraphAlgorithm *s = new isCycle(scc_dependencies);
    
    std::vector<int> dp(n, 0);
    
    s->topsort();

    int max_hype = 0;
    // std::reverse(s->toporder.begin(),s->toporder.end());
    for (int u : s->validtoporder)
    {
        dp[u] += scc_hypescores[u]; // Add hype score of this SCC
        max_hype = std::max(max_hype, dp[u]);
        for (int v : scc_dependencies[u])
        {
            dp[v] = std::max(dp[v], dp[u]);
        }
    }
    maxhype = max_hype;
}

int main()
{
    int N, M; // N = number of events and M = number of dependencies
    cin >> N >> M;

    std::vector<int> hypescores(N + 1); // vector for hypescores for all events respectively

    for (int i = 1; i <= N; i++)
    {
        cin >> hypescores[i];
    }

    std::vector<std::set<int>> dependencies; // map for dependencies
    
    dependencies.resize(N + 1, {});
    
    for (int i = 0; i < M; i++)
    { // Storing dependencies in map
        int u, v;
        cin >> u >> v;
        dependencies[u].insert(v);
    }
    
    int Q, type; // Q = number of queries , type = query type
    
    cin >> Q;
    
    GraphAlgorithm *q1 = new isCycle(dependencies);
    
    GraphAlgorithm *q2 = new indepComponent(dependencies);
    
    GraphAlgorithm *q3 = new validOrder(dependencies);
    
    GraphAlgorithm *q4 = new maxHype(dependencies, hypescores);
    
    while (Q--)
    { // will pass dependencies later
        cin >> type;
        if (type == 1)
        {
            q1->query();
        }
        else if (type == 2)
        {
            q2->query();
        }
        else if (type == 3)
        {
            q3->query();
        }
        else if (type == 4)
        {
            q4->query();
        }
    }
}