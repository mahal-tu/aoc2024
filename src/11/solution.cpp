#include <fstream>
#include <iostream>
#include <iterator>
#include <list>
#include <map>

using namespace std;

unsigned long long int
powl10(unsigned long long int e)
{
  unsigned long long int l = 1;
  while (e > 0) {
    l *= 10;
    --e;
  }
  return l;
}

unsigned
num_digits(unsigned long long int l)
{
  unsigned result = 1;
  while (l >= 10) {
    ++result;
    l /= 10;
  }
  return result;
}

int
solution1(const string& fname)
{
  ifstream f(fname);
  auto beg = istream_iterator<unsigned long long int>(f);
  auto end = istream_iterator<unsigned long long int>();
  list<unsigned long long int> values(beg, end);

  copy(values.begin(), values.end(), std::ostream_iterator<int>(std::cout, " "));
  cout << endl;

  for (int blink = 0; blink < 25; ++blink) {
    for (auto i = values.begin(); i != values.end(); ++i) {
      if (*i == 0) {
        *i = 1;
      } else if ((num_digits(*i) % 2) == 0) {
        auto total_digits = num_digits(*i);
        auto divisor = powl10(total_digits / 2);
        unsigned long long int left = *i / divisor;
        unsigned long long int right = *i - left * divisor;
        *i = right;
        i = values.insert(i, left);
        ++i;
      } else {
        *i *= 2024;
      }
    }

    // copy(values.begin(), values.end(), std::ostream_iterator<int>(std::cout, " "));
    // cout << endl;
  }

  return values.size();
}

unsigned long long int
solution2(const string& fname)
{
  ifstream f(fname);
  auto beg = istream_iterator<unsigned long long int>(f);
  auto end = istream_iterator<unsigned long long int>();
  list<unsigned long long int> values(beg, end);

  copy(values.begin(), values.end(), std::ostream_iterator<int>(std::cout, " "));
  cout << endl;

  int cur_count = 0;
  int next_count = (cur_count + 1) % 2;
  map<unsigned long long int, unsigned long long int> counts[2];
  for (const auto l : values) counts[cur_count][l]++;

  for (int blink = 0; blink < 75; ++blink) {
    const auto& cur = counts[cur_count];
    auto& next = counts[next_count];
    next.clear();
    for (const auto lv : cur) {
      if (lv.first == 0) {
        // 0 -> 1
        next[1] += lv.second;
      } else if ((num_digits(lv.first) % 2) == 0) {
        // split if even number of digits
        auto total_digits = num_digits(lv.first);
        auto divisor = powl10(total_digits / 2);
        unsigned long long int left = lv.first / divisor;
        unsigned long long int right = lv.first - left * divisor;
        next[left] += lv.second;
        next[right] += lv.second;
      } else {
        // multiply with 2024
        next[lv.first * 2024] += lv.second;
      }
    }

    cur_count = next_count;
    next_count = (cur_count + 1) % 2;
  }

  unsigned long long int result = 0;
  for (const auto& kv : counts[cur_count]) { result += kv.second; }
  return result;
}
