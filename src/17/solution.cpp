#include <boost/dynamic_bitset.hpp>
#include <boost/tokenizer.hpp>
#include <cassert>
#include <fstream>
#include <iostream>
#include <iterator>
#include <limits>
#include <random>
#include <regex>
#include <vector>

using namespace std;
using namespace boost;

struct comp
{
  long long int reg_a;
  long long int reg_b;
  long long int reg_c;
  long long int reg_ip;

  vector<char> mem;

  long long int combo(char c)
  {
    switch (c) {
      case 0:
      case 1:
      case 2:
      case 3: return c;
      case 4: return reg_a;
      case 5: return reg_b;
      case 6: return reg_c;
      default: throw runtime_error("invalid combo");
    }
  }

  void read(const string& line);
  void run(vector<char>& o);

  unsigned long find_next(unsigned long cur_res, int depth)
  {
    if (depth >= mem.size()) return cur_res;

    auto expected_output = *(mem.rbegin() + depth);
    for (unsigned candidate = 0; candidate < 8; ++candidate) {
      comp c2(*this);
      c2.reg_a = (cur_res << 3) + candidate;
      vector<char> o;
      c2.run(o);
      if (!o.empty()) {
        // cout << candidate << "->" << int(o.front()) << endl;
        if (o.front() == expected_output) {
          auto try_result = (cur_res << 3) + candidate;
          auto recursive_result = find_next(try_result, depth + 1);
          if (recursive_result != 0) { return recursive_result; }
        }
      }
    }
    // nothing found
    return 0;
  }

  unsigned long find_start()
  {
    unsigned long result = find_next(0, 0);
    return result;
  }
};

void
comp::read(const string& s)
{
  regex reg_regex("Register (\\w+): (\\d+)");
  for (auto m = sregex_iterator(s.begin(), s.end(), reg_regex); m != std::sregex_iterator(); ++m) {
    smatch sm(*m);
    if (sm.size() == 3) {
      // full match, then the captures
      switch (sm[1].str().front()) {
        case 'A': reg_a = stoll(sm[2].str()); return;
        case 'B': reg_b = stoll(sm[2].str()); return;
        case 'C': reg_c = stoll(sm[2].str()); return;
      }
    }
  }

  if (s.substr(0, 9) == "Program: ") {
    string p = s.substr(9);
    char_separator<char> sep(",");
    tokenizer<char_separator<char>> tokens(p, sep);
    for (auto tok = tokens.begin(); tok != tokens.end(); ++tok) mem.emplace_back(stoi(*tok));
  }
}

void
comp::run(vector<char>& o)
{
  reg_ip = 0;
  while (reg_ip + 1 < mem.size()) {
    switch (mem[reg_ip]) {
      case 0: // adv
        reg_a = reg_a / (1 << combo(mem[reg_ip + 1]));
        reg_ip += 2;
        break;
      case 1: // bxl
        reg_b ^= mem[reg_ip + 1];
        reg_ip += 2;
        break;
      case 2: // bst
        reg_b = combo(mem[reg_ip + 1]) % 8;
        // cout << "b=" << reg_b << endl;
        reg_ip += 2;
        break;
      case 3: // jnz
        if (reg_a != 0) {
          // cout << "jnz " << reg_a << endl;
          reg_ip = mem[reg_ip + 1];
        } else {
          reg_ip += 2;
        }
        break;
      case 4: // bxc
        reg_b ^= reg_c;
        reg_ip += 2;
        break;
      case 5: // out
        o.emplace_back(combo(mem[reg_ip + 1]) % 8);
        reg_ip += 2;
        break;
      case 6: // bdv
        reg_b = reg_a / (1 << combo(mem[reg_ip + 1]));
        reg_ip += 2;
        break;
      case 7: // cdv
        reg_c = reg_a / (1 << combo(mem[reg_ip + 1]));
        // cout << "c=" << std::oct << reg_c << endl;
        reg_ip += 2;
        break;
    }
  }
}

// 3 bits per char used
int
bit_error(const vector<char>& a, const vector<char>& b)
{
  if (a.size() != b.size()) return max(a.size(), b.size()) * 3;

  int result = 0;
  for (size_t u = 0; u < a.size(); ++u) {
    char ca = a[u];
    char cb = b[u];
    if ((ca & 1) != (cb & 1)) ++result;
    if ((ca & 2) != (cb & 2)) ++result;
    if ((ca & 4) != (cb & 4)) ++result;
  }
  return result;
}

std::string
solution1(const string& fname)
{
  comp c;

  ifstream f(fname);
  while (f) {
    string line;
    std::getline(f, line);
    if (!f) break;
    if (line.empty()) continue;
    c.read(line);
  }

  cout << c.reg_a << " " << c.reg_b << " " << c.reg_c << " " << endl;

  copy(c.mem.begin(), c.mem.end(), ostream_iterator<int>(cout, " "));
  cout << endl;

  vector<char> o;
  c.run(o);

  ostringstream ostr;
  copy(o.begin(), o.end(), ostream_iterator<int>(ostr, ","));

  return ostr.str();
}

unsigned long
solution2(const string& fname)
{
  comp c;

  ifstream f(fname);
  while (f) {
    string line;
    std::getline(f, line);
    if (!f) break;
    if (line.empty()) continue;
    c.read(line);
  }

  cout << c.reg_a << " " << c.reg_b << " " << c.reg_c << " " << endl;

  copy(c.mem.begin(), c.mem.end(), ostream_iterator<int>(cout, " "));
  cout << endl;

#if 0
  c.reg_a = 105981155568026;
  cout << "a=" << std::oct << c.reg_a << endl;
  vector<char> o;
  c.run(o);
  copy(o.begin(), o.end(), ostream_iterator<int>(cout, " "));
  cout << endl;

  cout << "err:" << bit_error(c.mem, o) << endl;
  return 0;
#elif 1
  auto result = c.find_start();

  c.reg_a = result;
  cout << "a=" << std::oct << c.reg_a << endl;
  vector<char> o;
  c.run(o);
  copy(o.begin(), o.end(), ostream_iterator<int>(cout, " "));
  cout << endl;

  cout << "err:" << bit_error(c.mem, o) << endl;
  cout << std::oct << result << " " << std::dec << result << endl;

  return result;
#elif 1
  unsigned long result = 0;
  auto out_it = c.mem.rbegin();
  while (out_it != c.mem.rend()) {
    bool found = false;
    for (unsigned candidate = 0; candidate < 8 && !found; ++candidate) {
      comp c2(c);
      c2.reg_a = (result << 3) + candidate;
      vector<char> o;
      c2.run(o);
      if (!o.empty()) {
        // cout << candidate << "->" << int(o.front()) << endl;
        if (o.front() == *out_it) {
          found = true;
          result = (result << 3) + candidate;
          cout << oct << result << " " << o.front() << endl;
        }
      }
    }
    if (found) {
      ++out_it;
    } else {
      cerr << "dead end" << endl;
      break;
    }
  }
  return result;
#else

  size_t n_bits = 3 * c.mem.size();
  dynamic_bitset<> bits(n_bits);
  bits.set();

  int cur_bit_error;
  unsigned long cur_result;
  {
    comp c2(c);
    c2.reg_a = bits.to_ulong();
    vector<char> o;
    c2.run(o);
    cur_bit_error = bit_error(c.mem, o);
    cur_result = bits.to_ulong() << 3;
  }
  cout << "err: " << cur_bit_error << endl;

  random_device dev;
  mt19937 rand(dev());

  for (int i = 0; i < 1000000000; ++i) {
    dynamic_bitset<> mask(n_bits);
    int n_flip = 1;
    while ((rand() % 3) == 0) n_flip *= 2;
    // n_flip += rand() % 4;
    for (int c = 0; c < n_flip; ++c) {
      size_t bit = rand() % n_bits;
      mask.set(bit);
    }
    // now try
    bits ^= mask;
    comp c2(c);
    c2.reg_a = bits.to_ulong();
    vector<char> o;
    c2.run(o);
#if 0
    copy(o.begin(), o.end(), ostream_iterator<int>(cout, " "));
    cout << endl;
#endif
    int test_bit_error = bit_error(c.mem, o);
    if (test_bit_error > cur_bit_error) {
      // undo
      bits ^= mask;
    } else {
      cur_bit_error = test_bit_error;
      cur_result = min(cur_result, bits.to_ulong());
    }
#if 0
    if (cur_bit_error == 0) {
      break;
    }
#endif
  }

  cout << "err: " << cur_bit_error << endl;
  cout << oct << cur_result << " " << dec << cur_result << endl;

  return cur_result;
#endif
}
