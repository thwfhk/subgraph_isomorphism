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
#include "GraphDS.h"
#include "GraphQL.cpp"
using namespace std;

const int AIDS_TOTAL = 42687;
const int GRAPHN = 42687 + 5;

Graph* gs[GRAPHN];

int getrand(int n) {
  return rand() % n + 1;
}

int vis_cnt = 0;
bool vis[300];
bool dfs(int u, Graph &G, Graph &P, int &rmsize, map<int, int> &g2p) {
  // printf("dfs %d %d\n", u, rmsize);
  if (rmsize == 0) return true;
  vector<int> vec;
  for (int i = G.h[u]; i; i = G.e[i].ne) {
    int v = G.e[i].v;
    if (!vis[v]) {
      vec.push_back(v);
    }
    else if (!P.g[g2p[u]][g2p[v]]) {
      if (getrand(10) >= 2) {
        P.ins(g2p[u], g2p[v]);
        P.m++;
      }
    }
  }
  if (vec.size() == 0) return false;
  // int cur_choose = getrand(min((int)vec.size(), rmsize));
  // while (G.n - vis_cnt - vec.size() < rmsize - cur_choose) cur_choose++;
  random_shuffle(vec.begin(), vec.end());
  for (int v : vec) {
    vis[v] = true; vis_cnt++;
    g2p[v] = ++P.n; //printf("g2p %d -> %d\n", v, P.n);
    P.label[P.n] = G.label[v];
    P.ins(g2p[u], g2p[v]);
    P.m++;
    rmsize--;
    if (rmsize == 0) return true;
  }
  for (int v : vec) {
    if (dfs(v, G, P, rmsize, g2p)) return true;
  }
  return false;
}

pair<bool, Graph*> generate_subgraph(Graph &G, int psize) {
  int old_psize = psize;
  if (G.n < psize) cerr << "generate_subgraph ERROR: |P-> > |G|\n";
  Graph *P = new Graph(psize, psize * psize);
  P->n = P->m = 0;
  P->id = G.id + "_sub";
  vis_cnt = 0;
  memset(vis, 0, sizeof(vis));
  map<int, int> g2p;

  int u = getrand(G.n);
  vis[u] = true; vis_cnt++;
  g2p[u] = ++P->n; //printf("g2p %d -> %d\n", u, P->n);
  P->label[P->n] = G.label[u];
  psize--;
  if (!dfs(u, G, *P, psize, g2p)) {
    cerr << "generate_subgraph ERROR: failed\n";
    printf("P->n: %d  G.n: %d\n", P->n, G.n);
    // return make_pair(false, P);
  }
  return make_pair(true, P);
}

void load_data(string file_name, int data_num) {
  ifstream fin(file_name);
  for(int i = 1; i <= data_num; i++) {
    // printf("i %d\n", i);
    string s;
    int n, m;
    getline(fin, s);
    fin >> n >> m; //printf("nm %d %d\n", n, m);
    gs[i] = new Graph(n, m);
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

void print_graph(Graph &P, ofstream &fout) {
  fout << "Graph " << P.id << "\n";
  fout << P.n << " " << P.m << "\n";
  for(int i=1; i<=P.n; i++) fout << P.label[i] << " "; fout << "\n";
  for(int u=0; u<=P.n; u++) {
    for(int i=P.h[u]; i; i=P.e[i].ne) {
      int v = P.e[i].v;
      if(v >= u) fout << u << " " << v << "\n";
    }
  }
  fout << "\n";
}
void sample(int data_num, int sample_num, string file_name) {
  srand(317);
  vector<int> vec;
  for (int i=1; i<=data_num; i++) vec.push_back(i);
  // random_shuffle(vec.begin(), vec.end()); //NOTE: not shuffle here.
  vector<int> vm {4, 8, 12, 16, 20, 24};

  for (int m : vm) { printf("m %d\n", m);
    ofstream fout(file_name + to_string(m) + ".data");
    for (int i=0; i<sample_num; i++) { //printf("i %d\n", i);
      int x = vec[i];
      while (gs[x]->n < m) x = getrand(data_num);
      pair<bool, Graph*> pa = generate_subgraph(*gs[x], m); int c = 0;
      while (!pa.first) {
        printf("c: %d\n", ++c);
        x = getrand(data_num);
        pa = generate_subgraph(*gs[x], m);
      }
      Graph P = *pa.second;
      //printf("look %d\n", solve(P, *gs[x]));
      print_graph(P, fout);
    }
  }
}
int main() {
  int data_cnt = AIDS_TOTAL;
  load_data("dataset/aids/aids.data", data_cnt);
  sample(data_cnt, 1000, "dataset/aids/query");
}