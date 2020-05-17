#include "../GraphDS.h"
#include "Graph.cpp"
#include "Graph.h"
#include "VF2.cpp"
#include "VF2.h"

bool solve(TGraph::Graph &P, TGraph::Graph &G)
{
    UGraph aa = UGraph();
    for (int i = 0; i < P.n; i++)
        aa.addv(i, P.label[i + 1]);
    for (int i = 0; i < P.n; i++)
        for (int j = i + 1; j < P.n; j++)
            if (P.g[i + 1][j + 1])
                aa.adde(i, j, 0);
    UGraph bb = UGraph();
    for (int i = 0; i < G.n; i++)
        bb.addv(i, G.label[i + 1]);
    for (int i = 0; i < G.n; i++)
        for (int j = i + 1; j < G.n; j++)
            if (G.g[i + 1][j + 1])
                bb.adde(i, j, 0);
    VF2 v = VF2();
    bool found = v.vf2(aa, bb);
    return found;
}