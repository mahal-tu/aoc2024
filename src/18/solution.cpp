#include "dijkstra.hpp"
#include "vec2.hpp"

#include <boost/tokenizer.hpp>
#include <cassert>
#include <fstream>
#include <iostream>
#include <iterator>
#include <limits>
#include <sstream>
#include <vector>

using namespace std;
using namespace boost;

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

template<int w>
struct path
{
  typedef array<array<bool, w>, w> _field;
  _field field;

  path()
  {
    for (auto& row : field) { fill(begin(row), end(row), false); }
  }

  void print() const
  {
    for (const auto& row : field) {
      for (const auto& p : row) { cout << (p ? "#" : "."); }
      cout << endl;
    }
  }

  int get_path_len(const vec2i& beg, const vec2i& end) const
  {
    dijkstra2d d;

    for (int y = 0; y < w; ++y) {
      for (int x = 0; x < w; ++x) {
        if (!field[y][x]) d.add_node(vec2i{ y, x });
      }
    }

    d.init(beg, end);
    d.run();
    int len = d.get_total_cost();
    return len != d.INF_COST ? len : -1;
  }

  vector<vec2i> get_path(const vec2i& beg, const vec2i& end) const {
    dijkstra2d d;

    for (int y = 0; y < w; ++y) {
      for (int x = 0; x < w; ++x) {
        if (!field[y][x]) d.add_node(vec2i{ y, x });
      }
    }

    d.init(beg, end);
    d.run();
    return d.shortest_path();
  }

#if 0
  static constexpr vec2i dirs[4] = {{0,1},{1,0},{0,-1},{-1,0}};
  static constexpr int NO_PATH = -1;
  static constexpr int MAX_PATH = w*w;

  int next_step(_field& f, const vec2i& p, const vec2i& target) const {
    int best_steps = MAX_PATH;
    // no longer allow coming here
    f[p[0]][p[1]] = true;
    for (const auto& d : dirs) {
      vec2i n = p + d;
      if(n == target) {
        f[p[0]][p[1]] = false;
        return 1;
      }

      if(n[0]<0 || n[0]>=w) continue;
      if(n[1]<0 || n[1]>=w) continue;
      if(f[n[0]][n[1]]) continue;

      // try step
      int try_step = next_step(f, n, target);
      if (try_step != NO_PATH) {
        if(try_step < best_steps) {
          best_steps = try_step;
        }
      }
    }

    // free up p again
    f[p[0]][p[1]] = false;

    if(best_steps < MAX_PATH) {
      return best_steps + 1;
    } else {
      return NO_PATH;
    }
  }

  int get_path_len() const {
    _field f = field;
    vec2i start{0, 0};
    vec2i target{w-1, w-1};

    int result = next_step(f, start, target);

    return result;
  }
#endif
};

template<int w, int steps>
int
solution1(const string& fname)
{
  vector<vec2i> bad_pos;
  ifstream f(fname);
  while (f) {
    string line;
    std::getline(f, line);
    if (!f) break;
    if (line.empty()) continue;
    char_separator<char> sep(",");
    tokenizer<char_separator<char>> tokens(line, sep);
    int idx = 0;
    bad_pos.emplace_back(vec2i{});
    for (auto tok = tokens.begin(); tok != tokens.end(); ++tok) { bad_pos.back()[idx++] = stoi(*tok); }
  }

  path<w> p;

  for (size_t u = 0; u < steps; ++u) { p.field[bad_pos[u][1]][bad_pos[u][0]] = true; }

  p.print();

  return p.get_path_len(vec2i{ 0, 0 }, vec2i{ w - 1, w - 1 });
}

template<int w>
std::string
solution2(const string& fname)
{
  vector<vec2i> bad_pos;
  ifstream f(fname);
  while (f) {
    string line;
    std::getline(f, line);
    if (!f) break;
    if (line.empty()) continue;
    char_separator<char> sep(",");
    tokenizer<char_separator<char>> tokens(line, sep);
    int idx = 0;
    bad_pos.emplace_back(vec2i{});
    for (auto tok = tokens.begin(); tok != tokens.end(); ++tok) { bad_pos.back()[idx++] = stoi(*tok); }
  }

  path<w> p;
  vector<vec2i> cur_path;
  // start with diagonal path
  cur_path.emplace_back(vec2i{0,0});
  for(int i=1; i<w; ++i) {
    cur_path.emplace_back(vec2i{i-1,i});
    cur_path.emplace_back(vec2i{i,i});
  }
  sort(cur_path.begin(), cur_path.end());

  for (size_t u = 0; u < bad_pos.size(); ++u) {
    p.field[bad_pos[u][1]][bad_pos[u][0]] = true;
    // does it block the current path?
    if(binary_search(cur_path.begin(), cur_path.end(), vec2i{bad_pos[u][1], bad_pos[u][0]})) {
      cur_path = p.get_path(vec2i{ 0, 0 }, vec2i{ w - 1, w - 1 });
      sort(cur_path.begin(), cur_path.end());
      if (cur_path.empty()) {
        ostringstream ostr;
        ostr << bad_pos[u][0] << "," << bad_pos[u][1];
        return ostr.str();
      }
    }
  }

  return "";
}
