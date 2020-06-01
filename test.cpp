#include <iostream>
#include <fstream>
#include <cstdio>
#include <algorithm>
#include <cstring>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <queue>
#include <cstdlib>
#include <ctime>
#include "GraphDS.h"
#include "GraphQL/GraphQL.cpp" // NOTE: Replace with your file here.
// #include "GraphQL/GraphQL2.cpp" // NOTE: Replace with your file here.
// #include "VF2/driver_VF2.cpp" 
// #include "VF2/driver_Boost.cpp" 
// #include "Ullmann/driver_Ullman.cpp" 
// #include "QuickSI/QuickSI.cpp" // Please compile with GraphDS.cpp QuickSI/IsoSolver.cpp QuickSI/Graph.cpp QuickSI/QISeqEntry.cpp QuickSI/util.cpp
// #include "QuickSI/QuickSI_with_selected_candidates.cpp" // Please compile with GraphDS.cpp QuickSI/IsoSolver.cpp QuickSI/Graph.cpp QuickSI/QISeqEntry.cpp QuickSI/util.cpp
using namespace std;
// using TGraph::Graph;
// using TGraph::edge;

const int AIDS_TOTAL = 42687;
const int GRAPHN = 42687 + 5;

TGraph::Graph* gs[GRAPHN];
TGraph::Graph* qs[GRAPHN];

int getrand(int n) {
  return rand() % n + 1;
}

void load_data(string file_name, int data_num, TGraph::Graph* gs[GRAPHN]) {
  ifstream fin(file_name);
  for(int i = 1; i <= data_num; i++) {
    // printf("i %d\n", i);
    string s;
    int n, m;
    getline(fin, s);
    fin >> n >> m; //printf("nm %d %d\n", n, m);
    gs[i] = new TGraph::Graph(n, m);
    gs[i]->id = to_string(i);
    for(int j=1; j<=n; j++) {
      int l;
      fin >> l;
      gs[i]->label[j] = l;
    }
    for(int j=1; j<=m; j++) {
      int u, v;
      fin >> u >> v;
      gs[i]->ins(u, v);
    }
    getline(fin, s); // "\n"
    getline(fin, s); // "\n"
  }
}

// set_number 4 8 12 16 20 24
void test(int set_number) {
  // printf("test begin --- queryset: %d\n", set_number);
  // load_data("dataset/aids/aids.data", 1000, gs);
  // load_data("dataset/aids/query" + to_string(set_number) + ".data", 1000, qs);
  load_data("dataset/dblp_withlabel/dblp500/dblp500.data", 100, gs);
  load_data("dataset/dblp_withlabel/dblp500/query" + to_string(set_number) + ".data", 100, qs);
  double total_time = 0;
  int qn = 100, gn = 100; // NOTE: only 100 here!
  int total_match = 0;
  #ifdef QUICKSI
  initialize(100, gs);
  #endif
  for (int i=1; i<=qn; i++)
    for (int j=1; j<=gn; j++) {
      // printf("i j %d %d\n", i, j);
      TGraph::Graph &P = *qs[i], &G = *gs[j];
      double begin = clock();
      bool res = solve(P, G);
      total_match += res;
      total_time += (clock() - begin) / CLOCKS_PER_SEC;
      // cout << "sub-iso " << P.id << " " << G.id << " " << solve(P, G) << "\n";
      // P.print(); G.print();
    }
  printf("average time of queryset %d: %lf\n", set_number, total_time / (qn*gn));
  printf("total match %d / %d\n", total_match, qn*gn);
}

int main() {
  vector<int> vec {4, 8, 12, 16, 20, 24};
  // vector<int> vec {8};
  for (int v : vec) test(v);
}