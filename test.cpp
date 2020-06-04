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

#include "GraphQL/GraphQL.cpp"
// #include "GraphQL/GraphQL_opt1_only.cpp"
// #include "GraphQL/GraphQL_opt2x.cpp"

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
  int labelMax = 0;
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
      labelMax = max(labelMax, l);
    }
    for(int j=1; j<=m; j++) {
      int u, v;
      fin >> u >> v;
      gs[i]->ins(u, v);
    }
    getline(fin, s); // "\n"
    getline(fin, s); // "\n"
  }
  // printf("The Max Label: %d\n", labelMax);
}

// set_number 4 8 12 16 20 24
void test(string data_name, int set_number) {
  // printf("test begin --- %s --- queryset: %d\n", data_name.c_str(), set_number);
  load_data(data_name + "graphs.data", 100, gs);
  load_data(data_name + "query" + to_string(set_number) + ".data", 100, qs);
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
      #ifdef QUICKSI
      bool res = solve(P, G, j);
      #endif
      #ifndef QUICKSI
      bool res = solve(P, G);
      #endif
      total_match += res;
      total_time += (clock() - begin) / CLOCKS_PER_SEC;
      // cout << "sub-iso " << P.id << " " << G.id << " " << solve(P, G) << "\n";
      // P.print(); G.print();
    }
  printf("average time of queryset %d: %lf\n", set_number, total_time / (qn*gn));
  printf("total match %d / %d\n", total_match, qn*gn);
}

int main() {
  vector<string> data_directory_list = {
    "dataset/aids/", 
    // "dataset/dblp_withlabel/dblp100/", 
    "dataset/dblp_withlabel/dblp500/",
    // "dataset/dblp_withlabel/dblp1000/"
  };
  vector<int> vec {4, 8, 12, 16, 20, 24};
  // vector<int> vec {20};
  string data_name = data_directory_list[3];
  for (int v : vec) test(data_name, v);
}