#include "vec2.hpp"

#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <numeric>
#include <sstream>
#include <unordered_map>
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

struct pads
{
  map<char, vec2i> num_pad;
  map<char, vec2i> dir_pad;

  static constexpr vec2i num_forbidden{ 0, 3 };
  static constexpr vec2i dir_forbidden{ 0, 0 };

  static constexpr vec2i num_A{ 2, 3 };
  static constexpr vec2i dir_A{ 2, 0 };

  pads()
  {
    num_pad['7'] = vec2i{ 0, 0 };
    num_pad['8'] = vec2i{ 1, 0 };
    num_pad['9'] = vec2i{ 2, 0 };
    num_pad['4'] = vec2i{ 0, 1 };
    num_pad['5'] = vec2i{ 1, 1 };
    num_pad['6'] = vec2i{ 2, 1 };
    num_pad['1'] = vec2i{ 0, 2 };
    num_pad['2'] = vec2i{ 1, 2 };
    num_pad['3'] = vec2i{ 2, 2 };
    num_pad['0'] = vec2i{ 1, 3 };
    num_pad['A'] = vec2i{ 2, 3 };

    dir_pad['^'] = vec2i{ 1, 0 };
    dir_pad['A'] = vec2i{ 2, 0 };
    dir_pad['<'] = vec2i{ 0, 1 };
    dir_pad['v'] = vec2i{ 1, 1 };
    dir_pad['>'] = vec2i{ 2, 1 };
  }

  // policy is stable across recursion depths
  map<vec2i,map<vec2i,pair<int,int>>> policy;

  template<typename _it>
  long long int shortest_code_dir(_it beg, _it end, const vec2i& pos, int depth)
  {
    if (beg == end) return 0;
    if (depth == 0) return end - beg;

    vec2i tgt = dir_pad[*beg];
    long long int remainder;
    string right(beg + 1, end);
    static map<int, unordered_map<vec2i, unordered_map<string, long long int>>> cache;
    if (cache[depth][tgt].count(right) > 0) {
      remainder = cache[depth][tgt][right];
    } else {
      remainder = shortest_code_dir(beg + 1, end, tgt, depth);
      cache[depth][tgt][right] = remainder;
    }
    // cur goal -> (count where x was better, N)
    auto& cur_policy = policy[pos][tgt];

    static constexpr int GET_GREEDY = 100;

    if(cur_policy.second == GET_GREEDY && cur_policy.first != 0 && cur_policy.first != GET_GREEDY) {
      throw runtime_error("ambiguous policy");
    }

    bool x_better=true;
    long long int result = numeric_limits<long long int>::max();
    // x first
    if (cur_policy.second < GET_GREEDY || cur_policy.first == cur_policy.second)
    {
      ostringstream ostr;
      vec2i cur = pos;
      for (; cur[0] < tgt[0] && cur != dir_forbidden; ++cur[0]) ostr << ">";
      for (; cur[0] > tgt[0] && cur != dir_forbidden; --cur[0]) ostr << "<";
      for (; cur[1] < tgt[1] && cur != dir_forbidden; ++cur[1]) ostr << "v";
      for (; cur[1] > tgt[1] && cur != dir_forbidden; --cur[1]) ostr << "^";
      if (cur == tgt) {
        ostr << "A";
        string code(ostr.str());
        result = min(result, shortest_code_dir(code.begin(), code.end(), dir_A, depth - 1));
      }
    }
    // y first
    if (cur_policy.second < GET_GREEDY || cur_policy.first == 0)
    {
      ostringstream ostr;
      vec2i cur = pos;
      for (; cur[1] < tgt[1] && cur != dir_forbidden; ++cur[1]) ostr << "v";
      for (; cur[1] > tgt[1] && cur != dir_forbidden; --cur[1]) ostr << "^";
      for (; cur[0] < tgt[0] && cur != dir_forbidden; ++cur[0]) ostr << ">";
      for (; cur[0] > tgt[0] && cur != dir_forbidden; --cur[0]) ostr << "<";
      if (cur == tgt) {
        ostr << "A";
        string code(ostr.str());
        auto y_result = shortest_code_dir(code.begin(), code.end(), dir_A, depth - 1);
        if(y_result < result) {
          result = min(result, y_result);
          x_better = false;
        }
      }
    }

    if(cur_policy.second < GET_GREEDY) {
      cur_policy.second += 1;
      if(x_better) cur_policy.first += 1;
    }

    return result + remainder;
  }

  template<int depth, typename _it>
  long long int shortest_code(_it beg, _it end, const vec2i& pos)
  {
    if (beg == end) return 0;

    vec2i tgt = num_pad[*beg];
    long long int remainder = shortest_code<depth>(beg + 1, end, tgt);
    long long int result = numeric_limits<long long int>::max();
    // x first
    {
      ostringstream ostr;
      vec2i cur = pos;
      for (; cur[0] < tgt[0] && cur != num_forbidden; ++cur[0]) ostr << ">";
      for (; cur[0] > tgt[0] && cur != num_forbidden; --cur[0]) ostr << "<";
      for (; cur[1] < tgt[1] && cur != num_forbidden; ++cur[1]) ostr << "v";
      for (; cur[1] > tgt[1] && cur != num_forbidden; --cur[1]) ostr << "^";
      if (cur == tgt) {
        ostr << "A";
        // cout << ostr.str() << endl;
        string code(ostr.str());
        result = min(result, shortest_code_dir(code.begin(), code.end(), dir_A, depth));
      }
    }
    // y first
    {
      ostringstream ostr;
      vec2i cur = pos;
      for (; cur[1] < tgt[1] && cur != num_forbidden; ++cur[1]) ostr << "v";
      for (; cur[1] > tgt[1] && cur != num_forbidden; --cur[1]) ostr << "^";
      for (; cur[0] < tgt[0] && cur != num_forbidden; ++cur[0]) ostr << ">";
      for (; cur[0] > tgt[0] && cur != num_forbidden; --cur[0]) ostr << "<";
      if (cur == tgt) {
        ostr << "A";
        // cout << ostr.str() << endl;
        string code(ostr.str());
        result = min(result, shortest_code_dir(code.begin(), code.end(), dir_A, depth));
      }
    }

    return result + remainder;
  }
};

int
solution1(const string& fname)
{
  vector<string> codes;

  ifstream f(fname);
  while (f) {
    string line;
    std::getline(f, line);
    if (!f) break;
    if (line.empty()) continue;
    codes.emplace_back(std::move(line));
  }

  int result = 0;
  pads p;
  for (const auto& c : codes) {
    cout << c << endl;
    auto l = p.shortest_code<2>(c.begin(), c.end(), p.num_A);
    cout << l << "x" << stoi(c) << endl;
    result += l * stoi(c);
  }

#if 0
  for(const auto& kkv : p.policy) {
    for(const auto& kv : kkv.second) {
      cout << kkv.first << " " << kv.first << " -> " << kv.second.first << " " << kv.second.second << endl;
    }
  }
#endif

  return result;
}

long long int
solution2(const string& fname)
{
  vector<string> codes;

  ifstream f(fname);
  while (f) {
    string line;
    std::getline(f, line);
    if (!f) break;
    if (line.empty()) continue;
    codes.emplace_back(std::move(line));
  }

  long long int result = 0;
  pads p;
  for (const auto& c : codes) {
    cout << c << endl;
    auto l = p.shortest_code<25>(c.begin(), c.end(), p.num_A);
    cout << l << "x" << stoi(c) << endl;
    result += l * stoi(c);
  }

  return result;
}
