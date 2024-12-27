#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
#include <regex>
#include <set>
#include <sstream>

using namespace std;

static constexpr int SIZE = 5;

typedef array<int, SIZE> tpl;

long long unsigned
solution1(const string& fname)
{
  vector<tpl> keys;
  vector<tpl> locks;

  int cur_line = 0;
  tpl cur_tpl;
  fill(cur_tpl.begin(), cur_tpl.end(), 0);
  bool is_key = false;

  ifstream f(fname);
  while (f) {
    string line;
    std::getline(f, line);
    if (!f) break;
    if (line.empty()) {
      cur_line = 0;
      fill(cur_tpl.begin(), cur_tpl.end(), 0);
      continue;
    }

    assert(line.length() == SIZE);
    for (size_t u = 0; u < SIZE; ++u) {
      if (cur_line > 0 && cur_line < 6) {
        if (line[u] == '#') ++cur_tpl[u];
      }
    }

    if (cur_line == 0) is_key = line[0] == '.';

    if (cur_line == 6) {
      if (is_key) {
        keys.emplace_back(cur_tpl);
      } else {
        locks.emplace_back(cur_tpl);
      }
    }

    ++cur_line;
  }

  cout << "locks" << endl;
  for (const auto& l : locks) {
    copy(l.begin(), l.end(), ostream_iterator<int>(cout, ","));
    cout << endl;
  }

  cout << "keys" << endl;
  for (const auto& k : keys) {
    copy(k.begin(), k.end(), ostream_iterator<int>(cout, ","));
    cout << endl;
  }

  long long unsigned result = 0;
  for (const auto& l : locks) {
    for (const auto& k : keys) {
      int fitting = 0;
      for (size_t u = 0; u < SIZE; ++u) {
        if (l[u] + k[u] <= SIZE) ++fitting;
      }
      if (fitting == SIZE) ++result;
    }
  }

  return result;
}
