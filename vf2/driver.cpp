#include "isomorphism.h"

int main() {
    Graph a = Graph(3);
    a.insert(0, 1);
    a.insert(1, 2);
    Graph b = Graph(3);
    b.insert(0, 2);
    b.insert(1, 2);
    Ullmann u = Ullmann(a, b);
    u.subisomorphism();
    u.print_res();

    Digraph aa = Digraph(7);
    aa.insert(0, 1);
    aa.insert(1, 2);
    aa.insert(2, 3);
    aa.insert(3, 4);
    aa.insert(4, 5);
    aa.insert(5, 6);
    aa.insert(6, 1);
    Digraph bb = Digraph(5);
    bb.insert(0, 1);
    bb.insert(1, 2);
    bb.insert(3, 1);
    bb.insert(4, 3);
    VF2 v = VF2(aa, bb);
    v.subisomorphism();
    v.print_res();
    return 0;
}
