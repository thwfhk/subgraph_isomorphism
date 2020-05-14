#include "Preprocessor.h"
#include "IsoSolver.h"

void Preprocessor::update_candidates(Trie<QISeqEntry, std::set<int>>::Node *u) {
	if (init)
		candidates = prefix_tree.inv_list[u], init = 0;
	else {
		std::set<int> new_cons = prefix_tree.inv_list[u];
		std::set<int> intersection;
		std::set_intersection(candidates.begin(), candidates.end(),
							  new_cons.begin(), new_cons.end(),
							  std::inserter(intersection, intersection.end()));
		candidates.swap(intersection);
	}
}

bool Preprocessor::extra_satisfied(const QISeqEntry &T, const Node &v) {
	for (const auto &restriction : T.extra) {
		if (restriction.first == DEG) {
			if (restriction.second != v.deg) return 0;
		} else if (restriction.first == EDGE) {
			if (!connected(v, iso[restriction.second])) return 0;
		}
	}
	return 1;
}

void Preprocessor::dfs(Trie<QISeqEntry, std::set<int>>::Node *u, const Graph &q, int dep) {
	QISeqEntry T = u->entry;
	for (const auto &v : q.nodes) {
		if (v.second.label == T.label && !used[v.first] && (!dep || connected(v.second, iso[T.parent]))) {
			if (extra_satisfied(T, v.second)) {
				if (u->is_end) update_candidates(u);
				if (u->child.empty()) {
					vis[u] = 1;
					return;
				}
				used[v.first] = 1;
				iso[dep] = v.first;
				for (const auto &c : u->child) {
					if (!vis[c.second]) dfs(c.second, q, dep + 1);
				}
				bool flag = 1;
				for (const auto &c : u->child) {
					if (!vis[c.second]) {
						flag = 0;
						break;
					}
				}
				if (flag) {
					vis[u] = 1;
					return;
				}
				used[v.first] = 0;
			}
		}
	}
}

extern std::vector<Graph> Database;
extern std::unordered_map<short, int> label_weights;
extern std::unordered_map<std::pair<short, short>, int> edge_weights;

Preprocessor::Preprocessor(double freq_thres, double disc_thres, int max_feature_size) {
	std::unordered_set<Tree> features;

	for (const auto &i : label_weights) {
		short l = i.first;
		Tree T;
		T.AddNode(l, i.second);
		T.GetQISeq();
		std::set<int> inv_list;
		for (int i = 0, ___ = Database.size(); i < ___; i++) {
			IsoSolver solver(T, Database[i]);
			if (solver.QuickSI()) inv_list.insert(i);
		}
		prefix_tree.add(T.QISeq, inv_list);
	}

	for (const auto &i : edge_weights) {
		Tree T;
		T.AddEdge(T.AddNode(i.first.first, label_weights[i.first.first]), T.AddNode(i.first.second, label_weights[i.first.second]), i.second);
		T.GetQISeq();
		T.calc_hash();
		features.insert(T);
		std::set<int> inv_list;
		for (int i = 0, ___ = Database.size(); i < ___; i++) {
			IsoSolver solver(T, Database[i]);
			if (solver.QuickSI()) inv_list.insert(i);
		}
		prefix_tree.add(T.QISeq, inv_list);
	}

	for (int i = 3; i <= max_feature_size; i++) {
		std::unordered_set<Tree> new_features;
		for (const auto &T : features) {
			for (const auto &v : T.nodes) {
				for (const auto &new_v : label_weights) {
					Tree new_T = T;
					short lu = T.nodes.find(v.first)->second.label;
					short lv = new_v.first;
					if (lu > lv) std::swap(lu, lv);
					new_T.AddEdge(v.first, new_T.AddNode(new_v.first, new_v.second), edge_weights[std::make_pair(lu, lv)]);
					new_T.GetQISeq();
					new_T.calc_hash();
					new_features.insert(new_T);
					std::set<int> inv_list;
					Filtering(new_T);
					for (int idx : candidates) {
						IsoSolver solver(new_T, Database[idx]);
						if (solver.QuickSI()) inv_list.insert(i); 
					}
					if (inv_list.size() * 1.0 / Database.size() >= freq_thres && inv_list.size() * 1.0 / candidates.size() < 1 - disc_thres) {
						prefix_tree.add(new_T.QISeq, inv_list);
					}
				}
			}
		}
		features = new_features;
	}
}

void Preprocessor::Filtering(const Graph &q) {
	candidates.clear();
	iso.clear(), used.clear(), vis.clear();
	init = 1;
	for (const auto &c : prefix_tree.root->child) {
		dfs(c.second, q, 0);
	}
}
