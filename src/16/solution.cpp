#include "vec2.hpp"
#include "dijkstra.hpp"

#include <cassert>
#include <fstream>
#include <iostream>
#include <iterator>
#include <limits>
#include <map>
#include <regex>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

struct state
{
  vec2i p;
  vec2i d;
};

template<>
struct std::hash<state>
{
  std::size_t operator()(const state& s) const noexcept
  {
    size_t result = s.p[0] & 0xff;
    result <<= 8;
    result |= s.p[1] & 0xff;
    result <<= 8;
    result |= s.d[0] & 0xf;
    result <<= 4;
    result |= s.d[1] & 0xf;
    return result;
  }
};

inline bool
operator<(const state& a, const state& b)
{
  if (a.p == b.p) return a.d < b.d;
  return a.p < b.p;
}

inline bool
operator==(const state& a, const state& b)
{
  return a.p == b.p && a.d == b.d;
}

namespace ops {
struct DASH
{
  static state forward(const state& s) { return state{s.p + s.d, s.d}; }
  static state backward(const state& s) { return state{s.p - s.d, s.d}; }
  static size_t cost() { return 1; }
};
struct TURN_RIGHT
{
  static state forward(const state& s) { return state{s.p, turn_right(s.d)}; }
  static state backward(const state& s) { return state{s.p, turn_left(s.d)}; }
  static size_t cost() { return 1000; }
};
struct TURN_LEFT
{
  static state forward(const state& s) { return state{s.p, turn_left(s.d)}; }
  static state backward(const state& s) { return state{s.p, turn_right(s.d)}; }
  static size_t cost() { return 1000; }
};

}

// directional version
typedef dijkstra<state, ops::DASH, ops::TURN_RIGHT, ops::TURN_LEFT> dijkstra_dir;

struct paths
{
  vector<string> field;

  dijkstra_dir dd;

  vec2i end;
  state beg;

  void init()
  {
    for (int y = 0; y < field.size(); ++y) {
      for (int x = 0; x < field[y].size(); ++x) {
        if (field[y][x] == 'E') {
          end = vec2i{ y, x };
          field[y][x] = '.';
        }
        if (field[y][x] == 'S') {
          beg = state{ vec2i{ y, x }, vec2i{ 0, 1 } };
          field[y][x] = '.';
        }
        if (field[y][x] == '.') {
          for (const auto& d : ops::dirs) {
            dd.add_node(state{ vec2i{ y, x }, d });
          }
        }
      }
    }

    dd.init(beg, state{end, vec2i{0, 1}});
    dd.run();
  }

  int get_total_cost() const
  {
    int result = dd.INF_COST;
    for (const auto& d : ops::dirs) {
      state s{ end, d };
      auto it = dd.cost.find(s);
      if (it != dd.cost.end()) { result = min(result, it->second.first); }
    }
    return result;
  }

  void print() const
  {
    for (const auto& r : field) cout << r << endl;
  }

  template<typename A>
  void op_backward(const state& cur, multimap<int, state>& options, int cur_cost) const
  {
    state s = A::backward(cur);
    auto cit = dd.cost.find(s);
    if (cit != dd.cost.end() && cit->second.first == cur_cost - A::cost()) {
      options.emplace(cit->second.first, s);
    }
  }

  template<typename A, typename B, typename... C>
  void op_backward(const state& cur, multimap<int, state>& options, int cur_cost) const
  {
    state s = A::backward(cur);
    auto cit = dd.cost.find(s);
    if (cit != dd.cost.end() && cit->second.first == cur_cost - A::cost()) {
      options.emplace(cit->second.first, s);
    }
    op_backward<B, C...>(cur, options, cur_cost);
  }

  // from end to start
  void follow(const state& cur, set<state>& states_on_paths, int cur_cost)
  {
    states_on_paths.insert(cur);
    if (cur == beg) return;

    multimap<int, state> options;
    // check next possible states, but never go back
    op_backward<ops::DASH, ops::TURN_RIGHT, ops::TURN_LEFT>(cur, options, cur_cost);

    for (auto it = options.begin(); it != options.end(); ++it) {
      // cout << cur.p << cur.d << " -> " << it->second.p << it->second.d << ": " << min_cost << endl;
      follow(it->second, states_on_paths, it->first);
    }
  }

  size_t follow_paths()
  {
    set<state> states_on_paths;
    int min_end_cost = dd.INF_COST;
    for (const auto& d : ops::dirs) {
      state s{ end, d };
      min_end_cost = min(min_end_cost, dd.cost[s].first);
    }
    for (const auto& d : ops::dirs) {
      state s{ end, d };
      if (dd.cost[s].first == min_end_cost) { follow(s, states_on_paths, dd.cost[s].first); }
    }
    set<vec2i> pos;
    for (const auto& s : states_on_paths) pos.insert(s.p);
#if 0
    vector<string> f(field);
    for(const auto& p : pos) {
      f[p[0]][p[1]] = 'O';
    }
    for(const auto& r : f) cout << r << endl;
#endif
    return pos.size();
  }
};

int
solution1(const string& fname)
{
  paths p;

  ifstream f(fname);
  while (f) {
    string line;
    std::getline(f, line);
    if (!f) break;
    if (line.empty()) continue;
    p.field.emplace_back(move(line));
  }

  p.init();
  // p.print();

  return p.get_total_cost();
}

size_t
solution2(const string& fname)
{
  paths p;

  ifstream f(fname);
  while (f) {
    string line;
    std::getline(f, line);
    if (!f) break;
    if (line.empty()) continue;
    p.field.emplace_back(move(line));
  }

  p.init();
  // p.print();

  return p.follow_paths();
}
