#define QUICKSI

#include "../GraphDS.h"
#include "IsoSolver.h"
// #include "Preprocessor.h"

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

void initialize(int data_num, TGraph::Graph *gs[__N]) {
	rng = std::mt19937(dev());

	label_weights.clear();
	edge_weights.clear();
	for (int i = 1; i <= data_num; i++) {
		for (int v = 1; v <= gs[i]->n; v++) {
			short lv = gs[i]->label[v];
			label_weights[lv]++;
			for (int e = gs[i]->h[v]; e; e = gs[i]->e[e].ne) {
				int u = gs[i]->e[e].v;
				short lu = gs[i]->label[u];
				if (lv < lu) edge_weights[std::make_pair(lv, lu)]++;
			}
		}
	}
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
	return Q;
}

const int __V = 2000;
bool phi[__V][__V];

bool solve(TGraph::Graph &P, TGraph::Graph &G, int G_id) {
	// P.print();
	// G.print();
	QuickSI::Graph Q = convert(P);
	// for(auto v : Q.nodes) printf("%d %d\n", v.first, v.second.label);
	// for (auto e : Q.edges) printf("%d %d %d\n", e.from, e.to, e.weight);
	Q.GetQISeq();
	// for (auto v : Q.QISeq) printf("%d %d\n", v.label, v.id);
	// Q.GetQISeq();
	// if (__flag) {
		// int ___ = 0;
		// for(auto v:Q.QISeq){
		// 	printf("%d: %d %d %d\n", ___++, v.id, v.label, v.parent);
		// 	printf("extra: ");
		// 	for(auto x:v.extra)printf("%d %d,  ", x.first, x.second);puts("");
		// }
	// exit(123);
	// }
	// for(auto v:Q.QISeq){
	// 	printf("%d %d %d\n", v.id, v.label, v.parent);
	// 	printf("extra: ");
	// 	for(auto x:v.extra)printf("%d %d,  ", x.first, x.second);puts("");
	// }
	QuickSI::Graph G_ = convert(G);
	// for(auto v : G_.nodes) printf("%d %d %d\n", v.first, v.second.label, v.second.deg);
	// for (auto e : G_.edges) printf("%d %d %d\n", e.from, e.to, e.weight);
	IsoSolver solver(Q, G_);
	return solver.QuickSI(0);
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
