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
	bool dfs(int dep, std::vector<int> *Phi, std::vector<int> *Eq);

public:
	IsoSolver(const Graph &query, Graph &G);

	bool QuickSI(std::vector<int> *Phi = nullptr, std::vector<int> *Eq = nullptr);
};

#endif