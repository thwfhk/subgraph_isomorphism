#include "isomorphism.h
#include "../GraphDS.h"
#include "Graph.h"

using namespace UGraph;

bool solve(Graph &P, Graph &G) {
    UGraph::Graph aa = UGraph::Graph(P.n);
    for (int i = 0; i < P.n; i++)
        for (int j = 0; j < P.n; j++)
            if (P.g[i][j]) aa.insert(i, j);
    for (int i = 0; i < P.n; i++) aa.label[i] = P.label[i];
    UGraph::Graph bb = UGraph::Graph(G.n);
    for (int i = 0; i < G.n; i++)
        for (int j = 0; j < G.n; j++)
            if (G.g[i][j]) bb.insert(i, j);
    for (int i = 0; i < G.n; i++) bb.label[i] = G.label[i];
    Ullmann u = Ullmann(aa, bb);
    u.subisomorphism();
    return u.found;
}