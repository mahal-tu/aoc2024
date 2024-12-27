#include "vec2.hpp"

#include <array>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <vector>

using namespace std;

template<int task>
int
solution(const string& fname)
{
  vector<string> field;

  ifstream f(fname);
  while (f) {
    string line;
    std::getline(f, line);
    if (!f) break;
    if (line.empty()) continue;
    field.emplace_back(move(line));
  }

  // for(const auto& l : field) cout << l << endl;

  const int height = field.size();
  const int width = field.front().size();
  auto within = [=](const vec2i& v) {
    return v[0] >= 0 && v[0] < height && v[1] >= 0 && v[1] < width;
  };

  map<char, set<vec2i>> antennas;
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      if (field[y][x] != '.') { antennas[field[y][x]].insert(vec2i{ y, x }); }
    }
  }

  set<vec2i> antinodes;
  for (const auto& antenna : antennas) {
    // check all pairs
    const auto& positions = antenna.second;
    for (auto i = positions.begin(); i != positions.end(); ++i) {
      auto j = i;
      ++j;
      for (; j != positions.end(); ++j) {
        auto diff = *i - *j;
        if (task == 1) {
          vec2i a1 = *i + diff;
          vec2i a2 = *j - diff;
          // cout << *i << "-" << *j << " -> " << a1 << " " << a2 << endl;
          if (within(a1)) antinodes.insert(a1);
          if (within(a2)) antinodes.insert(a2);
        } else if (task == 2) {
          antinodes.insert(*i);
          vec2i a = *i + diff;
          while (within(a)) {
            antinodes.insert(a);
            a += diff;
          }
          a = *i - diff;
          while (within(a)) {
            antinodes.insert(a);
            a -= diff;
          }
        }
      }
    }
  }

  return antinodes.size();
}
