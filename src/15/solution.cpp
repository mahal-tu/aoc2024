#include <cassert>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <regex>
#include <vector>

#include "vec2.hpp"

using namespace std;

long long int
solution1(const string& fname)
{
  ifstream f(fname);

  bool field_full(false);
  vector<string> field;
  vector<vec2ll> traj;

  while (f) {
    string line;
    std::getline(f, line);
    if (!f) break;
    if (line.empty()) {
      field_full = true;
      continue;
    }

    if (!field_full) {
      field.emplace_back(move(line));
    } else {
      for (char c : line) {
        switch (c) {
          case '^': traj.emplace_back(vec2ll{ -1, 0 }); break;
          case 'v': traj.emplace_back(vec2ll{ 1, 0 }); break;
          case '<': traj.emplace_back(vec2ll{ 0, -1 }); break;
          case '>': traj.emplace_back(vec2ll{ 0, 1 }); break;
        }
      }
    }
  }

  // find robot pos
  vec2ll p;
  for (int y = 0; y < field.size(); ++y) {
    for (int x = 0; x < field[y].size(); ++x) {
      if (field[y][x] == '@') {
        p = vec2ll{ y, x };
        field[y][x] = '.';
      }
    }
  }

  for (const auto& row : field) { cout << row << endl; }

  cout << p << endl;
  for (const auto& d : traj) cout << d << " ";
  cout << endl;

  // apply trajectory
  for (const auto& d : traj) {
    vec2ll target = p + d;
    cout << field[target[0]][target[1]];
    if (field[target[0]][target[1]] == '.') {
      // empty -> move
      p = target;
    } else if (field[target[0]][target[1]] == '#') {
      // wall -> nothing
    } else if (field[target[0]][target[1]] == 'O') {
      // box -> try to move
      vec2ll space = target + d;
      while (field[space[0]][space[1]] == 'O') space += d;
      if (field[space[0]][space[1]] == '.') {
        // we can move
        swap(field[space[0]][space[1]], field[target[0]][target[1]]);
        p = target;
      }
    }
    cout << p << endl;
  }

  for (const auto& row : field) { cout << row << endl; }

  long long int result = 0;
  for (int y = 1; y < field.size(); ++y) {
    for (int x = 1; x < field[y].size(); ++x) {
      if (field[y][x] == 'O') { result += y * 100 + x; }
    }
  }
  return result;
}

vector<string>
make_wide(const vector<string>& f)
{
  vector<string> result;
  for (const auto& r : f) {
    result.emplace_back("");
    for (char c : r) {
      switch (c) {
        case '#': result.back().append("##"); break;
        case '.': result.back().append(".."); break;
        case 'O': result.back().append("[]"); break;
        case '@': result.back().append("@."); break;
      }
    }
  }
  return result;
}

bool
can_move(const vector<string>& f, const vec2ll& p, const vec2ll& d)
{
  switch (f[p[0]][p[1]]) {
    case '.': return true;
    case '#': return false;
    case '[':
      if (d[0] != 0) {
        return can_move(f, p + d, d) && can_move(f, p + d + vec2ll{ 0, 1 }, d);
      } else {
        return can_move(f, p + d, d);
      }
    case ']':
      if (d[0] != 0) {
        return can_move(f, p + d, d) && can_move(f, p + d + vec2ll{ 0, -1 }, d);
      } else {
        return can_move(f, p + d, d);
      }
  }
  // unreachable
  return false;
}

void
do_move(vector<string>& f, const vec2ll& p, const vec2ll& d)
{
  vec2ll target = p + d;

  switch (f[target[0]][target[1]]) {
    case '[':
      if (d[1] > 0) {
        do_move(f, target + vec2ll{ 0, 1 }, d);
        do_move(f, target, d);
      } else {
        do_move(f, target, d);
        do_move(f, target + vec2ll{ 0, 1 }, d);
      }
      break;
    case ']':
      if (d[1] < 0) {
        do_move(f, target + vec2ll{ 0, -1 }, d);
        do_move(f, target, d);
        break;
      } else {
        do_move(f, target, d);
        do_move(f, target + vec2ll{ 0, -1 }, d);
        break;
      }
  }
  assert(f[target[0]][target[1]] == '.');
  swap(f[target[0]][target[1]], f[p[0]][p[1]]);
}

long long int
solution2(const string& fname)
{
  ifstream f(fname);

  bool field_full(false);
  vector<string> field;
  vector<vec2ll> traj;

  while (f) {
    string line;
    std::getline(f, line);
    if (!f) break;
    if (line.empty()) {
      field_full = true;
      continue;
    }

    if (!field_full) {
      field.emplace_back(move(line));
    } else {
      for (char c : line) {
        switch (c) {
          case '^': traj.emplace_back(vec2ll{ -1, 0 }); break;
          case 'v': traj.emplace_back(vec2ll{ 1, 0 }); break;
          case '<': traj.emplace_back(vec2ll{ 0, -1 }); break;
          case '>': traj.emplace_back(vec2ll{ 0, 1 }); break;
        }
      }
    }
  }

  field = make_wide(field);

  // find robot pos
  vec2ll p;
  for (int y = 0; y < field.size(); ++y) {
    for (int x = 0; x < field[y].size(); ++x) {
      if (field[y][x] == '@') {
        p = vec2ll{ y, x };
        field[y][x] = '.';
      }
    }
  }

  for (const auto& row : field) { cout << row << endl; }

  // apply trajectory
  for (const auto& d : traj) {
    vec2ll target = p + d;
    cout << field[target[0]][target[1]];
    if (can_move(field, target, d)) {
      do_move(field, p, d);
      p = target;
    }
    cout << p << endl;
  }

  for (const auto& row : field) { cout << row << endl; }

  long long int result = 0;
  for (int y = 1; y < field.size(); ++y) {
    for (int x = 1; x < field[y].size(); ++x) {
      if (field[y][x] == '[') { result += y * 100 + x; }
    }
  }
  return result;
}
