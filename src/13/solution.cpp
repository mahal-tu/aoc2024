#include <fstream>
#include <iostream>
#include <regex>

#include "vec2.hpp"

using namespace std;

struct claw_m
{
  vec2ll btn_a;
  vec2ll btn_b;
  vec2ll target;

  size_t num_read;

  // return true if machine is complete
  bool try_read(const std::string& s);
  void reset() { num_read = 0; }

  long long int tokens_to_win() const;
};

bool
get_btn_a(const std::string& s, vec2ll& tgt)
{
  regex rg("Button A: X\\+(\\d+), Y\\+(\\d+)");

  for (auto m = sregex_iterator(s.begin(), s.end(), rg); m != std::sregex_iterator(); ++m) {
    smatch sm(*m);
    if (sm.size() == 3) {
      // full match, then the two captures
      tgt = vec2ll({ stoi(sm[1].str()), stoi(sm[2].str()) });
      return true;
    }
  }
  return false;
}

bool
get_btn_b(const std::string& s, vec2ll& tgt)
{
  regex rg("Button B: X\\+(\\d+), Y\\+(\\d+)");

  for (auto m = sregex_iterator(s.begin(), s.end(), rg); m != std::sregex_iterator(); ++m) {
    smatch sm(*m);
    if (sm.size() == 3) {
      // full match, then the two captures
      tgt = vec2ll({ stoi(sm[1].str()), stoi(sm[2].str()) });
      return true;
    }
  }
  return false;
}

bool
get_tgt(const std::string& s, vec2ll& tgt)
{
  regex rg("Prize: X=(\\d+), Y=(\\d+)");

  for (auto m = sregex_iterator(s.begin(), s.end(), rg); m != std::sregex_iterator(); ++m) {
    smatch sm(*m);
    if (sm.size() == 3) {
      // full match, then the two captures
      tgt = vec2ll({ stoi(sm[1].str()), stoi(sm[2].str()) });
      return true;
    }
  }
  return false;
}

bool
claw_m::try_read(const std::string& s)
{
  if (get_btn_a(s, btn_a)) ++num_read;
  if (get_btn_b(s, btn_b)) ++num_read;
  if (get_tgt(s, target)) ++num_read;
  return num_read == 3;
}

long long int
claw_m::tokens_to_win() const
{
  static constexpr long long int NO_WIN = 10000000000000LL;
  long long int min_win = NO_WIN;
#if 0
  {
    double up = (double(target[1]) / double(btn_a[1]) - double(target[0]) / double(btn_a[0]));
    double down = (double(btn_b[1]) / double(btn_a[1]) - double(btn_b[0]) / double(btn_a[0]));
    double test_b = up / down;
    cout << up << "/" << down << "=" << test_b << endl;
  }
#endif
  cout << btn_a << " " << btn_b << " " << target << endl;

  auto test_b = (target[1] * btn_a[0] - target[0] * btn_a[1]) / (btn_b[1] * btn_a[0] - btn_b[0] * btn_a[1]);
  cout << "b=" << test_b << endl;

  for (long long int b = test_b; b <= test_b; ++b) {
    const vec2ll diff = target - btn_b * b;
    // cout << "diff=" << diff << endl;
    // cout << "a=" << (diff[0] / btn_a[0]) << " / " << (diff[1] / btn_a[1]) << endl;
    if ((diff[0] % btn_a[0] == 0) && (diff[1] % btn_a[1] == 0)) {
      long long int a0 = diff[0] / btn_a[0];
      long long int a1 = diff[1] / btn_a[1];
      if (a0 == a1) { min_win = min(min_win, a0 * 3 + b); }
    }
  }
  // cout << min_win << endl;
  return (min_win < NO_WIN) ? min_win : 0;

#if 0
  vec2ll start{0,0};
  static constexpr long long int NO_WIN = 10000000000000LL;
  long long int min_win = NO_WIN;
  const auto max_a = min(target[0] / btn_a[0], target[1] / btn_a[1]);
  for(long long int a=0;a<max_a;++a) {
    const vec2ll diff = target - btn_a*a;
    if((diff[0] % btn_b[0] == 0) && (diff[1] % btn_b[1] == 0)) {
      long long int b0 = diff[0] / btn_b[0];
      long long int b1 = diff[1] / btn_b[1];
      if (b0 == b1) {
        min_win = min(min_win, a*3 + b0);
      }
    }
  }
  return (min_win < NO_WIN) ? min_win : 0;
#endif
}

long long int
solution1(const string& fname)
{

  claw_m m;
  m.reset();

  ifstream f(fname);

  long long int result = 0;
  while (f) {
    string line;
    std::getline(f, line);
    if (!f) break;
    if (line.empty()) { m.reset(); }

    if (m.try_read(line)) {
      const auto tokens = m.tokens_to_win();
      cout << tokens << endl;
      result += tokens;
    }
  }

  return result;
}

long long int
solution2(const string& fname)
{

  claw_m m;
  m.reset();

  ifstream f(fname);

  long long int result = 0;
  while (f) {
    string line;
    std::getline(f, line);
    if (!f) break;
    if (line.empty()) { m.reset(); }

    if (m.try_read(line)) {
      m.target += vec2ll{ 10000000000000LL, 10000000000000LL };
      const auto tokens = m.tokens_to_win();
      cout << tokens << endl;
      result += tokens;
    }
  }

  return result;
}
