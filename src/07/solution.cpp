#include <boost/tokenizer.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;
using namespace boost;

template<int task, typename _iter>
bool
works(_iter beg, _iter end, long long int current, long long int target)
{
  if (beg == end) return current == target;
  // try plus
  bool plus_works = works<task>(beg + 1, end, current + *beg, target);
  if (plus_works) return true;
  // try mul
  bool mul_works = works<task>(beg + 1, end, current * *beg, target);
  if (mul_works) return true;
  if (task == 2) {
    // try concat
    for (long long int multiplier = 1; true; multiplier *= 10) {
      if (multiplier > *beg) {
        long long int concat_val = current * multiplier + *beg;
        bool concat_works = works<task>(beg + 1, end, concat_val, target);
        if (concat_works) return true;
        break;
      }
    }
  }
  return false;
}

template<int task, typename _iter>
bool
works(_iter beg, _iter end, long long int target)
{
  if (beg == end) return false;
  return works<task>(beg + 1, end, *beg, target);
  ;
}

template<int task>
long long int
parse_line(const string& line)
{
  char_separator<char> sep(":");
  tokenizer<char_separator<char>> tokens(line, sep);
  auto tok = tokens.begin();
  long long int target = stoll(*tok++);
  istringstream istr(*tok);

  auto beg = istream_iterator<long long int>(istr);
  auto end = istream_iterator<long long int>();
  vector<long long int> values(beg, end);

#if 1
  cout << line << endl;
  cout << target << endl;
  copy(values.begin(), values.end(), std::ostream_iterator<int>(std::cout, ", "));
  cout << endl;
  bool ok = works<task>(values.begin(), values.end(), target);
  cout << ok << endl;
#endif

  return ok ? target : 0;
}

template<int task>
long long int
solution(const string& fname)
{
  long long int result = 0;

  ifstream f(fname);
  while (f) {
    string line;
    std::getline(f, line);
    if (!f) break;
    if (line.empty()) continue;
    result += parse_line<task>(line);
  }

  return result;
}
