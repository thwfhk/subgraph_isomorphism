#include "Graph.h"

// using namespace UGraph;

UGraph::UGraph(int n) : n(n) {
    for (int i = 0; i < n; i++) neigh.push_back(std::vector<int>());
    deg = new int[n];
    label = new int[n];
    adj = new int *[n];
    for (int i = 0; i < n; i++) {
        adj[i] = new int[n];
        memset(adj[i], 0, n * sizeof(int));
    }
    memset(deg, 0, n * sizeof(int));
    memset(label, 0, n * sizeof(int));
}

void UGraph::insert(int i, int j) {
    deg[i]++;
    deg[j]++;
    adj[i][j] = adj[j][i] = 1;
    neigh[i].push_back(j);
    neigh[j].push_back(i);
}

UGraph::~UGraph() {
    delete[] deg;
    delete[] label;
    for (int i = 0; i < n; i++) delete[] adj[i];
    delete[] adj;
}

Digraph::Digraph(int n) : n(n) {
    for (int i = 0; i < n; i++) {
        in.push_back(std::set<int>());
        out.push_back(std::set<int>());
    }
    indeg = new int[n];
    outdeg = new int[n];
    label = new int[n];
    adj = new int *[n];
    for (int i = 0; i < n; i++) {
        adj[i] = new int[n];
        memset(adj[i], 0, n * sizeof(int));
    }
    memset(indeg, 0, n * sizeof(int));
    memset(outdeg, 0, n * sizeof(int));
    memset(label, 0, n * sizeof(int));
}

void Digraph::insert(int i, int j) {
    outdeg[i]++;
    indeg[j]++;
    adj[i][j] = 1;
    in[j].insert(i);
    out[i].insert(j);
}

Digraph::~Digraph() {
    delete[] indeg;
    delete[] outdeg;
    delete[] label;
    for (int i = 0; i < n; i++) delete[] adj[i];
    delete[] adj;
}