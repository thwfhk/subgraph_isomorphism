#define GraphQL

#include <iostream>

const int MAX_NODE = 1005, MAX_EDGE = 1e5+5;

struct edge {
  int v, ne;
  edge() {}
  edge(int v, int ne): v(v), ne(ne) {}
};

struct Graph {
  int n, m, cnt;
  int h[MAX_NODE], degree[MAX_NODE], label[MAX_NODE];
  bool g[MAX_NODE][MAX_NODE]; // adjacent matrix
  edge e[MAX_EDGE];
  void ins(int u, int v) {
    g[u][v] =  g[v][u] = true;
    e[++cnt] = edge(v, h[u]); h[u] = cnt;
    e[++cnt] = edge(u, h[v]); h[v] = cnt;
  }
  Graph() {
    cnt = 0;
    memset(h, 0, sizeof(h));
    memset(g, 0, sizeof(g));
  }
};