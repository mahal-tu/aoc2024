#include <array>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <vector>

#include "vec2.hpp"

using namespace std;

int
paths_to_nine(const vector<string>& field, int y, int x)
{
  if (field[y][x] == '9') return 1;

  int count_paths = 0;

  const int width = field.front().size();
  const int height = field.size();
  struct
  {
    int x, y;
  } dirs[4] = { { 1, 0 }, { 0, 1 }, { -1, 0 }, { 0, -1 } };
  for (const auto& dir : dirs) {
    int try_x = x + dir.x;
    if (try_x < 0 || try_x >= width) continue;

    int try_y = y + dir.y;
    if (try_y < 0 || try_y >= height) continue;

    if (field[try_y][try_x] - field[y][x] == 1) { count_paths += paths_to_nine(field, try_y, try_x); }
  }

  return count_paths;
}

void
trail_ends(const vector<string>& field, int y, int x, set<vec2i>& ends)
{
  if (field[y][x] == '9') ends.insert({ y, x });

  const int width = field.front().size();
  const int height = field.size();
  struct
  {
    int x, y;
  } dirs[4] = { { 1, 0 }, { 0, 1 }, { -1, 0 }, { 0, -1 } };
  for (const auto& dir : dirs) {
    int try_x = x + dir.x;
    if (try_x < 0 || try_x >= width) continue;

    int try_y = y + dir.y;
    if (try_y < 0 || try_y >= height) continue;

    if (field[try_y][try_x] - field[y][x] == 1) { trail_ends(field, try_y, try_x, ends); }
  }
}

int
solution1(const string& fname)
{
  vector<string> field;

  ifstream f(fname);
  while (f) {
    string line;
    std::getline(f, line);
    if (!f) break;
    if (line.empty()) continue;
    field.emplace_back(move(line));
  }

  // for(const auto& l : field) cout << l << endl;

  long long int score = 0;
  for (int y = 0; y < field.size(); ++y) {
    for (int x = 0; x < field[y].size(); ++x) {
      if (field[y][x] == '0') {
        set<vec2i> ends;
        trail_ends(field, y, x, ends);
        score += ends.size();
      }
    }
  }

  return score;
}

int
solution2(const string& fname)
{
  vector<string> field;

  ifstream f(fname);
  while (f) {
    string line;
    std::getline(f, line);
    if (!f) break;
    if (line.empty()) continue;
    field.emplace_back(move(line));
  }

  long long int score = 0;
  for (int y = 0; y < field.size(); ++y) {
    for (int x = 0; x < field[y].size(); ++x) {
      if (field[y][x] == '0') { score += paths_to_nine(field, y, x); }
    }
  }

  return score;
}
