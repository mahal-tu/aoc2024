#include "vec2.hpp"

#include <array>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <vector>

using namespace std;

vector<string>
pad_data(const vector<string>& x, size_t n, char c = ' ')
{
  size_t w = x.front().size();
  size_t h = x.size();

  vector<string> result;
  for (size_t u = 0; u < h + 2 * n; ++u) { result.emplace_back(string(h + 2 * n, c)); }

  // copy original content into the padded one
  for (size_t u = 0; u < h; ++u) { std::copy(x[u].begin(), x[u].end(), result[u + n].begin() + n); }

  return result;
}

struct agent
{
  vec2i pos;
  vec2i dir;

  void turn_right() { dir = ::turn_right(dir); }

  //! \return whether walk is complete
  bool step(const vector<string>& field)
  {
    vec2i front_pos = pos + dir;
    // cout << front_pos << " " << field[front_pos[0]][front_pos[1]] << endl;
    switch (field[front_pos[0]][front_pos[1]]) {
      case '.': pos += dir; return false;
      case '#': turn_right(); return false;
      default: pos += dir; return true;
    }
  }
};

bool
would_cycle(const vector<string>& f, const agent& a)
{
  vector<string> field(f);
  agent b(a);

  vec2i front_pos = b.pos + b.dir;
  if (field[front_pos[0]][front_pos[1]] != '.') { return false; }

  field[front_pos[0]][front_pos[1]] = '#';

  for (int i = 0; i < f.size() * f.front().size(); ++i) {
    if (b.step(field)) break;
    if (b.pos == a.pos && b.dir == a.dir) return true;
  }

  return false;
}

template<int task>
long long int
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

  field = pad_data(field, 1, ' ');

  for (const auto& l : field) cout << l << endl;

  agent a;
  a.dir = { -1, 0 };

  for (int y = 0; y < field.size(); ++y) {
    for (int x = 0; x < field[y].size(); ++x) {
      if (field[y][x] == '^') {
        a.pos[0] = y;
        a.pos[1] = x;
        // remove agent from field
        field[y][x] = '.';
      }
    }
  }

  set<vec2i> cycle_points;
  set<vec2i> visited;
  cout << a.pos << "," << a.dir << endl;
  visited.insert(a.pos);

  for (int i = 0; i < 100000; ++i) {
    if (would_cycle(field, a)) {
      cycle_points.insert(a.pos + a.dir);
      cout << (a.pos + a.dir) << endl;
    }
    if (a.step(field)) break;
    cout << a.pos << " " << a.dir << endl;
    visited.insert(a.pos);
  }

  if (task == 1) {
    return visited.size();
  } else if (task == 2) {
    return cycle_points.size();
  }

  return 0;
}
