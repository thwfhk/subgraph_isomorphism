#include "../GraphDS.h"
#include "Graph.cpp"
#include "Graph.h"
#include "isomorphism.cpp"
#include "isomorphism.h"

bool solve(Graph &P, Graph &G)
{
    UGraph aa = UGraph(P.n);
    for (int i = 0; i < P.n; i++)
        for (int j = i + 1; j < P.n; j++)
            if (P.g[i + 1][j + 1])
                aa.insert(i, j);
    for (int i = 0; i < P.n; i++)
        aa.label[i] = P.label[i + 1];
    UGraph bb = UGraph(G.n);
    for (int i = 0; i < G.n; i++)
        for (int j = i + 1; j < G.n; j++)
            if (G.g[i + 1][j + 1])
                bb.insert(i, j);
    for (int i = 0; i < G.n; i++)
        bb.label[i] = G.label[i + 1];
    Ullmann u = Ullmann(aa, bb);
    u.subisomorphism();
    return u.found;
}