#ifndef GraphDS
#define GraphDS
#include <iostream>

const int MAXPN = 50; // maximum size for query graph P
const int MAX_NODE = 300; // maximum size for all graphs in the aids dataset

struct edge {
  int v, ne;
  edge();
  edge(int, int);
};

struct Graph {
  std::string id;
  int n, m, cnt;
  int nc, ec;
  int *h, *degree;
  short *label; // NOTE: you need to map labels to shorts
  bool **g;
  edge *e;
  void ins(int, int);
  Graph();
  Graph(int, int);
  ~Graph();
  void print();
};
#endif