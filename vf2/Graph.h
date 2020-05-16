#ifndef Graph_wc
#define Graph_wc
#include <cstring>
#include <set>
#include <vector>

struct UGraph {
    int n;
    int *deg, *label;
    int **adj;
    std::vector<std::vector<int>> neigh;

    UGraph(int);
    void insert(int, int);
    ~UGraph();
};

struct Digraph {
    int n;
    int *indeg, *outdeg, *label;
    int **adj;
    std::vector<std::set<int>> in, out;

    Digraph(int);
    void insert(int, int);
    ~Digraph();
};

#endif