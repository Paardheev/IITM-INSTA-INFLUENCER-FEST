#include"GraphAlgorithm.cpp"
#include<iostream>

using std::cout ;
using std::endl;

class isCycle : public GraphAlgorithm
{ // derived class from GraphAlgorithm
public:
    isCycle(std::vector<std::set<int>> &d) : GraphAlgorithm(d)
    {
        makedfs();
    }
    void query() override;
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