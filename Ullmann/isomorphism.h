#ifndef Iso_wc
#define Iso_wc
#include <ctime>

#include "Graph.h"

// struct State {
//     Digraph &G1, &G2;
//     std::map<int, int> M1, M2, P;
//     std::set<int> in1, out1, in2, out2;
//     int depth_for_G2 = 0;
//     bool found = false, in1h, in2h, out1h, out2h;
//     std::set<int> in1hs, in2hs, out1hs, out2hs;

//     void log();
//     void insert(int, int);
//     void revert(int, int);
//     void make_P();
//     bool is_feasible(int, int);

//     State(Digraph &A, Digraph &B) : G1(A), G2(B){};
// };

struct Ullmann
{
    UGraph &A, &B;
    int *res = NULL, *F;
    int **M;
    bool found, timeout = false;
    double tik;

    bool check(int, int);
    void match(int);
    bool subisomorphism();
    void print_res();

    Ullmann(UGraph &A, UGraph &B) : A(A), B(B) {}
};

// struct VF2 {
//     Digraph &G1, &G2;
//     State s;
//     int *res = NULL;
//     bool found;

//     void match(State &);
//     bool subisomorphism();
//     void print_res();

//     VF2(Digraph &A, Digraph &B) : G1(A), G2(B), s(State(A, B)) {}
// };

#endif