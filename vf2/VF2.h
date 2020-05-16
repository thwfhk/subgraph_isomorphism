#ifndef VF2_H
#define VF2_H

#include <cstring>
#include <set>
#include <iterator>
#include <vector>
#include <algorithm>
using namespace std;
typedef pair<int, int> prii;
#include "Graph.h"

class VF2
{
public:
	struct State
	{
		vector<prii> s;
		int core1[UGraph::maxv];
		int core2[UGraph::maxv];
		bool in1[UGraph::maxv];
		bool in2[UGraph::maxv];
		bool out1[UGraph::maxv];
		bool out2[UGraph::maxv];
		State()
		{
			s.clear();
			memset(core1, -1, sizeof(core1));
			memset(core2, -1, sizeof(core2));
			memset(in1, 0, sizeof(in1));
			memset(in2, 0, sizeof(in2));
			memset(out1, 0, sizeof(out1));
			memset(out2, 0, sizeof(out2));
		}
	};

	struct Match
	{
		vector<prii> s;
		int id;
		Match() {}
		Match(vector<prii> _s, int _id) : s(_s), id(_id) {}
	};

	VF2() {}
	bool vf2(const UGraph &QG, const UGraph &P);

	void GenRevUGraph(const UGraph &src, UGraph &dst);
	bool CheckPrev(const State &s, int a, int b);
	bool CheckSucc(const State &s, int a, int b);
	bool CheckIn(const State &s);
	bool CheckOut(const State &s);
	bool CheckNew(const State &s);
	void CalDFSVec(const State &s);
	void CalCheckVec(const State &s, int a, int b);
	bool check(const State &s, int a, int b);
	void GenPairs(const State &s);
	void CheckPairs(const State &s);
	void UpdateState(State &s, int a, int b);
	bool FinalCheck(const State &s);
	bool dfs(const State &s);
	bool query();

	UGraph QueryUGraph;

	vector<prii> tlist;
	vector<prii> allPairs;
	vector<prii> candiPairs;
	bool flagIn, flagOut, flagAll;
	vector<int> pred1, pred2;
	vector<int> succ1, succ2;
	vector<int> m1, m2;
	vector<int> tin1, tin2;
	vector<int> tout1, tout2;
	vector<int> n1, n2;
	vector<int> ns1, ns2;
	vector<int> t1, t2;

	UGraph pat, g;
	UGraph revpat, revg;
};

#endif