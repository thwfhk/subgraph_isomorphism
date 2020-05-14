#ifndef TRIE
#define TRIE

#include <unordered_map>
#include <vector>

template <class NodeType, class EndType>
class Trie {
	class Node {
		NodeType entry;
		std::unordered_map<NodeType, Node *> child;
		bool is_end;

		friend class Preprocessor;
	};

	Node *root;
	std::unordered_map<Node *, EndType> inv_list;

public:
	Trie();

	void add(const std::vector<NodeType> &str, const EndType &end);

	friend class Preprocessor;
};

#include "Trie.cpp"

#endif