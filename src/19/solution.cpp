#include <algorithm>
#include <boost/algorithm/string/trim.hpp>
#include <boost/tokenizer.hpp>
#include <cassert>
#include <fstream>
#include <iostream>
#include <iterator>
#include <numeric>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;
using namespace boost;

template<typename _it>
bool
try_construct(const vector<string>& towels, _it beg, _it end)
{
  if (beg == end) return true;
  if (beg > end) return false;

  size_t remaining = end - beg;
  for (const auto& t : towels) {
    if (t.length() <= remaining) {
      _it test_end = beg + t.length();
      if (equal(t.begin(), t.end(), beg, test_end)) {
        if (try_construct(towels, test_end, end)) return true;
      }
    }
  }
  return false;
}

template<typename _it>
unsigned long long
n_construct(const vector<string>& towels, _it beg, _it end, unordered_map<string, unsigned long long>& cache)
{
  if (beg == end) return 1;
  if (beg > end) return 0;

  unsigned long long options = 0;
  size_t remaining = end - beg;
  for (const auto& t : towels) {
    if (t.length() <= remaining) {
      _it test_end = beg + t.length();
      if (equal(t.begin(), t.end(), beg, test_end)) {
        string remaining(test_end, end);
        auto cached = cache.find(remaining);
        if (cached != cache.end()) {
          options += cached->second;
        } else {
          auto n = n_construct(towels, test_end, end, cache);
          options += n;
          cache[remaining] = n;
        }
      }
    }
  }
  return options;
}


template<typename _it>
size_t
count_opts(const unordered_set<string>& towels, size_t max_prefix, _it beg, _it end)
{
  if (beg == end) return 1;
  if (beg > end) return 0;

  int options = 0;
  size_t remaining = end - beg;
  for (size_t u = 1; u <= max_prefix && u <= remaining; ++u) {
    _it test_end = beg + u;
    if (towels.count(string(beg, test_end)) > 0) { options += count_opts(towels, max_prefix, test_end, end); }
  }
  return options;
}

size_t
n_options(const vector<string>& t, const vector<string>& patterns)
{
  unordered_set<string> towels;
  towels.insert(t.begin(), t.end());
  size_t max_prefix =
    accumulate(towels.begin(), towels.end(), 0, [](size_t u, const string& s) { return max(u, s.length()); });
  cout << "max prefix: " << max_prefix << endl;

  size_t result = 0;
  for (const auto& p : patterns) {
    size_t options = count_opts(towels, max_prefix, p.begin(), p.end());
    cout << p << " " << options << endl;
    result += options;
  }
  return result;
}

int
solution1(const string& fname)
{
  vector<string> towels;
  vector<string> patterns;

  ifstream f(fname);
  while (f) {
    string line;
    std::getline(f, line);
    if (!f) break;
    if (line.empty()) continue;

    if (towels.empty()) {
      char_separator<char> sep(",");
      tokenizer<char_separator<char>> tokens(line, sep);
      for (auto tok = tokens.begin(); tok != tokens.end(); ++tok) { towels.emplace_back(trim_left_copy(*tok)); }
    } else {
      patterns.emplace_back(move(line));
    }
  }

  sort(towels.begin(), towels.end());

  for (const auto& t : towels) { cout << t << endl; }

  int result = 0;

  for (const auto& p : patterns) {
    bool constructible = try_construct(towels, p.begin(), p.end());
    cout << p << " " << constructible << endl;
    if (constructible) ++result;
  }

  return result;
}

unsigned long long
solution2(const string& fname)
{
  vector<string> towels;
  vector<string> patterns;

  ifstream f(fname);
  while (f) {
    string line;
    std::getline(f, line);
    if (!f) break;
    if (line.empty()) continue;

    if (towels.empty()) {
      char_separator<char> sep(",");
      tokenizer<char_separator<char>> tokens(line, sep);
      for (auto tok = tokens.begin(); tok != tokens.end(); ++tok) { towels.emplace_back(trim_left_copy(*tok)); }
    } else {
      patterns.emplace_back(move(line));
    }
  }

  //sort(towels.begin(), towels.end());

  //for (const auto& t : towels) { cout << t << endl; }

#if 1
  unordered_map<string, unsigned long long> cache;

  unsigned long long result = 0;
  for (const auto& p : patterns) {
    unsigned long long options = n_construct(towels, p.begin(), p.end(), cache);
    //cout << p << " " << options << endl;
    result += options;
  }
  return result;
#else
  return n_options(towels, patterns);
#endif
}
