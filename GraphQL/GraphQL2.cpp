#include <iostream>
#include <cstdio>
#include <algorithm>
#include <vector>
#include <set>
#include <map>
#include <queue>

#include "Dinic.cpp"
#include "../GraphDS.h"
using TGraph::Graph;
using TGraph::edge;

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

// current d level, max r level
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

struct qwq {
  int id, phisize, label;
  qwq() {}
  qwq(int a, int b, int c): id(a), phisize(b), label(c) {}
  bool operator < (const qwq &r) const {
    if (phisize == r.phisize) return label < r.label;
    else return phisize < r.phisize;
  }
};

// 找a和b中点能否有一个完美匹配，返回否一定是没有完美匹配
bool checkPerfectMatch(vector<int> &a, vector<int> &b, set<int> *Phi, Graph &P) {
  // printf("--- checkPerfectMatch ---\n"); print_vector(a); print_vector(b);
  if (b.size() < a.size()) return false;
  // set<int> set_b;
  // for (int v : b) set_b.insert(v);
  sort(b.begin(), b.end());
  vector<qwq> vec;
  for (int u : a) vec.push_back(qwq(u, Phi[u].size(), P.label[u]));
  sort(vec.begin(), vec.end());
  set<int> cur;
  for (int i = 1; i <= vec.size(); i++) {
    int u = vec[i-1].id;
    // sort(Phi[u].begin(), Phi[u].end());
    int bind = 0; // b index
    for (int x : Phi[u]) {
      while (bind < b.size()-1 && b[bind] < x) bind++;
      if (b[bind] == x) cur.insert(x);
    }
    // for (int x : Phi[u]) if (set_b.count(x)) cur.insert(x);
    if (cur.size() < i) return false;
  }
  return true;
}

bool checkPerfectMatch2(vector<int> &a, vector<int> &b, set<int> *Phi, Graph &P) {
  // printf("--- checkPerfectMatch ---\n"); print_vector(a); print_vector(b);
  if (b.size() < a.size()) return false;
  // set<int> set_b;
  // for (int v : b) set_b.insert(v);
  sort(b.begin(), b.end());
  vector<qwq> vec;
  set<int> bset;
  for (int x : b) bset.insert(x);
  set<int> PhiSetCap[MAXPN];  // cnt[u] 表示Phi[u]\cap b的个数
  for (int u : a) {
    for (int x : Phi[u]) if (bset.count(x)) PhiSetCap[u].insert(x);
  }
  for (int u : a) vec.push_back(qwq(u, PhiSetCap[u].size(), P.label[u]));
  sort(vec.begin(), vec.end());
  set<int> cur;
  for (int i = 1; i <= vec.size(); i++) {
    int u = vec[i-1].id;
    // sort(Phi[u].begin(), Phi[u].end());
    int bind = 0; // b index
    for (int x : PhiSetCap[u]) {
      cur.insert(x);
    }
    // for (int x : Phi[u]) if (set_b.count(x)) cur.insert(x);
    if (cur.size() < i) return false;
  }
  return true;
}

void optimize2xPhi(Graph &P, Graph &G, vector<int> *Phi, int l) {
  static bool inqt[MAXPN][MAXPN];
  memset(inqt, 0, sizeof(inqt));

  queue<pair<int, int> > q;
  for (int u = 1; u <= P.n; u++) {
    for (int v : Phi[u]) {
      q.push({u, v});
    }
  }

  set<int> *PhiSet = new set<int>[P.n+1];
  for (int u = 1; u <= P.n; u++) {
    for (int v : Phi[u]) PhiSet[u].insert(v);
  }

  for (int i = 1; i <= l; i++) {
    // qt is used to store the (u,v) pairs needed to be updated in the next level
    queue<pair<int, int> > qt;
    memset(inqt, 0, sizeof(inqt));
    while (!q.empty()) {
      pair<int, int> x = q.front(); q.pop();
      int u = x.first, v = x.second;
      if (inqt[u][v]) inqt[u][v] = false;
      if (PhiSet[u].count(v) == 0) continue;
      vector<int> neighbor_u; getNeighbor(P, 0, u, 1, neighbor_u, false);
      vector<int> neighbor_v; getNeighbor(G, 0, v, 1, neighbor_v, false);
      //printf("-----------------now(%d,%d)-----------------\n", u, v);
      //print_vector(neighbor_u);
      //print_vector(neighbor_v);

      bool pm = checkPerfectMatch(neighbor_u, neighbor_v, PhiSet, P); 
      // bool pm = true;
      if (!pm) {
        // printf("not match (%d, %d), Phi[u]: \n", u, v); print_vector(Phi[u]);
        PhiSet[u].erase(v);
        for (int ut : neighbor_u) 
          for (int vt : neighbor_v) {
            //printf("repeat %d %d\n", ut, vt);
            if (!inqt[ut][vt] && PhiSet[ut].count(vt)) {
              qt.push({ut, vt});
              inqt[ut][vt] = true;
            }
          }
      }
    }
    q = qt;
    //printf("qt %d  (%d,%d)\n", qt.size(), qt.front().first, qt.front().second);
  }
  for (int u = 1; u <= P.n; u++) {
    Phi[u].clear();
    for (int v : PhiSet[u]) Phi[u].push_back(v);
  }
}

bool check(int u, int v, vector<int> *Phi, int *phi, Graph &P, Graph &G) {
  for (int i = P.h[u]; i; i = P.e[i].ne) {
    int ut = P.e[i].v;
    if (make_pair(Phi[ut].size(), ut) > make_pair(Phi[u].size(), u)) continue; // NOTE: order |Phi|
    if (!G.g[v][phi[ut]]) return false;
  }
  return true;
}

// main search
bool dfs(vector<pair<int, int> > &li, int cur, int n, 
  vector<int> *Phi, bool *used, int *phi, Graph &P, Graph &G) {
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

  optimize2xPhi(P, G, Phi, 1); // 
  // puts("Optimization2x: "); print_Phi(P, Phi);

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