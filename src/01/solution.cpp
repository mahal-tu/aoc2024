#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
#include <vector>

using namespace std;

template<int task>
int
solution(const string& fname)
{
  vector<int> a, b;
  map<int, int> c;

  ifstream f(fname);
  while (f) {
    int l, r;
    f >> l >> r;
    if (f) {
      a.emplace_back(l);
      b.emplace_back(r);
      c[r]++;
    }
  }

  std::sort(a.begin(), a.end());
  std::sort(b.begin(), b.end());

  int sum = 0;
  int similarity = 0;
  for (size_t u = 0; u < a.size(); ++u) {
    sum += abs(a[u] - b[u]);
    similarity += a[u] * c[a[u]];
  }

  if (task == 1) {
    return sum;
  } else if (task == 2) {
    return similarity;
  }

  return 0;
}
