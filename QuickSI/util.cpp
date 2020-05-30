#include "util.h"
using namespace QuickSI;

namespace std {

size_t hash<pair<Tag, int>>::operator()(const pair<Tag, int> &key) const {
	return hash<int>()((key.second << 1) | key.first);
}

template <class T>
size_t hash<pair<T, T>>::operator()(const pair<T, T> &key) const {
	size_t value = 0;
	hash<T> hasher;
	value ^= hasher(key.first) + 0x9e3779b9 + (value << 6) + (value >> 2);
	value ^= hasher(key.second) + 0x9e3779b9 + (value << 6) + (value >> 2);
	return value;
}

template <class T>
hash<vector<T>>::hash(size_t seed) : seed(seed) {}

template <class T>
size_t hash<vector<T>>::operator()(const vector<T> &key) const {
	size_t value = seed;
	hash<T> hasher;
	for (const auto &x : key) {
		value ^= hasher(x) + 0x9e3779b9 + (value << 6) + (value >> 2);
	}
	return value;
}

template <class T>
hash<unordered_multiset<T>>::hash(size_t seed) : seed(seed) {}

template <class T>
size_t hash<unordered_multiset<T>>::operator()(const unordered_multiset<T> &key) const {
	size_t value = seed;
	hash<T> hasher;
	for (const auto &x : key) {
		value ^= hasher(x) + 0x9e3779b9 + (value << 6) + (value >> 2);
	}
	return value;
}

size_t hash<QISeqEntry>::operator()(const QISeqEntry &key) const {
	return hash<int>()(key.parent) ^ hash<short>()(key.label) ^ hash<vector<pair<Tag, int>>>(key.extra.size())(key.extra);
}

size_t hash<Tree>::operator()(const Tree &key) const {
	return key.get_hash();
}

template class hash<pair<size_t, size_t>>;
template class hash<pair<short, short>>;
template class hash<unordered_multiset<size_t>>;
template class hash<Tree>;

} // namespace std

bool connected(const Node &v, int u) {
	return find_if(v.adj, [&u](const Edge &e) { return e.to == u; });
}

int getfa(std::vector<int> &fa, int x) { return x == fa[x] ? x : fa[x] = getfa(fa, fa[x]); }
