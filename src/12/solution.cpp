#include <fstream>
#include <iostream>
#include <iterator>
#include <list>
#include <map>
#include <vector>

#include "vec2.hpp"

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

void
mark(vector<string>& field, vector<vector<int>>& result, int y, int x, char c, int reg_id)
{
  if (y < 0 || y >= field.size()) return;
  if (x < 0 || x >= field[y].size()) return;
  if (field[y][x] != c) return;

  result[y][x] = reg_id;
  field[y][x] = 0;

  struct
  {
    int y, x;
  } idx[4] = { { 1, 0 }, { 0, 1 }, { -1, 0 }, { 0, -1 } };
  for (const auto& i : idx) { mark(field, result, y + i.y, x + i.x, c, reg_id); }
}

vector<vector<int>>
regions(const vector<string>& f)
{
  vector<string> field(f);
  int height = field.size();
  int width = field.front().size();

  vector<vector<int>> result(height, vector<int>(width));

  int cur_region = 1;

  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      if (result[y][x] > 0) continue;

      char cur_plant = field[y][x];
      mark(field, result, y, x, cur_plant, cur_region);
      ++cur_region;
    }
  }

  return result;
}

int
solution1(const string& fname)
{

  vector<string> field;
  map<int, vec2i> plants;

  ifstream f(fname);
  while (f) {
    string line;
    std::getline(f, line);
    if (!f) break;
    if (line.empty()) continue;
    field.emplace_back(move(line));
  }

  int height = field.size();
  int width = field.front().size();
  int pad = 1;
  field = pad_data(field, pad);
  auto regs = regions(field);
#if 0
  for(const auto& row : regs) {
    copy(row.begin(), row.end(), std::ostream_iterator<int>(std::cout, " "));
    cout << endl;
  }
#endif

  for (int y = pad; y < height + pad; ++y) {
    for (int x = pad; x < width + pad; ++x) {
      struct
      {
        int y, x;
      } idx[4] = { { 1, 0 }, { 0, 1 }, { -1, 0 }, { 0, -1 } };

      int fence = 0;
      for (const auto& i : idx) {
        if (field[y][x] != field[y + i.y][x + i.x]) ++fence;
      }

      plants[regs[y][x]] += vec2i{ 1, fence };
    }
  }

  int result = 0;
  for (const auto& p : plants) {
    cout << p.first << ": " << p.second[0] << "x" << p.second[1] << endl;
    result += p.second[0] * p.second[1];
  }
  return result;
}

struct fence_part
{
  vec2i a, b;
};
ostream&
operator<<(ostream& o, const fence_part& f)
{
  o << "[" << f.a << "," << f.b << "]";
  return o;
}

int
count_gaps_h(const vector<fence_part>& f)
{
  vector<fence_part> fence(f);
  sort(fence.begin(), fence.end(), [](const fence_part& a, const fence_part& b) {
    int a_dir = a.a[0] - a.b[0];
    int b_dir = b.a[0] - b.b[0];
    if (a_dir == b_dir) {
      if (a.a[0] == b.a[0]) return a.a[1] < b.a[1];
      return a.a[0] < b.a[0];
    } else {
      return a_dir < b_dir;
    }
  });

#if 0
  cout << "h ";
  copy(fence.begin(), fence.end(), std::ostream_iterator<fence_part>(std::cout, " "));
  cout << endl;
#endif

  int result = 1;
  for (size_t u = 1; u < fence.size(); ++u) {
    // new y coord
    if (fence[u].a[0] != fence[u - 1].a[0] || fence[u].b[0] != fence[u - 1].b[0]) {
      ++result;
      continue;
    }
    // gap in x coord
    if (fence[u].a[1] - fence[u - 1].a[1] > 1) {
      ++result;
      continue;
    }
  }

  return result;
}

int
count_gaps_v(const vector<fence_part>& f)
{
  vector<fence_part> fence(f);
  sort(fence.begin(), fence.end(), [](const fence_part& a, const fence_part& b) {
    int a_dir = a.a[1] - a.b[1];
    int b_dir = b.a[1] - b.b[1];
    if (a_dir == b_dir) {
      if (a.a[1] == b.a[1]) return a.a[0] < b.a[0];
      return a.a[1] < b.a[1];
    } else {
      return a_dir < b_dir;
    }
  });

#if 0
  cout << "v ";
  copy(fence.begin(), fence.end(), std::ostream_iterator<fence_part>(std::cout, " "));
  cout << endl;
#endif

  int result = 1;
  for (size_t u = 1; u < fence.size(); ++u) {
    // new x coord
    if (fence[u].a[1] != fence[u - 1].a[1] || fence[u].b[1] != fence[u - 1].b[1]) {
      ++result;
      continue;
    }
    // gap in y coord
    if (fence[u].a[0] - fence[u - 1].a[0] > 1) {
      ++result;
      continue;
    }
  }

  return result;
}

int
solution2(const string& fname)
{

  vector<string> field;
  map<int, vec2i> plants;
  map<int, char> reg_names;

  ifstream f(fname);
  while (f) {
    string line;
    std::getline(f, line);
    if (!f) break;
    if (line.empty()) continue;
    field.emplace_back(move(line));
  }

  int height = field.size();
  int width = field.front().size();
  int pad = 1;
  field = pad_data(field, pad);
  auto regs = regions(field);
#if 0
  for(const auto& row : regs) {
    copy(row.begin(), row.end(), std::ostream_iterator<int>(std::cout, " "));
    cout << endl;
  }
#endif
  map<int, vector<fence_part>> fences_v;
  map<int, vector<fence_part>> fences_h;

  for (int y = pad; y < height + pad; ++y) {
    for (int x = pad; x < width + pad; ++x) {
      reg_names[regs[y][x]] = field[y][x];

      struct
      {
        int y, x;
      } idx[4] = { { 1, 0 }, { 0, 1 }, { -1, 0 }, { 0, -1 } };

      int fence = 0;
      for (const auto& i : idx) {
        if (field[y][x] != field[y + i.y][x + i.x]) {
          ++fence;
          vec2i a{ y, x };
          vec2i b{ y + i.y, x + i.x };
          if (a[1] == b[1]) {
            fences_h[regs[y][x]].emplace_back(fence_part{ a, b });
          } else {
            fences_v[regs[y][x]].emplace_back(fence_part{ a, b });
          }
        }
      }

      plants[regs[y][x]] += vec2i{ 1, fence };
    }
  }

  int result = 0;
  for (const auto& p : plants) {
    int sides = count_gaps_v(fences_v[p.first]) + count_gaps_h(fences_h[p.first]);
    cout << p.first << ": " << reg_names[p.first] << ": " << p.second[0] << "x" << sides
#if 0
      << fences_v[p.first].size() << " "
      << fences_h[p.first].size() << " "
      << count_gaps_v(fences_v[p.first]) << " "
      << count_gaps_h(fences_h[p.first])
#endif
         << endl;

    result += p.second[0] * sides;
  }

  return result;
}
