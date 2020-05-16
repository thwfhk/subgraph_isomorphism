#include "isomorphism.h"
#include "../GraphDS.h"
#include "Graph.h"
#include "Graph.cpp"
#include "isomorphism.cpp"

// using namespace UGraph;

bool solve(Graph &P, Graph &G) {
    UGraph aa = UGraph(P.n);
    for (int i = 0; i < P.n; i++)
        for (int j = 0; j < P.n; j++)
            if (P.g[i+1][j+1]) aa.insert(i, j);
    for (int i = 0; i < P.n; i++) aa.label[i] = P.label[i+1];
    UGraph bb = UGraph(G.n);
    for (int i = 0; i < G.n; i++)
        for (int j = 0; j < G.n; j++)
            if (G.g[i+1][j+1]) bb.insert(i, j);
    for (int i = 0; i < G.n; i++) bb.label[i] = G.label[i+1];
    Ullmann u = Ullmann(aa, bb);
    u.subisomorphism();
    return u.found;
}