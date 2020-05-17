#include "isomorphism.h"

// void State::log() {
//     printf("In depth %d\nNext possible set is", depth_for_G2);
//     for (auto x : P) printf("(%d, %d) ", x.first, x.second);
//     printf("\n");
// }

// void State::insert(int i, int j) {
//     printf("Insert pair (%d, %d)\n", i, j);
//     in1h = in1.erase(i);
//     out1h = out1.erase(i);
//     in2h = in2.erase(j);
//     out2h = out2.erase(j);
//     M1[i] = j;
//     M2[j] = i;
//     for (auto t : G1.in[i])
//         if (!M1.count(t))
//             if (in1.insert(t).second) in1hs.insert(t);
//     for (auto t : G1.out[i])
//         if (!M1.count(t))
//             if (out1.insert(t).second) out1hs.insert(t);
//     for (auto t : G2.in[j])
//         if (!M2.count(t))
//             if (in2.insert(t).second) in2hs.insert(t);
//     for (auto t : G2.out[j])
//         if (!M2.count(t))
//             if (out2.insert(t).second) out2hs.insert(t);
//     /*
//     printf("M2: ");
//     for (auto x : M2) printf("(%d, %d) ", x.first, x.second);
//     printf("\n");
//     printf("In1: ");
//     for (auto x : in1) printf("%d ", x);
//     printf("\n");
//     printf("Out1: ");
//     for (auto x : out1) printf("%d ", x);
//     printf("\n");
//     printf("In2: ");
//     for (auto x : in2) printf("%d ", x);
//     printf("\n");
//     printf("Out2: ");
//     for (auto x : out2) printf("%d ", x);
//     printf("\n");
//     */
// }

// void State::revert(int i, int j) {
//     M1.erase(i);
//     M2.erase(j);
//     if (in1h) in1.insert(i);
//     if (in2h) in2.insert(j);
//     if (out1h) out1.insert(i);
//     if (out2h) out2.insert(j);
//     for (auto x : in1hs) in1.erase(x);
//     for (auto x : in2hs) in2.erase(x);
//     for (auto x : out1hs) out1.erase(x);
//     for (auto x : out2hs) out2.erase(x);
//     in1hs.clear();
//     in2hs.clear();
//     out1hs.clear();
//     out2hs.clear();
// }

// void State::make_P() {
//     P.clear();
//     bool flag = false;
//     if (out1.size() && out2.size()) {
//         flag = true;
//         for (auto i : out1)
//             if (out2.count(depth_for_G2))
//                 P.insert(std::make_pair(i, depth_for_G2));
//     }
//     if (in1.size() && in2.size()) {
//         flag = true;
//         for (auto i : in1)
//             if (in2.count(depth_for_G2))
//                 P.insert(std::make_pair(i, depth_for_G2));
//     }
//     if (!flag) {
//         if (!M2.count(depth_for_G2))
//             for (int i = 0; i < G1.n; i++)
//                 if (!M1.count(i)) P.insert(std::make_pair(i, depth_for_G2));
//     }
// }

// bool State::is_feasible(int i, int j) {
//     for (auto n : G1.in[i])
//         if (M1.count(n))
//             if (!G2.in[j].count(M1[n])) return false;
//     for (auto n : G2.in[j])
//         if (M2.count(n))
//             if (!G1.in[i].count(M2[n])) return false;
//     for (auto n : G1.out[i])
//         if (M1.count(n))
//             if (!G2.out[j].count(M1[n])) return false;
//     for (auto n : G2.out[j])
//         if (M2.count(n))
//             if (!G1.out[i].count(M2[n])) return false;
//     std::set<int> c1, c2;
//     std::set_intersection(G1.out[i].begin(), G1.out[i].end(), in1.begin(),
//                           in1.end(), std::inserter(c1, c1.begin()));
//     std::set_intersection(G2.out[j].begin(), G2.out[j].end(), in2.begin(),
//                           in2.end(), std::inserter(c2, c2.begin()));
//     if (c1.size() < c2.size()) return false;
//     c1.clear();
//     c2.clear();

//     std::set_intersection(G1.in[i].begin(), G1.in[i].end(), in1.begin(),
//                           in1.end(), std::inserter(c1, c1.begin()));
//     std::set_intersection(G2.in[j].begin(), G2.in[j].end(), in2.begin(),
//                           in2.end(), std::inserter(c2, c2.begin()));
//     if (c1.size() < c2.size()) return false;
//     c1.clear();
//     c2.clear();

//     std::set_intersection(G1.out[i].begin(), G1.out[i].end(), out1.begin(),
//                           out1.end(), std::inserter(c1, c1.begin()));
//     std::set_intersection(G2.out[j].begin(), G2.out[j].end(), out2.begin(),
//                           out2.end(), std::inserter(c2, c2.begin()));
//     if (c1.size() < c2.size()) return false;
//     c1.clear();
//     c2.clear();

//     std::set_intersection(G1.in[i].begin(), G1.in[i].end(), out1.begin(),
//                           out1.end(), std::inserter(c1, c1.begin()));
//     std::set_intersection(G2.in[j].begin(), G2.in[j].end(), out2.begin(),
//                           out2.end(), std::inserter(c2, c2.begin()));
//     if (c1.size() < c2.size()) return false;
//     c1.clear();
//     c2.clear();

//     std::set<int> N1, N2;
//     for (int i = 0; i < G1.n; i++)
//         if (!M1.count(i) && !in1.count(i) && !out1.count(i)) N1.insert(i);
//     for (int i = 0; i < G2.n; i++)
//         if (!M2.count(i) && !in2.count(i) && !out2.count(i)) N2.insert(i);

//     std::set_intersection(G1.in[i].begin(), G1.in[i].end(), N1.begin(),
//                           N1.end(), std::inserter(c1, c1.begin()));
//     std::set_intersection(G2.in[j].begin(), G2.in[j].end(), N2.begin(),
//                           N2.end(), std::inserter(c2, c2.begin()));
//     if (c1.size() < c2.size()) return false;

//     c1.clear();
//     c2.clear();
//     std::set_intersection(G1.out[i].begin(), G1.out[i].end(), N1.begin(),
//                           N1.end(), std::inserter(c1, c1.begin()));
//     std::set_intersection(G2.out[j].begin(), G2.out[j].end(), N2.begin(),
//                           N2.end(), std::inserter(c2, c2.begin()));
//     if (c1.size() < c2.size()) return false;
//     return true;
// }

bool Ullmann::check(int d, int k)
{
    if (A.label[d] != B.label[k])
        return false;
    for (auto x : A.neigh[d])
    {
        bool ok = false;
        for (auto y : B.neigh[k])
            if (M[x][y] == 1)
            {
                ok = true;
                break;
            }
        if (!ok)
        {
            M[d][k] = 0;
            return false;
        }
    }
    return true;
}

void Ullmann::match(int depth)
{
    if (found || timeout)
        return;
    if ((clock() - tik) / CLOCKS_PER_SEC >= 5)
    {
        if (!found)
            printf("Warning: execution time exceeded the time-out. Skip.\n");
        timeout = true;
        return;
    }
    if (depth >= A.n)
    {
        for (int i = 0; i < A.n; i++)
            for (int j = i + 1; j < A.n; j++)
                if (A.adj[i][j] && !B.adj[res[i]][res[j]])
                    return;
        found = true;
        return;
    }
    for (int i = 0; i < B.n; i++)
    {
        bool restore = M[depth][i];
        if (!F[i] && check(depth, i))
        {
            F[i] = 1;
            res[depth] = i;
            match(depth + 1);
            M[depth][i] = restore;
            F[i] = 0;
        }
    }
}

bool Ullmann::subisomorphism()
{
    found = false;
    if (res != NULL)
        delete[] res;
    res = new int[A.n];
    int va = A.n, vb = B.n;
    if (va > vb)
        return false;
    M = new int *[va];
    for (int i = 0; i < va; i++)
    {
        M[i] = new int[vb];
        memset(M[i], 0, vb * sizeof(int));
    }
    for (int i = 0; i < va; i++)
        for (int j = 0; j < vb; j++)
            if (A.deg[i] <= B.deg[j])
                M[i][j] = 1;
    F = new int[vb];
    memset(F, 0, vb * sizeof(int));
    tik = clock();
    match(0);
    delete[] F;
    for (int i = 0; i < va; i++)
        delete[] M[i];
    delete[] M;
    return found;
}

void Ullmann::print_res()
{
    if (found)
        printf("Success.\n");
    else
        printf("Failed or haven't run.\n");
}

// void VF2::match(State &s)
// {
//     if (s.M2.size() >= s.G2.n)
//     {
//         s.found = true;
//         for (std::map<int, int>::iterator i = s.M2.begin(); i != s.M2.end();
//              i++)
//             res[i->first] = i->second;
//         return;
//     }
//     s.make_P();
//     std::map<int, int> pp = s.P;
//     // s.log();
//     if (!pp.empty())
//         for (std::map<int, int>::iterator i = pp.begin(); i != pp.end(); i++)
//         {
//             int a = i->first, b = i->second;
//             // printf("try insert (%d, %d)\n", a, b);
//             if (s.is_feasible(a, b))
//             {
//                 s.insert(a, b);
//                 s.depth_for_G2 += 1;
//                 match(s);
//                 s.depth_for_G2 -= 1;
//                 s.revert(a, b);
//             }
//         }
// }

// bool VF2::subisomorphism()
// {
//     found = false;
//     if (res != NULL)
//         delete[] res;
//     res = new int[s.G2.n];
//     match(s);
//     found = s.found;
//     return found;
// }

// void VF2::print_res()
// {
//     if (found)
//         printf("Success.\n");
//     else
//         printf("Failed or haven't run.\n");
// }