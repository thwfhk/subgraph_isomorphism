#include <iostream>
#include <cstdio>
#include <algorithm>
#include <vector>
#include <set>
#include <map>
#include <queue>
#include "GraphQL.h"
using namespace std;

namespace Dinic {
  int s, t;
  struct edge {int v, ne, c, f;} e[MAX_NODE * MAX_NODE];
  int cnt=1, h[MAX_NODE];
  void ins(int u, int v, int c) { 
    //printf("ins %d %d %d\n", u, v, c);
    e[++cnt] = (edge) {v, h[u], c, 0}; h[u] = cnt;
    e[++cnt] = (edge) {u, h[v], 0, 0}; h[v] = cnt;
  }
  int cur[MAX_NODE], vis[MAX_NODE], d[MAX_NODE], head, tail, q[MAX_NODE];
  bool bfs() {
    memset(vis, 0, sizeof(vis));
    head = tail = 1;
    q[tail++] = s; d[s] = 0; vis[s] = 1;
    while(head != tail) {
      int u = q[head++];
      for(int i=h[u]; i; i=e[i].ne) {
        int v = e[i].v;
        if(!vis[v] && e[i].c > e[i].f) {
          vis[v] = 1;
          d[v] = d[u] + 1;
          q[tail++] = v;
          if(v == t) return true;
        }
      }
    }
    return false;
  }
  int dfs(int u, int a) {
    if(u==t || a==0) return a;
    int flow = 0, f;
    for(int &i=cur[u]; i; i=e[i].ne) {
      int v = e[i].v;
      if(d[v] == d[u]+1 && (f = dfs(v, min(a, e[i].c-e[i].f))) > 0) {
        flow += f;
        e[i].f += f;
        e[i^1].f -= f;
        a -= f;
        if(a==0) break;
      }
    }
    if(a) d[u] = -1;
    return flow;
  }

  void init() {
    cnt = 1;
    memset(h, 0, sizeof(h));
  }

  int dinic() { 
    int flow = 0;
    while(bfs()) {
      for(int i=s; i<=t; i++) cur[i] = h[i];
      flow += dfs(s, 1e9);
    }
    return flow;
  }
}