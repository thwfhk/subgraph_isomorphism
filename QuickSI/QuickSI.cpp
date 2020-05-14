#include "IsoSolver.h"
#include "Preprocessor.h"

std::random_device dev;
std::mt19937 rng;

std::vector<Graph> Database;
std::unordered_map<short, int> label_weights;
std::unordered_map<std::pair<short, short>, int> edge_weights;

void calc_weight() {
	for (const auto &G : Database) {
		for (const auto &v : G.nodes) {
			label_weights[v.second.label]++;
		}
		for (const auto &e : G.edges) {
			short u = G.nodes.find(e.from)->second.label;
			short v = G.nodes.find(e.to)->second.label;
			if (u > v) std::swap(u, v);
			edge_weights[std::make_pair(u, v)]++;
		}
	}
	for (auto &G : Database) {
		for (auto &v : G.nodes) {
			v.second.weight = label_weights[v.second.label];
		}
		for (auto &e : G.edges) {
			short u = G.nodes[e.from].label;
			short v = G.nodes[e.to].label;
			if (u > v) std::swap(u, v);
			e.weight = edge_weights[std::make_pair(u, v)];
		}
	}
}

int main() {
	rng = std::mt19937(dev());
	calc_weight();

	double freq_thres = 0.1;
	double disc_thres = 0.1;
	int max_feature_size = 10;
	Preprocessor Index(freq_thres, disc_thres, max_feature_size);

	Graph query;

	query.GetQISeq();
	Index.Filtering(query);

	std::vector<int> result;

	for (int i : Index.candidates) {
		IsoSolver solver(query, Database[i]);
		if (solver.QuickSI()) result.push_back(i);
	}

	return 0;
}
