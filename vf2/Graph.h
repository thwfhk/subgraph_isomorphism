#include <cstring>
#include <set>
#include <vector>

namespace UGraph {
struct Graph {
    int n;
    int *deg, *label;
    int **adj;
    std::vector<std::vector<int>> neigh;

    Graph(int);
    void insert(int, int);
    ~Graph();
};
}  // namespace UGraph

struct Digraph {
    int n;
    int *indeg, *outdeg, *label;
    int **adj;
    std::vector<std::set<int>> in, out;

    Digraph(int);
    void insert(int, int);
    ~Digraph();
};
