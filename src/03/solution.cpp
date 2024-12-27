#include <fstream>
#include <iostream>

#include <regex>

using namespace std;

int
line_result(const std::string& s)
{
  regex mul_regex("mul\\((\\d+),(\\d+)\\)");

  int result = 0;
  for (auto m = sregex_iterator(s.begin(), s.end(), mul_regex); m != std::sregex_iterator(); ++m) {
    smatch sm(*m);
    if (sm.size() == 3) {
      // full match, then the two captures
      result += stoi(sm[1].str()) * stoi(sm[2].str());
    }
  }
  return result;
}

int
line_result2(const std::string& s)
{
  regex mul_regex("(do)\\(\\)|(don't)\\(\\)|(mul)\\((\\d+),(\\d+)\\)");

  int result = 0;
  static bool on_state = true;
  for (auto m = sregex_iterator(s.begin(), s.end(), mul_regex); m != std::sregex_iterator(); ++m) {
    smatch sm(*m);
    // full match, then the captures
    if (sm[1].str() == "do") {
      on_state = true;
    } else if (sm[2].str() == "don't") {
      on_state = false;
    } else if (sm[3].str() == "mul") {
      if (on_state) { result += stoi(sm[4].str()) * stoi(sm[5].str()); }
    }
  }
  return result;
}

template<int task>
int
solution(const string& fname)
{
#if 0
  string s("xmul(2,4)&mul[3,7]!^don't()_mul(5,5)+mul(32,64](mul(11,8)undo()?mul(8,5))");
  cout << line_result2(s) << endl;
  return 0;
#endif
  ifstream f(fname);

  int result = 0;
  while (f) {
    string line;
    std::getline(f, line);
    if (!f) break;
    if (line.empty()) break;

    if (task == 1) {
      result += line_result(line);
    } else if (task == 2) {
      result += line_result2(line);
    }
  }

  return result;
}
