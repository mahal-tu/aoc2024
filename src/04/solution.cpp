#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

vector<string>
pad_data(const vector<string>& x, size_t n)
{
  size_t w = x.front().size();
  size_t h = x.size();

  vector<string> result;
  for (size_t u = 0; u < h + 2 * n; ++u) { result.emplace_back(string(h + 2 * n, '.')); }

  // copy original content into the padded one
  for (size_t u = 0; u < h; ++u) { std::copy(x[u].begin(), x[u].end(), result[u + n].begin() + n); }

  return result;
}

int
count_targets(const vector<string>& a, int x, int y, const string& target)
{
  int result = 0;
  const int len = target.length();
  struct
  {
    int x, y;
  } idx[8] = { { 1, 0 }, { 0, 1 }, { -1, 0 }, { 0, -1 }, { 1, 1 }, { 1, -1 }, { -1, 1 }, { -1, -1 } };

  for (const auto& d : idx) {
    for (int i = 0; i < len; ++i) {
      if (target[i] != a[x + d.x * i][y + d.y * i]) break;
      if (i == len - 1) ++result;
    }
  }

  return result;
}

int
find_xmas(const vector<string>& v)
{
  const string target = "XMAS";
  const int pad = target.length() - 1;
  vector<string> data = pad_data(v, pad);

  // for(const auto& s : data) cout << s << endl;

  int total = 0;
  for (int y = pad; y < data.size() - pad; ++y) {
    for (int x = pad; x < data[y].size() - pad; ++x) {
      if (data[x][y] != target[0]) continue;
      total += count_targets(data, x, y, target);
    }
  }
  return total;
}

int
count_crosses(const vector<string>& a, int x, int y, const string& target)
{
  int result = 0;
  const int len = target.length();
  const int ofs = -len / 2;
  struct
  {
    int x, y;
  } idx[4] = { { 1, 0 }, { 0, 1 }, { -1, 0 }, { 0, -1 } };

  for (const auto& d : idx) {
    for (int i = 0; i < len; ++i) {
      int delta = i + ofs;
      int orth_x = -d.y * delta;
      int orth_y = -d.x * delta;
      if (target[i] != a[x + d.x * delta + orth_x][y + d.y * delta + orth_y]) break;
      if (target[i] != a[x + d.x * delta - orth_x][y + d.y * delta - orth_y]) break;
      if (i == len - 1) ++result;
    }
  }

  return result;
}

int
find_cross_mas(const vector<string>& v, const string& target)
{
  const int pad = target.length() / 2;
  vector<string> data = pad_data(v, pad);

  // for(const auto& s : data) cout << s << endl;

  int total = 0;
  for (int y = pad; y < data.size() - pad; ++y) {
    for (int x = pad; x < data[y].size() - pad; ++x) {
      if (data[x][y] != target[pad]) continue;
      total += count_crosses(data, x, y, target);
    }
  }
  return total;
}

template<int task>
int
solution(const string& fname)
{
  vector<string> data;

  ifstream f(fname);
  while (f) {
    string line;
    std::getline(f, line);
    if (!f) break;
    if (line.empty()) break;
    data.emplace_back(line);
  }

  if (task == 1) {
    return find_xmas(data);
  } else if (task == 2) {
    return find_cross_mas(data, "MAS");
  }

  return 0;
}
