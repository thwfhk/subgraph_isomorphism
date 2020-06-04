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

const int __V = 2000;
extern bool phi[__V][__V];

bool IsoSolver::dfs(int dep, bool use_phi) {
	if (dep >= size) return 1;
	QISeqEntry T = QISeq[dep];
	if (!dep) {
		if (use_phi) {
			for (const auto &v : G.nodes) {
				if (phi[T.id][v.first] && !used[v.first]) {
					if (extra_satisfied(T, v.second)) {
						used[v.first] = 1;
						iso[dep] = v.first;
						// printf("choose %d, dep %d\n", v.first, dep);
						if (dfs(dep + 1, use_phi)) return 1;
						used[v.first] = 0;
					}
				}
			}
		} else {
			for (const auto &v : G.nodes) {
				if (v.second.label == T.label && !used[v.first]) {
					if (extra_satisfied(T, v.second)) {
						used[v.first] = 1;
						iso[dep] = v.first;
						if (dfs(dep + 1, use_phi)) return 1;
						used[v.first] = 0;
					}
				}
			}
		}
	} else {
		if (use_phi) {
			auto it = G.nodes.find(iso[T.parent]);
			if (it != G.nodes.end()) {
				for (const auto &e : it->second.adj) {
					int v = e.to;
					auto itv = G.nodes.find(v);
					auto node = itv->second;
					if (phi[T.id][v] && !used[v]) {
						if (extra_satisfied(T, node)) {
							used[v] = 1;
							iso[dep] = v;
							// if (dep <= 5) printf("choose %d, dep %d\n", v, dep);
							if (dfs(dep + 1, use_phi)) return 1;
							used[v] = 0;
						}
					}
				}
			}
		} else {
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
							if (dfs(dep + 1, use_phi)) return 1;
							used[v] = 0;
						}
					}
				}
			}
		}
	}
	return 0;
}

bool IsoSolver::dfs(int dep, bool use_phi, std::vector<int> &fa) {
	if (dep >= size) return 1;
	QISeqEntry T = QISeq[dep];
	if (!dep) {
		if (use_phi) {
			for (int i = 0; i < G.nodes.size(); i++) {
				if (phi[T.id][i] && !used[i]) {
					if (extra_satisfied(T, G.nodes[i])) {
						used[i] = 1;
						iso[dep] = i;
						// printf("choose %d, dep %d\n", i, dep);
						if (dfs(dep + 1, use_phi, fa)) return 1;
						used[i] = 0;
						for (int j = i + 1; j < G.nodes.size(); j++) {
							if (phi[T.id][j] && getfa(fa, i) == getfa(fa, j)) {
								phi[T.id][j] = 0;
							}
						}
					}
				}
			}
		} else {
			for (int i = 0; i < G.nodes.size(); i++) {
				auto v = G.nodes[i];
				if (v.label == T.label && !used[i]) {
					if (extra_satisfied(T, v)) {
						used[i] = 1;
						iso[dep] = i;
						if (dfs(dep + 1, use_phi, fa)) return 1;
						used[i] = 0;
					}
				}
			}
		}
	} else {
		if (use_phi) {
			auto it = G.nodes.find(iso[T.parent]);
			if (it != G.nodes.end()) {
				for (int i = 0; i < it->second.adj.size(); i++) {
					auto e = it->second.adj[i];
					int v = e.to;
					auto itv = G.nodes.find(v);
					auto node = itv->second;
					if (phi[T.id][v] && !used[v]) {
						if (extra_satisfied(T, node)) {
							used[v] = 1;
							iso[dep] = v;
							// if (dep <= 5) printf("choose %d, dep %d\n", v, dep);
							if (dfs(dep + 1, use_phi, fa)) return 1;
							used[v] = 0;
							for (int j = i + 1; j < it->second.adj.size(); j++) {
								int v = it->second.adj[j].to;
								if (phi[T.id][v] && getfa(fa, i) == getfa(fa, v)) {
									phi[T.id][v] = 0;
								}
							}
						}
					}
				}
			}
		} else {
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
							if (dfs(dep + 1, use_phi, fa)) return 1;
							used[v] = 0;
						}
					}
				}
			}
		}
	}
	return 0;
}

IsoSolver::IsoSolver(const Graph &query, Graph &G) : QISeq(query.QISeq), G(G) {
	size = QISeq.size();
}

bool IsoSolver::QuickSI(bool use_phi) {
	iso.clear(), used.clear();
	// for (int i = 0; i < QISeq.size(); i++) { printf("%d: ", i);
	// 	for (int j = 0; j < G.nodes.size(); j++) {
	// 		if (phi[i][j]) printf("%d ", j);
	// 	}puts("");
	// }
	return dfs(0, use_phi);
}

bool IsoSolver::QuickSI(bool use_phi, std::vector<int> &fa) {
	iso.clear(), used.clear();
	// for (int i = 0; i < QISeq.size(); i++) { printf("%d: ", i);
	// 	for (int j = 0; j < G.nodes.size(); j++) {
	// 		if (phi[i][j]) printf("%d ", j);
	// 	}puts("");
	// }
	return dfs(0, use_phi, fa);
}
