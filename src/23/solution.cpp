#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
#include <set>
#include <sstream>

using namespace std;

struct lanparty
{
  map<string, int> nodes;
  map<int, string> node_names;
  // k < v
  multimap<int, int> connections;

  void init(const string& fname);

  long long int t_triples() const
  {
    set<int> good_nodes;
    for (const auto& na : nodes) {
      if (na.first.front() == 't') good_nodes.emplace(na.second);
    }

    long long int result = 0;

    // find triples that are fully interconnected
    for (const auto& na : nodes) {
      // check for all pair of connections from a whether
      // the counterparts are connected as well
      auto conns = connections.equal_range(na.second);
      for (auto i = conns.first; i != conns.second; ++i) {
        auto j = i;
        if (j != conns.second) {
          for (++j; j != conns.second; ++j) {
            auto test_min = min(i->second, j->second);
            auto test_max = max(i->second, j->second);
            auto test = connections.equal_range(test_min);
            for (auto t = test.first; t != test.second; ++t) {
              if (t->second == test_max) {
                // check whether we have a "t"-node
                if (good_nodes.count(na.second) + good_nodes.count(test_min) + good_nodes.count(test_max) > 0) {
                  cout << node_names.at(na.second) << "-" << node_names.at(test_min) << "-" << node_names.at(test_max)
                       << endl;
                  ++result;
                }
              }
            }
          }
        }
      }
    }
    return result;
  }

  bool fully_connected_to(const set<int>& s, int node) const
  {
    for (auto t : s) {
      auto test_min = min(t, node);
      auto test_max = max(t, node);
      auto test = connections.equal_range(test_min);
      bool found = false;
      for (auto t = test.first; t != test.second; ++t) {
        if (t->second == test_max) {
          found = true;
          break;
        }
      }
      if (!found) return false;
    }
    return true;
  }

  template<typename _iter>
  set<int> find_largest(set<int>& cur, _iter beg, _iter end) const
  {
    set<int> best;
    for (auto i = beg; i != end; ++i) {
      if (fully_connected_to(cur, *i)) {
        cur.emplace(*i);
        if (cur.size() > best.size()) best = cur;

        auto j = i;
        ++j;
        auto candidate = find_largest(cur, j, end);
        if (candidate.size() > best.size()) best = candidate;

        cur.erase(*i);
      }
    }
    return best;
  }

  // largest set of fully connected nodes
  string largest_set() const
  {
    set<int> bestset;
    for (const auto& na : nodes) {
      auto conns = connections.equal_range(na.second);
      set<int> options;
      for (auto i = conns.first; i != conns.second; ++i) { options.emplace(i->second); }
      set<int> workset;
      workset.emplace(na.second);
      auto candidate = find_largest(workset, options.begin(), options.end());
      if (candidate.size() > bestset.size()) { bestset = candidate; }
    }

    set<string> n;
    for (const auto i : bestset) { n.emplace(node_names.at(i)); }

    ostringstream ostr;
    for (const auto s : n) { ostr << s << ","; }
    return ostr.str();
  }
};

void
lanparty::init(const string& fname)
{
  ifstream f(fname);
  while (f) {
    string line;
    std::getline(f, line);
    if (!f) break;
    if (line.empty()) continue;
    string a = line.substr(0, 2);
    int a_idx;
    auto it_a = nodes.find(a);
    if (it_a != nodes.end()) {
      a_idx = it_a->second;
    } else {
      a_idx = nodes.size();
      nodes[a] = a_idx;
      node_names[a_idx] = a;
    }
    string b = line.substr(3, 2);
    int b_idx;
    auto it_b = nodes.find(b);
    if (it_b != nodes.end()) {
      b_idx = it_b->second;
    } else {
      b_idx = nodes.size();
      nodes[b] = b_idx;
      node_names[b_idx] = b;
    }

    connections.emplace(min(a_idx, b_idx), max(a_idx, b_idx));

    // cout << a << " " << b << " -> " << a_idx << " " << b_idx << endl;
  }
}

long long int
solution1(const string& fname)
{
  lanparty lp;
  lp.init(fname);
  return lp.t_triples();
}

string
solution2(const string& fname)
{
  lanparty lp;
  lp.init(fname);
  return lp.largest_set();
}
