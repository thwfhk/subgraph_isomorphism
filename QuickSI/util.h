#ifndef UTIL
#define UTIL

#include "Graph.h"
#include "QISeqEntry.h"
#include <algorithm>
#include <iostream>
#include <unordered_set>
using namespace QuickSI;

namespace std {

template <>
class hash<pair<Tag, int>> {
public:
	size_t operator()(const pair<Tag, int> &key) const;
};

template <class T>
class hash<pair<T, T>> {
public:
	size_t operator()(const pair<T, T> &key) const;
};

template <class T>
class hash<vector<T>> {
	const size_t seed;

public:
	hash(size_t seed);

	size_t operator()(const vector<T> &key) const;
};

template <class T>
class hash<unordered_multiset<T>> {
	const size_t seed;

public:
	hash(size_t seed);

	size_t operator()(const unordered_multiset<T> &key) const;
};

template <>
class hash<QISeqEntry> {
public:
	size_t operator()(const QISeqEntry &key) const;
};

template <>
class hash<Tree> {
public:
	size_t operator()(const Tree &key) const;
};

} // namespace std

template <class ContainerType, class Pred>
bool find_if(const ContainerType &v, Pred f);

bool connected(const Node &v, int u);

#endif