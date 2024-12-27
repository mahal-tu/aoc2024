#include <boost/tokenizer.hpp>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;
using namespace boost;

bool
conforms_with(const vector<int>& update, const vector<pair<int, int>>& rules)
{
  for (const auto& r : rules) {
    auto i1st = find(update.begin(), update.end(), r.first);
    auto i2nd = find(update.begin(), update.end(), r.second);

    if (i1st != update.end() && i2nd != update.end()) {
      if (i2nd < i1st) return false;
    }
  }

  return true;
}

vector<int>
reorder(const vector<int>& update, const vector<pair<int, int>>& rules)
{
  vector<int> result(update);

  bool change_happened;
  do {
    change_happened = false;
    for (const auto& r : rules) {
      auto i1st = find(result.begin(), result.end(), r.first);
      auto i2nd = find(result.begin(), result.end(), r.second);

      if (i1st != result.end() && i2nd != result.end()) {
        if (i2nd < i1st) {
          // swap them and restart
          swap(*i2nd, *i1st);
          change_happened = true;
          break;
        }
      }
    }
  } while (change_happened);

  return result;
}

template<int task>
int
solution(const string& fname)
{
  vector<pair<int, int>> rules;
  vector<vector<int>> updates;

  ifstream f(fname);
  while (f) {
    string line;
    std::getline(f, line);
    if (!f) break;
    if (line.empty()) continue;

    if (line.find('|') != string::npos) {
      char_separator<char> sep("|");
      tokenizer<char_separator<char>> tokens(line, sep);
      auto beg = tokens.begin();
      int first = stoi(*beg++);
      int second = stoi(*beg++);
      rules.emplace_back(pair<int, int>(first, second));
    } else {
      updates.emplace_back(vector<int>());
      char_separator<char> sep(",");
      tokenizer<char_separator<char>> tokens(line, sep);
      for (auto tok = tokens.begin(); tok != tokens.end(); ++tok) updates.back().emplace_back(stoi(*tok));
    }
  }

  // for(const auto& r : rules) cout << r.first << " < " << r.second << endl;

  int sum = 0;
  for (const auto& u : updates) {
    copy(u.begin(), u.end(), std::ostream_iterator<int>(std::cout, ", "));
    bool conforms = conforms_with(u, rules);
    cout << " -> " << conforms << endl;

    if (task == 1) {
      if (conforms) { sum += u[u.size() / 2]; }
    } else if (task == 2) {
      if (!conforms) {
        auto improved = reorder(u, rules);
        copy(improved.begin(), improved.end(), std::ostream_iterator<int>(std::cout, ", "));
        cout << endl;
        sum += improved[improved.size() / 2];
      }
    }
  }

  return sum;
}
