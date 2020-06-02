#ifndef ISOSOLVER
#define ISOSOLVER

#include "Graph.h"
#include "QISeqEntry.h"
#include "util.h"

class IsoSolver {
	std::unordered_map<int, int> iso;
	std::unordered_map<int, bool> used;
	int size;
	const std::vector<QISeqEntry> &QISeq;
	Graph &G;

	bool extra_satisfied(const QISeqEntry &T, const Node &v);
	bool dfs(int dep, bool use_phi, std::vector<int> &fa);

public:
	IsoSolver(const Graph &query, Graph &G);

	bool QuickSI(bool use_phi, std::vector<int> &fa);
};

#endif