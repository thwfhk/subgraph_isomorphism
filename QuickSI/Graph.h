#ifndef GRAPH
#define GRAPH

#include "QISeqEntry.h"
#include <list>
#include <queue>
#include <random>
#include <unordered_map>

namespace QuickSI {
class Edge {
public:
	int from, to;
	int weight;

	Edge();
	Edge(int from, int to, int weight);

	bool operator<(const Edge &rhs) const;
	bool operator==(const Edge &rhs) const;
	bool operator!=(const Edge &rhs) const;
};

class Node {
public:
	int deg;
	int weight;
	short label;
	std::list<Edge> adj;

	Node();
	Node(short label, int weight);
};

class BaseGraph {
public:
	std::unordered_map<int, Node> nodes;
	std::list<Edge> edges;

	int AddNode(short label, int weight);
	void AddEdge(int u, int v, int w);
};

const int INF = 0x7fffffff;

class Graph : public BaseGraph { // Graph with QISeq
	std::unordered_map<int, bool> in_MST;
	std::unordered_map<int, int> pos_in_QISeq;

	Edge FirstEdge();
	int CalcInd(int u);
	Edge SpanningEdge(std::priority_queue<Edge> &heap);
	QISeqEntry MakeEntry(int u, int p);

public:
	std::vector<QISeqEntry> QISeq;
	void GetQISeq();

	friend class Preprocessor;
	friend class IsoSolver;
};

class Tree : public Graph { // Tree with hash
	std::unordered_map<int, int> size;
	std::unordered_map<int, int> weight;
	std::size_t hash;
	int root1, root2;
	bool has_hashed;

public:
	Tree();

	void get_center(int u, int fa);
	std::size_t calc_hash(int u, int fa, int dep);
	std::size_t calc_hash();
	std::size_t get_hash() const;
};
} // namespace QuickSI
#endif