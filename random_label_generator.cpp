#include <iostream>
#include <fstream>
#include <sstream>
#include <random>
#include <algorithm>
using namespace std;

default_random_engine generator(317);
int mu = 15;
normal_distribution<double> distribution(mu,5.0);
int c[100];
int get_num() {
  int x = distribution(generator);
  while (x < 0 || x > 2*mu) x = distribution(generator);
  return x;
}
int main() {

  // for (int i=1; i<=10000; i++) {
  //   int x = distribution(generator);
  //   while (x < 0 || x > 2*mu) x = distribution(generator);
  //   c[x]++;
  // }
  // for (int i=1; i<=2*mu; i++) printf("%d : %d\n", i, c[i]);

  ifstream fin("dataset/dblp/dblp500.data");
  ofstream fout("dataset/dblp_withlabel/dblp500/dblp500.data");
  int total = 100;
  for (int i=1; i<=total; i++) {
    string s;
    getline(fin, s);
    fout << s << "\n";
    int n, m;
    fin >> n >> m;
    getline(fin, s); // just a "\n"
    fout << n << " " << m << "\n";
    getline(fin, s);
    istringstream sin(s); 
    int x;
    while (sin >> x) fout << get_num() << " ";
    fout << "\n";
    for (int j=1; j<=m; j++) {
      int u, v;
      fin >> u >> v;
      fout << u+1 << " " << v+1 << "\n";
    }
    getline(fin, s); // just a "\n"
    getline(fin, s); // empty line
    fout << s << "\n";
  }
}