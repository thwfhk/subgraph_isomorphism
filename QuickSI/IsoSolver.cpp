#include "IsoSolver.h"

bool IsoSolver::extra_satisfied(const QISeqEntry &T, const Node &v) {
	for (const auto &restriction : T.extra) {
		if (restriction.first == DEG) {
			if (restriction.second > v.deg) return 0;
		} else if (restriction.first == EDGE) {
			if (!connected(v, iso[restriction.second])) return 0;
		}
	}
	return 1;
}

bool IsoSolver::dfs(int dep) {
	if (dep >= size) return 1;
	QISeqEntry T = QISeq[dep];
	if (!dep) {
		for (const auto &v : G.nodes) {
			if (v.second.label == T.label && !used[v.first]) {
				if (extra_satisfied(T, v.second)) {
					used[v.first] = 1;
					iso[dep] = v.first;
					if (dfs(dep + 1)) return 1;
					used[v.first] = 0;
				}
			}
		}
	}
	else {
		auto it = G.nodes.find(iso[T.parent]);
		if (it != G.nodes.end()) {
			for (const auto &e : it->second.adj) {
				int v = e.to;
				auto itv = G.nodes.find(v);
				auto node = itv->second;
				if (node.label == T.label && !used[v]) {
					if (extra_satisfied(T, node)) {
						used[v] = 1;
						iso[dep] = v;
						if (dfs(dep + 1)) return 1;
						used[v] = 0;
					}
				}
			}
		}
	}
	return 0;
}

IsoSolver::IsoSolver(const Graph &query, const Graph &G) : QISeq(query.QISeq), G(G) {
	size = QISeq.size();
}

bool IsoSolver::QuickSI() {
	iso.clear(), used.clear();
	return dfs(0);
}
