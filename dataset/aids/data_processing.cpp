#include <iostream>
#include <fstream>
#include <cstdio>
#include <algorithm>
#include <cstring>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <queue>

using namespace std;

int data_cnt = 0;

int label_cnt = 0;
map<string, int> ha;

int Int(char c) {
  if (c == ' ') return 0;
  else return c-'0';
}
int main() {
  ios::sync_with_stdio(false);
  ifstream fin; fin.open("AIDO99SD");
  ofstream fout("aids.data");
  while (true) {
    string s;
    int n, m;
    if (!getline(fin, s)) break;
    getline(fin, s);
    getline(fin, s);
    getline(fin, s);
    n = Int(s[0]) * 100 + Int(s[1]) * 10 + Int(s[2]);
    m = Int(s[3]) * 100 + Int(s[4]) * 10 + Int(s[5]);
    // printf("nm %d %d\n", n, m);
    fout << "Graph " << (++data_cnt) << "\n" 
         << n << " " << m << "\n";
    for (int i=1; i<=n; i++) {
      double x;
      fin >> x >> x >> x >> s;
      if (!ha.count(s)) {
        ha[s] = ++label_cnt;
        cout << s << " " << label_cnt << endl;
      }
      fout << ha[s] << " ";
      getline(fin, s);
    }
    fout << "\n";
    for (int i=1; i<=m; i++) {
      int u, v;
      getline(fin, s);
      u = Int(s[0]) * 100 + Int(s[1]) * 10 + Int(s[2]);
      v = Int(s[3]) * 100 + Int(s[4]) * 10 + Int(s[5]);
      // fin >> u >> v; getline(fin, s);
      fout << u << " " << v << "\n";
    }
    getline(fin, s);
    getline(fin, s);
    getline(fin, s);
    getline(fin, s);
    getline(fin, s);
    // fout << "(maybe)id: " << s << "\n\n";
    fout << "\n";
    getline(fin, s);
    getline(fin, s);
    if (s != "$$$$") cerr << "ERROR\n";
  }
}