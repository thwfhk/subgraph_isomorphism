#define QUICKSI

#include "../GraphDS.h"
#include "IsoSolver.h"
// #include "Preprocessor.h"
#include "../GraphQL/Dinic.cpp"
#include <cstring>
#include <set>
std::random_device dev;
std::mt19937 rng;

// std::vector<QuickSI::Graph> Database;
std::unordered_map<short, int> label_weights;
std::unordered_map<std::pair<short, short>, int> edge_weights;

// void calc_weight() {
// 	for (const auto &G : Database) {
// 		for (const auto &v : G.nodes) {
// 			label_weights[v.second.label]++;
// 		}
// 		for (const auto &e : G.edges) {
// 			short u = G.nodes.find(e.from)->second.label;
// 			short v = G.nodes.find(e.to)->second.label;
// 			if (u > v) std::swap(u, v);
// 			edge_weights[std::make_pair(u, v)]++;
// 		}
// 	}
// 	for (auto &G : Database) {
// 		for (auto &v : G.nodes) {
// 			v.second.weight = label_weights[v.second.label];
// 		}
// 		for (auto &e : G.edges) {
// 			short u = G.nodes[e.from].label;
// 			short v = G.nodes[e.to].label;
// 			if (u > v) std::swap(u, v);
// 			e.weight = edge_weights[std::make_pair(u, v)];
// 		}
// 	}
// }

const int __N = 42687 + 5;
const int __V = 2000;

std::vector<int> fa[__N];
bool phi[__V][__V];

void initialize(int data_num, TGraph::Graph *gs[__N]) {
	rng = std::mt19937(dev());

	label_weights.clear();
	edge_weights.clear();
	for (int g_i = 1; g_i <= data_num; g_i++) {
		// printf("%d\n", g_i);
		for (int v = 1; v <= gs[g_i]->n; v++) {
			short lv = gs[g_i]->label[v];
			label_weights[lv]++;
			for (int e = gs[g_i]->h[v]; e; e = gs[g_i]->e[e].ne) {
				int u = gs[g_i]->e[e].v;
				short lu = gs[g_i]->label[u];
				if (lv < lu) edge_weights[std::make_pair(lv, lu)]++;
			}
		}
		fa[g_i].resize(gs[g_i]->n + 2);
		for (int i = 1; i <= gs[g_i]->n; i++)
			fa[g_i][i] = i;
		for (int i = 1; i <= gs[g_i]->n; i++) {
			for (int j = i + 1; j <= gs[g_i]->n; j++) {
				if (getfa(fa[g_i], i) != getfa(fa[g_i], j)) {
					std::set<int> neighbor_i, neighbor_j;
					for (int e = gs[g_i]->h[i]; e; e = gs[g_i]->e[e].ne) {
						neighbor_i.insert(gs[g_i]->e[e].v);
					}
					for (int e = gs[g_i]->h[j]; e; e = gs[g_i]->e[e].ne) {
						neighbor_j.insert(gs[g_i]->e[e].v);
					}
					if (neighbor_i == neighbor_j) {
						fa[g_i][getfa(fa[g_i], i)] = getfa(fa[g_i], j);
					}
				}
			}
		}
		for (int i = 1; i <= gs[g_i]->n; i++)
			fa[g_i][i - 1] = fa[g_i][i] - 1;
	}
	// puts("initialize finished");
	// exit(233);
}

QuickSI::Graph convert(TGraph::Graph &G) {
	QuickSI::Graph Q;
	for (int v = 1; v <= G.n; v++) {
		short lv = G.label[v];
		Q.AddNode(lv, label_weights[lv]);
		for (int e = G.h[v]; e; e = G.e[e].ne) {
			int u = G.e[e].v;
			short lu = G.label[u];
			if (u < v) Q.AddEdge(u - 1, v - 1, edge_weights[std::make_pair(lv, lu)]);
		}
	}
	for (auto &v : Q.nodes) {
		std::sort(v.second.adj.begin(), v.second.adj.end(), [](const Edge &a, const Edge &b) { return a.to < b.to; });
	}
	return Q;
}

void initialPhi(TGraph::Graph &P, TGraph::Graph &G, std::vector<int> *Phi) {
	for (int i = 1; i <= P.n; i++) {
		for (int j = 1; j <= G.n; j++) {
			if (P.label[i] == G.label[j]) Phi[i].push_back(j);
		}
	}
}

void getNeighbor(TGraph::Graph &G, int d, int u, int r, std::vector<int> &neighbor, bool useLabel = true) {
	if (d != 0) {
		if (useLabel)
			neighbor.push_back(G.label[u]);
		else
			neighbor.push_back(u);
	}
	for (int i = G.h[u]; i; i = G.e[i].ne) {
		int v = G.e[i].v;
		if (d + 1 <= r) getNeighbor(G, d + 1, v, r, neighbor, useLabel);
	}
}

// O(n^2) dp
bool isSubSequence(std::vector<int> a, std::vector<int> b) {
	int n = a.size(), m = b.size();
	bool **f = new bool *[n]();
	for (int i = 0; i < n; i++)
		f[i] = new bool[m]();

	for (int j = 0; j < m; j++)
		f[0][j] = true;
	for (int i = 1; i < n; i++) {
		for (int j = i; j < m; j++) {
			if (a[i] == b[j])
				f[i][j] = f[i - 1][j - 1];
			else
				f[i][j] = f[i][j - 1];
		}
	}
	bool ans = false;
	for (int j = n - 1; j < m; j++)
		if (f[n - 1][j]) {
			ans = true;
			break;
		}
	for (int i = 0; i < n; i++)
		delete[] f[i];
	delete[] f;
	return ans;
}

void optimize1Phi(TGraph::Graph &P, TGraph::Graph &G, std::vector<int> *Phi, int r) {
	for (int u = 1; u <= P.n; u++) {
		std::vector<int> neighbor_u;
		std::vector<int> newPhi_u;
		getNeighbor(P, 0, u, r, neighbor_u);
		std::sort(neighbor_u.begin(), neighbor_u.end());
		//printf("u %d : ", u); print_vector(neighbor_u, '\n');
		for (int v : Phi[u]) {
			std::vector<int> neighbor_v;
			getNeighbor(G, 0, v, r, neighbor_v);
			std::sort(neighbor_v.begin(), neighbor_v.end());
			//printf("v %d : ", v); print_vector(neighbor_v, '\n');
			if (isSubSequence(neighbor_u, neighbor_v)) newPhi_u.push_back(v);
		}
		Phi[u] = newPhi_u;
	}
}

void optimize2Phi(TGraph::Graph &P, TGraph::Graph &G, std::vector<int> *Phi, int l) {
	static bool inqt[MAXPN][MAXPN];
	std::memset(inqt, 0, sizeof(inqt));

	queue<pair<int, int>> q;
	for (int u = 1; u <= P.n; u++)
		for (int v : Phi[u]) {
			q.push({u, v});
		}

	set<int> *PhiSet = new set<int>[P.n + 1];
	for (int u = 1; u <= P.n; u++) {
		for (int v : Phi[u])
			PhiSet[u].insert(v);
	}
	for (int i = 1; i <= l; i++) {
		// qt is used to store the (u,v) pairs needed to be updated in the next level
		queue<pair<int, int>> qt;
		memset(inqt, 0, sizeof(inqt));
		while (!q.empty()) {
			pair<int, int> x = q.front();
			q.pop();
			int u = x.first, v = x.second;
			if (inqt[u][v]) inqt[u][v] = false;
			if (PhiSet[u].count(v) == 0) continue;
			vector<int> neighbor_u;
			getNeighbor(P, 0, u, 1, neighbor_u, false);
			vector<int> neighbor_v;
			getNeighbor(G, 0, v, 1, neighbor_v, false);
			//printf("-----------------now(%d,%d)-----------------\n", u, v);
			//print_vector(neighbor_u);
			//print_vector(neighbor_v);

			// build bi-graph
			MF::init(P.n + G.n + 1);
			MF::s = 0;
			MF::t = P.n + G.n + 1;
			for (int u : neighbor_u)
				for (int v : neighbor_v) {
					// if (find(Phi[u].begin(), Phi[u].end(), v) != Phi[u].end()) {
					if (PhiSet[u].count(v)) {
						MF::ins(u, P.n + v, 1);
					}
				}
			for (int u : neighbor_u)
				MF::ins(MF::s, u, 1);
			for (int v : neighbor_v)
				MF::ins(P.n + v, MF::t, 1);

			// if there's no perfect matching
			int flow = MF::dinic(), maximum = neighbor_u.size();

			// printf("flow %d %d\n", flow, maximum);
			if (flow < maximum) {
				//printf("not match (%d, %d)\n", u, v);
				// Phi[u].erase(find(Phi[u].begin(), Phi[u].end(), v));
				PhiSet[u].erase(v);
				for (int ut : neighbor_u)
					for (int vt : neighbor_v) {
						//printf("repeat %d %d\n", ut, vt);
						// if (!inqt[ut][vt] && find(Phi[ut].begin(), Phi[ut].end(), vt) != Phi[ut].end()) {
						if (!inqt[ut][vt] && PhiSet[ut].count(vt)) {
							qt.push({ut, vt});
							inqt[ut][vt] = true;
						}
					}
			}
		}
		q = qt;
		//printf("qt %d  (%d,%d)\n", qt.size(), qt.front().first, qt.front().second);
	}
	for (int u = 1; u <= P.n; u++) {
		Phi[u].clear();
		for (int v : PhiSet[u])
			Phi[u].push_back(v);
	}
}

bool solve(TGraph::Graph &P, TGraph::Graph &G, int G_id) {
	// P.print();
	// G.print();
	std::vector<int> *Phi = new std::vector<int>[P.n + 1];
	initialPhi(P, G, Phi);
	optimize1Phi(P, G, Phi, 1);
	optimize2Phi(P, G, Phi, 3);

	memset(phi, 0, sizeof(phi));
	for (int u = 1; u <= P.n; u++) {
		for (int &v : Phi[u]) {
			phi[u - 1][v - 1] = 1;
		}
	}

	QuickSI::Graph Q = convert(P);
	// for(auto v : Q.nodes) printf("%d %d\n", v.first, v.second.label);
	// for (auto e : Q.edges) printf("%d %d %d\n", e.from, e.to, e.weight);
	Q.GetQISeq(Phi);
	// for (auto v : Q.QISeq) printf("%d %d\n", v.label, v.id);
	// Q.GetQISeq();
		// int __ = 0;
		// for(auto v:Q.QISeq){
		// 	printf("%d: %d %d %d\n", __++, v.id, v.label, v.parent);
		// 	printf("extra: ");
		// 	for(auto x:v.extra)printf("%d %d,  ", x.first, x.second);puts("");
		// }
	// exit(123);
	// for(auto v:Q.QISeq){
	// 	printf("%d %d %d\n", v.id, v.label, v.parent);
	// 	printf("extra: ");
	// 	for(auto x:v.extra)printf("%d %d,  ", x.first, x.second);puts("");
	// }
	QuickSI::Graph G_ = convert(G);
	// for(auto v : G_.nodes) printf("%d %d %d\n", v.first, v.second.label, v.second.deg);
	// for (auto e : G_.edges) printf("%d %d %d\n", e.from, e.to, e.weight);
	// std::vector<int> *Eq = new std::vector<int>[P.n + 1];
	// std::vector<int> fa(G.n + 1);
	// calc_eq(G_, fa);
	IsoSolver solver(Q, G_);
	// for (int i = 0; i < G_.nodes.size(); i++) printf("%d %d\n",i, fa[G_id][i]); fflush(stdout);
	return solver.QuickSI(1, fa[G_id]);
}

// int main() {
// 	// rng = std::mt19937(dev());
// 	// calc_weight();

// 	double freq_thres = 0.1;
// 	double disc_thres = 0.1;
// 	int max_feature_size = 10;
// 	Preprocessor Index(freq_thres, disc_thres, max_feature_size);

// 	QuickSI::Graph query;

// 	query.GetQISeq();
// 	Index.Filtering(query);

// 	std::vector<int> result;

// 	for (int i : Index.candidates) {
// 		IsoSolver solver(query, Database[i]);
// 		if (solver.QuickSI()) result.push_back(i);
// 	}

// 	return 0;
// }
