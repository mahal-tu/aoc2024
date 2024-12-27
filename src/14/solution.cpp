#include <cassert>
#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <vector>

#include "vec2.hpp"

using namespace std;

struct robot
{
  vec2i p;
  vec2i v;
};

bool
try_read(const string& s, robot& r)
{
  regex rg("p=(\\d+),(\\d+) v=([\\d-]+),([\\d-]+)");

  for (auto m = sregex_iterator(s.begin(), s.end(), rg); m != std::sregex_iterator(); ++m) {
    smatch sm(*m);
    if (sm.size() == 5) {
      // full match, then the captures
      r.p = vec2i({ stoi(sm[1].str()), stoi(sm[2].str()) });
      r.v = vec2i({ stoi(sm[3].str()), stoi(sm[4].str()) });
      return true;
    }
  }

  return false;
}

template<int w, int h>
void
print(const vector<robot>& v)
{
  static int counts[w][h];
  for (auto& col : counts) { fill(begin(col), end(col), 0); }

  for (const auto& r : v) {
    assert(r.p[0] >= 0);
    assert(r.p[1] >= 0);
    assert(r.p[0] < w);
    assert(r.p[1] < h);
    ++counts[r.p[0]][r.p[1]];
  }

  for (size_t y = 0; y < h; ++y) {
    cout << y << "\t";
    for (size_t x = 0; x < w; ++x) {
      if (counts[x][y] > 0) {
        cout << "x";
      } else {
        cout << ".";
      }
    }
    cout << endl;
  }
}

template<int w, int h>
bool
is_symm_y(const vector<robot>& v)
{
  static int counts[w][h];
  for (auto& col : counts) { fill(begin(col), end(col), 0); }

  for (const auto& r : v) {
    assert(r.p[0] >= 0);
    assert(r.p[1] >= 0);
    assert(r.p[0] < w);
    assert(r.p[1] < h);
    ++counts[r.p[0]][r.p[1]];
  }

  int diffs = 0;
  for (size_t y = 0; y < h; ++y) {
    for (size_t x = 0; x < w / 2; ++x) { diffs += abs(counts[x][y] - counts[w - x - 1][y]); }
  }
  return diffs < 100;
}

template<int w, int h>
int
num_lonely(const vector<robot>& v)
{
  static int counts[w][h];
  for (auto& col : counts) { fill(begin(col), end(col), 0); }

  for (const auto& r : v) {
    assert(r.p[0] >= 0);
    assert(r.p[1] >= 0);
    assert(r.p[0] < w);
    assert(r.p[1] < h);
    ++counts[r.p[0]][r.p[1]];
  }

  auto is_lonely = [&](int px, int py) {
    static constexpr struct
    {
      int x;
      int y;
    } dirs[8] = { { 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 }, { 1, 1 }, { -1, 1 }, { 1, -1 }, { -1, -1 } };
    for (const auto& d : dirs) {
      if (px + d.x < 0) continue;
      if (py + d.y < 0) continue;
      if (px + d.x >= w) continue;
      if (py + d.y >= h) continue;
      if (counts[px + d.x][py + d.y] != 0) return false;
    }
    return true;
  };

  int result = 0;
  for (const auto& r : v) {
    if (is_lonely(r.p[0], r.p[1])) ++result;
  }
  return result;
}

template<int w, int h>
int
asymmetry(const vector<robot>& v)
{
  const int mid_x = w / 2;
  auto get_side = [&](const vec2i& p) {
    if (p[0] == mid_x) return 0;
    return (p[0] > mid_x) ? 1 : -1;
  };

  map<int, map<int, int>> counts;
  for (auto& r : v) { counts[r.p[1]][get_side(r.p)]++; }

  int result = 0;
  for (const auto& col2c : counts) { result += abs(int(col2c.second.count(-1)) - int(col2c.second.count(1))); }
  return result;
}

template<int width, int height>
long long int
solution1(const string& fname)
{
  ifstream f(fname);

  vector<robot> v;

  while (f) {
    string line;
    std::getline(f, line);
    if (!f) break;
    if (line.empty()) continue;

    robot r;
    if (try_read(line, r)) {
      cout << r.p << " " << r.v << endl;
      v.emplace_back(r);
    }
  }

  const int steps = 100;
  for (int s = 0; s < steps; ++s) {
    for (auto& r : v) {
      r.p += r.v;
      while (r.p[0] < 0) r.p[0] += width;
      while (r.p[1] < 0) r.p[1] += height;

      while (r.p[0] >= width) r.p[0] -= width;
      while (r.p[1] >= height) r.p[1] -= height;
    }
  }

  // get number per quadrant
  int mid_x = width / 2;
  int mid_y = height / 2;
  auto get_quadrant = [&](const vec2i& p) {
    if (p[0] == mid_x) return -1;
    if (p[1] == mid_y) return -1;

    int result = 0;
    if (p[0] > mid_x) ++result;
    if (p[1] > mid_y) result += 2;
    return result;
  };

  map<int, int> counts;
  for (auto& r : v) { counts[get_quadrant(r.p)]++; }

  long long int result = 1;
  for (const auto& q2c : counts) {
    if (q2c.first >= 0) result *= q2c.second;
  }

  return result;
}

template<int width, int height>
long long int
solution2(const string& fname)
{
  ifstream f(fname);

  vector<robot> v;

  while (f) {
    string line;
    std::getline(f, line);
    if (!f) break;
    if (line.empty()) continue;

    robot r;
    if (try_read(line, r)) {
      //cout << r.p << " " << r.v << endl;
      v.emplace_back(r);
    }
  }

  // task says "most of the robots", let's assume at least 2 thirds
  const int not_most_of_robots = v.size() / 3;
  int min_lonely = 1000;

  long long int result = 0;
  for (long long int s = 1; s < 100000; ++s) {
    for (auto& r : v) {
      r.p += r.v;
      while (r.p[0] < 0) r.p[0] += width;
      while (r.p[1] < 0) r.p[1] += height;

      while (r.p[0] >= width) r.p[0] -= width;
      while (r.p[1] >= height) r.p[1] -= height;
    }
    auto lonely = num_lonely<width, height>(v);

    if(lonely < not_most_of_robots) {
      // most of the robots seem to have some connection to each other
      return s;
    }

    if (lonely < min_lonely) {
      min_lonely = lonely;
      result = s;
    }
  }
  cout << min_lonely << endl;

  // print<width, height>(v);

  return result;
}
