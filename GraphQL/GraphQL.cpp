#include <iostream>
#include <cstdio>
#include <algorithm>
#include <vector>
#include <set>
#include <map>
#include <queue>

#include "Dinic.cpp"
#include "../GraphDS.h"

using std::vector;

void print_vector(vector<int> v, char end = '\n') {
  for (int x : v) printf("%d ", x); printf("%c", end);
}

void print_Phi(Graph &P, vector<int> *Phi) {
  for (int i=1; i<=P.n; i++) {
    printf("Phi[%d]: ", i);
    for (int v : Phi[i]) printf(":%d ", v);
    puts("");
  }
}

void initialPhi(Graph &P, Graph &G, vector<int> *Phi) {
  for (int i = 1; i <= P.n; i++) {
    for (int j = 1; j <= G.n; j++) {
      if (P.label[i] == G.label[j]) Phi[i].push_back(j);
    }
  }
}

void getNeighbor(Graph &G, int d, int u, int r, vector<int> &neighbor, bool useLabel = true) {
  if (d != 0) {
    if (useLabel) neighbor.push_back(G.label[u]);
    else neighbor.push_back(u);
  }
  for (int i = G.h[u]; i; i = G.e[i].ne) {
    int v = G.e[i].v;
    if (d+1 <= r) getNeighbor(G, d+1, v, r, neighbor, useLabel);
  }
}

// O(n^2) dp
bool isSubSequence(vector<int> a, vector<int> b) {
  int n = a.size(), m = b.size();
  bool **f = new bool* [n] ();
  for (int i=0; i<n; i++) f[i] = new bool [m] ();

  for (int j=0; j<m; j++) f[0][j] = true;
  for (int i=1; i<n; i++) {
    for (int j=i; j<m; j++) {
      if (a[i] == b[j]) f[i][j] = f[i-1][j-1];
      else f[i][j] = f[i][j-1];
    }
  }
  bool ans = false;
  for (int j=n-1; j<m; j++) if (f[n-1][j]) {ans = true; break;}
  for (int i=0; i<n; i++) delete [] f[i];
  delete [] f;
  return ans;
}

void optimize1Phi(Graph &P, Graph &G, vector<int> *Phi, int r) {
  for (int u = 1; u <= P.n; u++) {
    vector<int> neighbor_u;
    vector<int> newPhi_u;
    getNeighbor(P, 0, u, r, neighbor_u);
    sort(neighbor_u.begin(), neighbor_u.end());
    //printf("u %d : ", u); print_vector(neighbor_u, '\n');
    for (int v : Phi[u]) {
      vector<int> neighbor_v;
      getNeighbor(G, 0, v, r, neighbor_v);
      sort(neighbor_v.begin(), neighbor_v.end());
      //printf("v %d : ", v); print_vector(neighbor_v, '\n');
      if (isSubSequence(neighbor_u, neighbor_v)) newPhi_u.push_back(v);
    }
    Phi[u] = newPhi_u;
  }
}

void optimize2Phi(Graph &P, Graph &G, vector<int> *Phi, int l) {
  static bool inqt[MAXPN][MAXPN];
  memset(inqt, 0, sizeof(inqt));

  queue<pair<int, int> > q;
  for (int u = 1; u <= P.n; u++)
    for (int v : Phi[u]) {
      q.push({u, v});
    }

  for (int i = 1; i <= l; i++) {
    // qt is used to store the (u,v) pairs needed to be updated in the next level
    queue<pair<int, int> > qt;
    memset(inqt, 0, sizeof(inqt));
    while (!q.empty()) {
      pair<int, int> x = q.front(); q.pop();
      int u = x.first, v = x.second;
      vector<int> neighbor_u; getNeighbor(P, 0, u, 1, neighbor_u, false);
      vector<int> neighbor_v; getNeighbor(G, 0, v, 1, neighbor_v, false);
      // printf("-----------------now(%d,%d)-----------------\n", u, v);
      //print_vector(neighbor_u);
      //print_vector(neighbor_v);

      // build bi-graph
      MF::init(P.n+G.n+1);
      for (int u : neighbor_u) 
        for (int v : neighbor_v) {
          if (find(Phi[u].begin(), Phi[u].end(), v) != Phi[u].end()) {
            MF::ins(u, P.n + v, 1);
          }
        }
      MF::s = 0; MF::t = P.n + G.n + 1;
      for (int u : neighbor_u) MF::ins(MF::s, u, 1);
      for (int v : neighbor_v) MF::ins(P.n + v, MF::t, 1);

      // if there's no perfect matching
      int flow = MF::dinic(), maximum = neighbor_u.size();
      
      // printf("flow %d %d\n", flow, maximum);
      if (flow < maximum) {
        //printf("not match (%d, %d)\n", u, v);
        Phi[u].erase(find(Phi[u].begin(), Phi[u].end(), v));
        for (int ut : neighbor_u) 
          for (int vt : neighbor_v) {
            //printf("repeat %d %d\n", ut, vt);
            if (!inqt[ut][vt] && find(Phi[ut].begin(), Phi[ut].end(), vt) != Phi[ut].end()) {
              qt.push({ut, vt});
              inqt[ut][vt] = true;
            }
          }
      }
    }
    q = qt;
    //printf("qt %d  (%d,%d)\n", qt.size(), qt.front().first, qt.front().second);
  }
}

bool check(int u, int v, vector<int> *Phi, int *phi, Graph &P, Graph &G) {
  for (int i = P.h[u]; i; i = P.e[i].ne) {
    int ut = P.e[i].v;
    if (Phi[ut].size() >= Phi[u].size()) continue; // NOTE: order |Phi|
    if (!G.g[v][phi[ut]]) return false;
  }
  return true;
}

// main search
bool dfs(vector<pair<int, int> > &li, int cur, int n, vector<int> *Phi, bool *used, int *phi, Graph &P, Graph &G) {
  int u = li[cur-1].second; // u in P
  for (int v : Phi[u]) if (!used[v]) {
    if (!check(u, v, Phi, phi, P, G)) continue;
    phi[u] = v;
    used[v] = true;
    if (cur < n) {
      if (dfs(li, cur+1, n, Phi, used, phi, P, G)) return true;
    }
    else return true;
    used[v] = false;
  }
  return false;
}

bool solve(Graph &P, Graph &G) {
  bool *used = new bool [G.n+1] ();
  vector<int> *Phi = new vector<int> [P.n+1];
  

  initialPhi(P, G, Phi);
  // puts("Initial: "); print_Phi(P, Phi);

  optimize1Phi(P, G, Phi, 1);
  // puts("Optimization1: "); print_Phi(P, Phi);

  optimize2Phi(P, G, Phi, 1);
  // puts("Optimization2: "); print_Phi(P, Phi);

  int *phi = new int [P.n+1];

  // optimize3: find a good order
  vector<pair<int, int> > li;
  for (int i=1; i <= P.n; i++) li.push_back({Phi[i].size(), i});
  sort(li.begin(), li.end()); //|Phi| in a increase order
  // printf("order: "); for (auto i : li) printf("(%d, %d) ", i.first, i.second); puts("");
  
  bool ans = dfs(li, 1, P.n, Phi, used, phi, P, G);

  delete [] Phi;
  delete [] used;
  return ans;
}

// int main() {
//   int n, m;
//   cin >> n >> m;
//   Graph P(n, m);
//   for (int i=1; i <= P.n; i++) {
//     int x;
//     scanf("%d", &x);
//     P.label[i] = x+'0';
//   }
//   for (int i=1; i <= P.m; i++) {
//     int u, v;
//     scanf("%d %d", &u, &v);
//     P.ins(u, v);
//   }
//   cin >> n >> m;
//   Graph G(n, m);
//   for (int i=1; i <= G.n; i++) {
//     int x;
//     scanf("%d", &x);
//     G.label[i] = x+'0';
//   }
//   for (int i=1; i <= G.m; i++) {
//     int u, v;
//     scanf("%d %d", &u, &v);
//     G.ins(u, v);
//   }
//   cout << solve(P, G);
// }