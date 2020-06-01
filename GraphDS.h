#ifndef GraphDS
#define GraphDS
#include <iostream>

const int MAXPN = 50; // the maximum size for query graph P
const int MAX_NODE = 1500; // the maximum size for all graphs in the aids dataset
namespace TGraph {

struct edge {
  int v, ne;
  edge();
  edge(int, int);
};

struct Graph {
  std::string id;
  int n, m; // the number of vertices and edges
  int cnt;
  int nc, ec;
  int *h, *degree;
  short *label; // NOTE: you need to map labels to shorts
  bool **g; // the adjacent matrix
  edge *e;
  void ins(int, int);
  Graph();
  Graph(int, int);
  ~Graph();
  void print();
};
/*
How to enumerate all edges start from the vertex u:
  for (int i=h[u]; i; i=e[i].ne) {
    int v = e[i].v;
    short label = e[i].label;

  }
How go get the adjacent matrix: just the bool matrix g.
*/

}

#endif