#include "Trie.h"

template <class NodeType, class EndType>
Trie<NodeType, EndType>::Trie() { root = new Node(); }

template <class NodeType, class EndType>
void Trie<NodeType, EndType>::add(const std::vector<NodeType> &str, const EndType &end) {
	Node *t = root;
	for (const auto &c : str) {
		if (!t->child[c]) t->child[c] = new Node();
		t = t->child[c];
	}
	t->is_end = 1;
	inv_list[t] = end;
}
