#include <set>
#include <vector>

#include "../GraphDS.h"

const int maxn = 500;

struct Tarjan {
    int G[maxn][maxn] = {0}, low[maxn] = {0}, dfn[maxn] = {0},
        Stack[maxn] = {0}, S[maxn] = {0}, E[maxn] = {0}, n, e, cnt, Index, num;
    int link[maxn] = {0}, unicom[maxn] = {0};
    bool vis[maxn] = {0};
    void tarjan(int u) {
        low[u] = dfn[u] = ++cnt;
        vis[u] = true;
        Stack[++Index] = u;
        for (int i = 0; i < G[u].size(); i++) {
            int v = G[u][i];
            if (!dfn[v]) {
                tarjan(v);
                low[u] = min(low[u], low[v]);
            } else if (vis[v]) {
                low[u] = min(low[u], dfn[v]);
            }
        }
        if (dfn[u] == low[u]) {
            ++num;
            do {
                p[num].push_back(Stack[Index]);
                unicom[Stack[Index]] = num;
                vis[Stack[Index]] = false;
                Index--;
            } while (u != Stack[Index + 1]);
        }
    }
    void solve(std::vector<int> &ans) {
        for (int i = 1; i <= n; i++) {
            if (!dfn[i]) tarjan(i);
        }
        for (int i = 1; i <= e; i++) {
            if (unicom[S[i]] != unicom[E[i]]) link[unicom[E[i]]] = 1;
        }
        for (int i = 1; i <= num; i++) sort(p[i].begin(), p[i].end());
        for (int i = 1; i <= num; i++) {
            if (!link[i]) ans.push_back(p[i][0]);
        }
    }
};

void equivalent(TGraph::Graph &G, vector<int> &res) {
    std::vector<std::multiset<short>> neighs;
    Tarjan t;
    for (int i = 1; i <= G.n; i++) neighs.push_back(std::multiset<short>());
    for (int i = 1; i <= G.m; i++) neighs[G.e[i].v].insert(G.label[G.e[i].ne]);
    for (int i = 1; i <= G.n; i++)
        for (int j = i + 1; j <= G.n; j++)
            if (G.label[i] == G.label[j] && G.degree[i] == G.degree[j] &&
                neighs[i] == neighs[j])
                t.G[i][j] = t.G[j][i] = 1;
    t.solve(res);
}