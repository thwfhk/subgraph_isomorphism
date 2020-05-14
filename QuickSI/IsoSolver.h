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
	const Graph &G;

	bool extra_satisfied(const QISeqEntry &T, const Node &v);
	bool dfs(int dep);

public:
	IsoSolver(const Graph &query, const Graph &G);

	bool QuickSI();
};

#endif