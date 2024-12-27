#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <numeric>
#include <sstream>
#include <vector>

using namespace std;

bool
safe(const std::vector<int>& v)
{
  auto beg = v.begin();
  auto end = v.end();
  int direction = 0;
  while (beg != end) {
    auto next = beg;
    ++next;
    if (next != end) {
      int cur_diff = *next - *beg;
      if (abs(cur_diff) < 1 || abs(cur_diff) > 3) break;
      int cur_direction = cur_diff > 0 ? 1 : -1;
      if (direction == 0) {
        direction = cur_direction;
      } else {
        if (direction != cur_direction) break;
      }
    } else {
      return true;
    }
    beg = next;
  }

  return false;
}

template<int task>
int
solution(const string& fname)
{
  int num_safe = 0;

  ifstream f(fname);
  while (f) {
    string line;
    std::getline(f, line);
    if (!f) break;

    istringstream istr(line);
    auto beg = istream_iterator<int>(istr);
    auto end = istream_iterator<int>();
    vector<int> v(beg, end);
    if (safe(v)) {
      ++num_safe;
    } else {
      if (task == 2) {
        // try again with one measurement removed
        for (size_t u = 0; u < v.size(); ++u) {
          vector<int> w(v);
          w.erase(w.begin() + u);
          if (safe(w)) {
            cout << line << " - " << u << endl;
            ++num_safe;
            break;
          }
        }
      }
    }
  }

  return num_safe;
}
