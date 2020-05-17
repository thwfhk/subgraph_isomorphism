#include <iostream>
#include "GraphDS.h"
using TGraph::Graph;
using TGraph::edge;


edge::edge() {}
edge::edge(int v, int ne): v(v), ne(ne) {}

void Graph::ins(int u, int v) {
  g[u][v] = g[v][u] = true;
  e[++cnt] = edge(v, h[u]); h[u] = cnt;
  e[++cnt] = edge(u, h[v]); h[v] = cnt;
}
Graph::Graph() {}
Graph::Graph(int node_cnt, int edge_cnt) {
  n = node_cnt; m = edge_cnt;
  nc = node_cnt; ec = edge_cnt;
  cnt = 0; 
  // NOTE: numbered from 1
  h = new int [node_cnt+1] (); 
  degree = new int [node_cnt+1] (); 
  label = new short [node_cnt+1] ();
  g = new bool* [node_cnt+1] ();
  for (int i=0; i<=node_cnt; i++)
    g[i] = new bool [node_cnt+1] ();
  e = new edge [2*edge_cnt+1] ();
}
Graph::~Graph() {
  delete [] h;
  delete [] degree;
  delete [] label;
  for (int i=0; i<=nc; i++)
    delete [] g[i];
  delete [] g;
  delete [] e;
}
void Graph::print() {
  std::cout << "Graph " << id << "\n";
  for(int i=1; i<=n; i++) printf("label %d: %d\n", i, label[i]);
  for(int u=0; u<=n; u++) {
    for(int i=h[u]; i; i=e[i].ne) {
      int v = e[i].v;
      if(v >= u) printf("%d -- %d\n", u, v);
    }
  }
  puts("");
}