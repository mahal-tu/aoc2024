#include "dijkstra.hpp"
#include "vec2.hpp"

#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>

using namespace std;

template<>
struct std::hash<vec2i>
{
  std::size_t operator()(const vec2i& s) const noexcept
  {
    size_t result = s[0] & 0xff;
    result <<= 8;
    result |= s[1] & 0xff;
    return result;
  }
};

struct path
{
  vector<string> field;

  vec2i beg;
  vec2i end;

  void init()
  {
    for (int y = 0; y < field.size(); ++y) {
      for (int x = 0; x < field[y].size(); ++x) {
        if (field[y][x] == 'E') {
          end = vec2i{ y, x };
          field[y][x] = '.';
        }
        if (field[y][x] == 'S') {
          beg = vec2i{ y, x };
          field[y][x] = '.';
        }
      }
    }
  }

  void print() const
  {
    for (const auto& row : field) { cout << row << endl; }
  }

  int get_path_len() const
  {
    dijkstra2d d;

    for (int y = 0; y < field.size(); ++y) {
      for (int x = 0; x < field[y].size(); ++x) {
        if (field[y][x] == '.') d.add_node(vec2i{ y, x });
      }
    }

    d.init(beg, end);
    d.run();
    int len = d.get_total_cost();
    return len != d.INF_COST ? len : -1;
  }

  int get_num_cheats(int cheat_steps, int min_improvement) const
  {
    dijkstra2d forward;
    dijkstra2d backward;

    for (int y = 0; y < field.size(); ++y) {
      for (int x = 0; x < field[y].size(); ++x) {
        if (field[y][x] == '.') {
          forward.add_node(vec2i{ y, x });
          backward.add_node(vec2i{ y, x });
        }
      }
    }

    forward.init(beg, end);
    forward.run();
    backward.init(end, beg);
    backward.run();

    int base_line = forward.get_total_cost();

    int result = 0;
    // check all pairs of nodes
    for (const auto& f : forward.cost) {
      for (const auto& b : backward.cost) {
        // city block distance
        vec2i dist_vec = f.first - b.first;
        int dist = abs(dist_vec[0]) + abs(dist_vec[1]);
        if (dist <= cheat_steps) {
          int cheat_steps = f.second.first + b.second.first + dist;
          if (base_line - cheat_steps >= min_improvement) ++result;
        }
      }
    }
    return result;
  }
};

template<int _cheatsteps, int _offset>
int
solution(const string& fname)
{
  path p;

  ifstream f(fname);
  while (f) {
    string line;
    std::getline(f, line);
    if (!f) break;
    if (line.empty()) continue;
    p.field.emplace_back(move(line));
  }

  p.init();
  //p.print();

  return p.get_num_cheats(_cheatsteps, _offset);
}
