#include "Graph.h"
#include "util.h"
#include <algorithm>
#include <cassert>
#include <unordered_set>

extern std::random_device dev;
extern std::mt19937 rng;

namespace QuickSI {
Edge::Edge() {}
Edge::Edge(int from, int to, int weight) : from(from), to(to), weight(weight) {}

bool Edge::operator<(const Edge &rhs) const {
	return weight > rhs.weight;
}

bool Edge::operator==(const Edge &rhs) const {
	return from == rhs.from && to == rhs.to && weight == rhs.weight;
}

bool Edge::operator!=(const Edge &rhs) const {
	return !(*this == rhs);
}

Node::Node() {}
Node::Node(short label, int weight) : label(label), weight(weight), deg(0) {}

int BaseGraph::AddNode(short label, int weight) {
	int t = nodes.size();
	nodes[t] = Node(label, weight);
	return t;
}

void BaseGraph::AddEdge(int u, int v, int w) { // undirected edge
	Edge e;
	e = Edge(u, v, w), edges.push_back(e), nodes[u].adj.push_back(e);
	e = Edge(v, u, w), edges.push_back(e), nodes[v].adj.push_back(e);
	++nodes[u].deg, ++nodes[v].deg;
}

Edge Graph::FirstEdge() {
	int min_weight = INF, cnt = 0;
	Edge edge;
	for (const auto &e : edges) {
		if (e.weight < min_weight) {
			min_weight = e.weight;
			edge = e;
			cnt = 1;
		} else if (e.weight == min_weight) {
			++cnt;
		}
	}
	if (cnt != 1) {
		cnt = 0;
		int min_deg = INF;
		for (const auto &e : edges) {
			if (e.weight == min_weight) {
				if (nodes[e.from].deg + nodes[e.to].deg < min_deg) {
					min_deg = nodes[e.from].deg + nodes[e.to].deg;
					edge = e;
					cnt = 1;
				} else if (nodes[e.from].deg + nodes[e.to].deg == min_deg) {
					++cnt;
				}
			}
		}
		if (cnt != 1) {
			cnt = 0;
			for (const auto &e : edges) {
				if (e.weight == min_weight && nodes[e.from].deg + nodes[e.to].deg == min_deg) {
					if (std::uniform_int_distribution<std::mt19937::result_type>(0, cnt++)(rng) == 0) {
						edge = e;
					}
				}
			}
		}
	}
	if (nodes[edge.from].weight > nodes[edge.to].weight)
		edge = Edge(edge.to, edge.from, edge.weight);
	return edge;
}

int Graph::CalcInd(int u) {
	int cnt = 0;
	for (const auto &e : nodes[u].adj) {
		if (in_MST[e.to]) {
			++cnt;
		}
	}
	return cnt;
}

Edge Graph::SpanningEdge(std::priority_queue<Edge> &heap) {
	std::vector<std::pair<Edge, int>> tmp; // pair(edge, ind_size)
	Edge u = heap.top();
	heap.pop(), tmp.push_back(std::make_pair(u, 0));
	while (!heap.empty() && heap.top().weight == u.weight) {
		tmp.push_back(std::make_pair(heap.top(), 0));
		heap.pop();
	}
	if (tmp.size() == 1) return u;
	int max_ind = 0, cnt = 0;
	Edge edge;
	for (auto &e : tmp) {
		e.second = CalcInd(e.first.to);
		if (e.second > max_ind) {
			max_ind = e.second;
			edge = e.first;
			cnt = 1;
		} else if (e.second == max_ind) {
			++cnt;
		}
	}
	if (cnt != 1) {
		cnt = 0;
		int min_deg = INF;
		for (const auto &e : tmp) {
			if (e.second == max_ind) {
				if (nodes[e.first.to].deg < min_deg) {
					min_deg = nodes[e.first.to].deg;
					edge = e.first;
					cnt = 1;
				} else if (nodes[e.first.to].deg == min_deg) {
					++cnt;
				}
			}
		}
		if (cnt != 1) {
			cnt = 0;
			for (const auto &e : tmp) {
				if (e.second == max_ind && nodes[e.first.to].deg == min_deg) {
					if (std::uniform_int_distribution<std::mt19937::result_type>(0, cnt++)(rng) == 0) {
						edge = e.first;
					}
				}
			}
		}
	}
	for (const auto &e : tmp) {
		if (e.first != edge) {
			heap.push(e.first);
		}
	}
	return edge;
}

QISeqEntry Graph::MakeEntry(int u, int p) {
	QISeqEntry entry(u, p, nodes[u].label);
	if (nodes[u].deg > 2) entry.extra.push_back(std::make_pair(Tag::DEG, nodes[u].deg));
	return entry;
}

void Graph::GetQISeq() {
	QISeq.clear();
	in_MST.clear(), pos_in_QISeq.clear();
	std::priority_queue<Edge> heap;
	Edge edge = FirstEdge();
	QISeq.push_back(MakeEntry(edge.from, -1));
	QISeq.push_back(MakeEntry(edge.to, 0));
	in_MST[edge.from] = in_MST[edge.to] = 1;
	pos_in_QISeq[edge.from] = 0, pos_in_QISeq[edge.to] = 1;
	for (const auto &e : nodes[edge.from].adj) {
		if (e.to != edge.to) {
			heap.push(e);
		}
	}
	for (const auto &e : nodes[edge.to].adj) {
		if (e.to != edge.from) {
			heap.push(e);
		}
	}
	while (!heap.empty()) {
		if (in_MST[heap.top().to]) {
			heap.pop();
			continue;
		}
		Edge edge = SpanningEdge(heap);
		QISeqEntry entry = MakeEntry(edge.to, pos_in_QISeq[edge.from]);
		in_MST[edge.to] = 1;
		pos_in_QISeq[edge.to] = QISeq.size();
		std::vector<Edge> extra_edge;
		for (const auto &e : nodes[edge.to].adj) {
			if (!in_MST[e.to]) {
				heap.push(e);
			} else if (e.to != edge.from) {
				extra_edge.push_back(e);
			}
		}
		std::sort(extra_edge.begin(), extra_edge.end(), [](const Edge &a, const Edge &b) { return a.weight < b.weight; });
		for (const auto &e : extra_edge) {
			entry.extra.push_back(std::make_pair(EDGE, pos_in_QISeq[e.to]));
		}
		QISeq.push_back(entry);
	}
}

std::vector<std::size_t> seed_list;

Tree::Tree() { root1 = root2 = -1, has_hashed = 0; }

void Tree::get_center(int u, int fa) {
	size[u] = 1;
	weight[u] = 0;
	for (const auto &e : nodes[u].adj) {
		int v = e.to;
		if (v != fa) {
			get_center(v, u);
			size[u] += size[v];
			weight[u] = std::max(weight[u], size[v]);
		}
	}
	weight[u] = std::max(weight[u], (int)nodes.size() - size[u]);
	if (root1 == -1 || weight[u] < weight[root1])
		root1 = u, root2 = -1;
	else if (weight[u] == weight[root1])
		root2 = u;
}

std::size_t Tree::calc_hash(int u, int fa, int dep) {
	std::unordered_multiset<std::size_t> child_hash;
	for (const auto &e : nodes[u].adj) {
		int v = e.to;
		if (v != fa) {
			child_hash.insert(calc_hash(v, u, dep + 1));
		}
	}
	if (dep == seed_list.size()) seed_list.push_back(std::uniform_int_distribution<std::mt19937::result_type>()(rng));
	return std::hash<std::pair<std::size_t, std::size_t>>()(std::make_pair(std::hash<short>()(nodes[u].label),
																		   std::hash<std::unordered_multiset<std::size_t>>(seed_list[dep])(child_hash)));
}

std::size_t Tree::calc_hash() {
	if (has_hashed) return hash;
	if (root1 == -1) get_center((*nodes.begin()).first, -1);
	size_t value = calc_hash(root1, -1, 0);
	if (root2 == -1) {
		has_hashed = 1;
		return hash = value;
	} else {
		std::pair<std::size_t, std::size_t> hash_pair(value, calc_hash(root2, -1, 0));
		if (hash_pair.first > hash_pair.second) std::swap(hash_pair.first, hash_pair.second);
		has_hashed = 1;
		return hash = std::hash<std::pair<std::size_t, std::size_t>>()(hash_pair);
	}
}

std::size_t Tree::get_hash() const {
	assert(has_hashed);
	return hash;
}
} // namespace QuickSI