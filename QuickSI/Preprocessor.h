#ifndef PREPROCESSOR
#define PREPROCESSOR

#include "Graph.h"
#include "QISeqEntry.h"
#include "Trie.h"
#include "util.h"
#include <algorithm>
#include <set>

class Preprocessor {
	Trie<QISeqEntry, std::set<int>> prefix_tree;
	bool init;
	std::unordered_map<int, int> iso;
	std::unordered_map<int, bool> used;
	std::unordered_map<Trie<QISeqEntry, std::set<int>>::Node *, bool> vis;

	void update_candidates(Trie<QISeqEntry, std::set<int>>::Node *u);
	bool extra_satisfied(const QISeqEntry &T, const Node &v);
	void dfs(Trie<QISeqEntry, std::set<int>>::Node *u, const Graph &q, int dep);

public:
	std::set<int> candidates;
	
	Preprocessor(double freq_thres, double disc_thres, int max_feature_size);

	void Filtering(const Graph &q);
};

#endif