#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
#include <regex>
#include <set>
#include <sstream>

#include <boost/format.hpp>

using namespace std;
using namespace boost;

enum class op
{
  AND,
  OR,
  XOR,
  ID
};

struct node
{
  string name;
  op operation;
  std::shared_ptr<node> left, right;
};

#if 1
// prefix notation for easier spotting of the start of a difference
ostream&
operator<<(ostream& o, const node& n)
{
  switch (n.operation) {
    case op::ID: o << n.name; break;
    case op::AND:
      if (n.left && n.right) o << "(& " << *n.left << " " << *n.right << ")";
      break;
    case op::OR:
      if (n.left && n.right) o << "(| " << *n.left << " " << *n.right << ")";
      break;
    case op::XOR:
      if (n.left && n.right) o << "(^ " << *n.left << " " << *n.right << ")";
      break;
  }
  return o;
}
#else
// infix notation
ostream&
operator<<(ostream& o, const node& n)
{
  switch (n.operation) {
    case op::ID: o << n.name; break;
    case op::AND:
      if (n.left && n.right) o << "(" << *n.left << " & " << *n.right << ")";
      break;
    case op::OR:
      if (n.left && n.right) o << "(" << *n.left << " | " << *n.right << ")";
      break;
    case op::XOR:
      if (n.left && n.right) o << "(" << *n.left << " ^ " << *n.right << ")";
      break;
  }
  return o;
}
#endif

std::shared_ptr<node>
carry(int bit)
{
  if (bit == 0) {
    return std::make_shared<node>("",
                                  op::AND,
                                  std::make_shared<node>(str(format("x%02d") % bit), op::ID, nullptr, nullptr),
                                  std::make_shared<node>(str(format("y%02d") % bit), op::ID, nullptr, nullptr));
  } else {
    return std::make_shared<node>(
      "",
      op::OR,
      std::make_shared<node>("",
                             op::AND,
                             std::make_shared<node>(str(format("x%02d") % bit), op::ID, nullptr, nullptr),
                             std::make_shared<node>(str(format("y%02d") % bit), op::ID, nullptr, nullptr)),
      std::make_shared<node>(
        "",
        op::AND,
        std::make_shared<node>("",
                               op::XOR,
                               std::make_shared<node>(str(format("x%02d") % bit), op::ID, nullptr, nullptr),
                               std::make_shared<node>(str(format("y%02d") % bit), op::ID, nullptr, nullptr)),
        carry(bit - 1)));
  }
}

std::shared_ptr<node>
expected_term(int bit, int max_bit)
{
  if (bit == 0) {
    return std::make_shared<node>("",
                                  op::XOR,
                                  std::make_shared<node>(str(format("x%02d") % bit), op::ID, nullptr, nullptr),
                                  std::make_shared<node>(str(format("y%02d") % bit), op::ID, nullptr, nullptr));
  } else if (bit < max_bit) {
    return std::make_shared<node>(
      "",
      op::XOR,
      std::make_shared<node>(str(format("z%02d") % bit),
                             op::XOR,
                             std::make_shared<node>(str(format("x%02d") % bit), op::ID, nullptr, nullptr),
                             std::make_shared<node>(str(format("y%02d") % bit), op::ID, nullptr, nullptr)),
      carry(bit - 1));
  } else {
    return carry(bit - 1);
  }
}

pair<std::shared_ptr<node>, string>
first_diff(const std::shared_ptr<node>& expected, const std::shared_ptr<node>& observed)
{
  if (expected->operation != observed->operation) { return make_pair(expected, observed->name); }
  if (expected->operation == op::ID) {
    if (expected->name != observed->name) { return make_pair(expected, observed->name); }
  } else {
    auto left_check = first_diff(expected->left, observed->left);
    if (left_check.first) { return left_check; }

    auto right_check = first_diff(expected->right, observed->right);
    if (right_check.first) { return right_check; }
  }

  return pair<std::shared_ptr<node>, string>(nullptr, "");
}

struct wiring
{
  map<string, bool> inputs;
  map<string, bool> outputs;

  struct conn
  {
    op operation;
    string left;
    string right;
  };

  static conn make_sorted(op o, const string& a, const string& b)
  {
    if (a < b) {
      return conn{ o, a, b };
    } else {
      return conn{ o, b, a };
    }
  }

  typedef map<string, conn> conn_t;
  conn_t wires;

  bool compute(const string& name) const
  {
    {
      auto it = inputs.find(name);
      if (it != inputs.end()) return it->second;
    }

    {
      auto it = wires.find(name);
      if (it != wires.end()) {
        switch (it->second.operation) {
          case op::AND: return compute(it->second.left) & compute(it->second.right);
          case op::OR: return compute(it->second.left) | compute(it->second.right);
          case op::XOR: return compute(it->second.left) ^ compute(it->second.right);
        }
      }
    }

    throw runtime_error("unknown bit");
  }

  void print() const
  {
    for (const auto& i : inputs) cout << i.first << " " << i.second << endl;
    for (const auto& kv : wires) {
      switch (kv.second.operation) {
        case op::AND: cout << kv.first << " = " << kv.second.left << " and " << kv.second.right << endl; break;
        case op::OR: cout << kv.first << " = " << kv.second.left << " or " << kv.second.right << endl; break;
        case op::XOR: cout << kv.first << " = " << kv.second.left << " xor " << kv.second.right << endl; break;
      }
    }
  }

  void read(const string& fname)
  {
    ifstream f(fname);

    regex rg_input("(\\w+): (\\d)");
    regex rg_or("(\\w+) OR (\\w+) -> (\\w+)");
    regex rg_and("(\\w+) AND (\\w+) -> (\\w+)");
    regex rg_xor("(\\w+) XOR (\\w+) -> (\\w+)");

    while (f) {
      string line;
      std::getline(f, line);
      if (!f) break;
      if (line.empty()) continue;

      for (auto m = sregex_iterator(line.begin(), line.end(), rg_input); m != std::sregex_iterator(); ++m) {
        smatch sm(*m);
        if (sm.size() == 3) {
          // full match, then the captures
          inputs[sm[1].str()] = stoi(sm[2].str());
        }
      }

      for (auto m = sregex_iterator(line.begin(), line.end(), rg_or); m != std::sregex_iterator(); ++m) {
        smatch sm(*m);
        if (sm.size() == 4) {
          // full match, then the captures
          wires[sm[3].str()] = make_sorted(op::OR, sm[1].str(), sm[2].str());
          if (sm[3].str().front() == 'z') outputs[sm[3].str()] = false;
        }
      }

      for (auto m = sregex_iterator(line.begin(), line.end(), rg_and); m != std::sregex_iterator(); ++m) {
        smatch sm(*m);
        if (sm.size() == 4) {
          // full match, then the captures
          wires[sm[3].str()] = make_sorted(op::AND, sm[1].str(), sm[2].str());
          if (sm[3].str().front() == 'z') outputs[sm[3].str()] = false;
        }
      }

      for (auto m = sregex_iterator(line.begin(), line.end(), rg_xor); m != std::sregex_iterator(); ++m) {
        smatch sm(*m);
        if (sm.size() == 4) {
          // full match, then the captures
          wires[sm[3].str()] = make_sorted(op::XOR, sm[1].str(), sm[2].str());
          if (sm[3].str().front() == 'z') outputs[sm[3].str()] = false;
        }
      }
    }
  }

  int min_input_bit(const string& w, int depth = 100) const
  {
    if (depth == 0) return -1;
    if (w.front() == 'x' || w.front() == 'y') return stoi(w.substr(1));
    int result = 1000;
    auto it = wires.find(w);
    if (it != wires.end()) {
      result = min(result, min_input_bit(it->second.left, depth - 1));
      result = min(result, min_input_bit(it->second.right, depth - 1));
    }
    return result;
  }

  set<string> input_conns(const string& wire, int max_depth = 100) const
  {
    if (max_depth == 0) return set<string>();

    set<string> result;
    if (wire.front() == 'x' || wire.front() == 'y') {
      result.insert(wire);
      return result;
    }
    auto it = wires.find(wire);
    if (it != wires.end()) {
      result.merge(input_conns(it->second.left, max_depth - 1));
      result.merge(input_conns(it->second.right, max_depth - 1));
    }
    return result;
  }

  void sort_ops()
  {
    for (auto& kv : wires) {
      if (min_input_bit(kv.second.left) < min_input_bit(kv.second.right)) { swap(kv.second.left, kv.second.right); }
    }
  }

  void print(ostream& o, const string& name) const
  {
    {
      auto it = inputs.find(name);
      if (it != inputs.end()) {
        o << name;
        return;
      }
    }

    {
      auto it = wires.find(name);
      if (it != wires.end()) {
        o << "(";
        print(o, it->second.left);
        switch (it->second.operation) {
          case op::OR: o << " | "; break;
          case op::AND: o << " & "; break;
          case op::XOR: o << " ^ "; break;
        }
        print(o, it->second.right);
        o << ")";
        return;
      }
    }

    throw runtime_error("unknown bit");
  }

  std::shared_ptr<node> build_term(const string& name, int depth = 200) const
  {
    if (depth < 0) return nullptr;

    {
      auto it = inputs.find(name);
      if (it != inputs.end()) { return std::make_shared<node>(name, op::ID, nullptr, nullptr); }
    }

    {
      auto it = wires.find(name);
      if (it != wires.end()) {
        return std::make_shared<node>(
          name, it->second.operation, build_term(it->second.left, depth - 1), build_term(it->second.right, depth - 1));
      }
    }

    throw runtime_error("unknown bit");
  }

  void swap_wires(const string& a, const string& b)
  {
    auto a_gate = wires[a];
    auto b_gate = wires[b];
    wires[b] = a_gate;
    wires[a] = b_gate;
  }

  void fix_from_bottom(set<string>& swaps)
  {
    // create normalized terms
    sort_ops();

    for (auto& kv : outputs) {
      auto n_expected = expected_term(stoi(kv.first.substr(1)), outputs.size() - 1);
      auto n_found = build_term(kv.first);

      // first difference in normalized term
      auto diff = first_diff(n_expected, n_found);
      if (diff.first) {
        // find wire that equals the expected node
        for (auto& wire : wires) {
          auto n_wire = build_term(wire.first);
          if (!first_diff(diff.first, n_wire).first) {
            // no difference found -> equal
            cout << wire.first << "<->" << diff.second << endl;
            swaps.insert(wire.first);
            swaps.insert(diff.second);
            swap_wires(wire.first, diff.second);
            // on to the next difference
            fix_from_bottom(swaps);
            return;
          }
        }
      }
    }
  }

  string try_fix()
  {
    set<string> swaps;
    fix_from_bottom(swaps);
    ostringstream ostr;
    copy(swaps.begin(), swaps.end(), ostream_iterator<string>(ostr, ","));
    return ostr.str();
  }
};

long long unsigned
solution1(const string& fname)
{
  wiring w;
  w.read(fname);
  // w.print();

  // compute outputs
  long long unsigned output = 0;
  for (auto& kv : w.outputs) {
    kv.second = w.compute(kv.first);
    cout << kv.first << ": " << kv.second << endl;

    output += static_cast<long long unsigned>(kv.second) << stoi(kv.first.substr(1));
  }

  return output;
}

string
solution2(const string& fname)
{
  wiring w;
  w.read(fname);
  // w.sort_ops();
  // w.print();
  return w.try_fix();
}
