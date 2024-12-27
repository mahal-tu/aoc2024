#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

static constexpr int EMPTY = -1;

struct span
{
  int id;
  int start;
  int len;
};

void
normalize_spans(vector<span>& v)
{
  // remove empty spans
  v.erase(remove_if(v.begin(), v.end(), [](const span& s) { return s.len == 0; }), v.end());

  // merge spans
  for (;;) {
    bool changed = false;
    for (auto it = v.begin(); it != v.end(); ++it) {
      auto next = it;
      if (next != v.end()) ++next;

      if (it->start + it->len == next->start) {
        // merge them!
        it->len += next->len;
        v.erase(next);
        changed = true;
        break; // out of inner loop
      }
    }

    if (!changed) break;
  }
}

long long int
solution1(const string& fname)
{
  vector<int> disk;

  vector<span> files;
  vector<span> voids;

  ifstream f(fname);
  while (f) {
    string line;
    std::getline(f, line);
    if (!f) break;
    if (line.empty()) continue;
    bool full = true;
    int file_idx = 0;
    for (char c : line) {
      int len = c - '0';
      if (full) {
        files.emplace_back(file_idx, disk.size(), len);
        for (int i = 0; i < len; ++i) disk.emplace_back(file_idx);
        ++file_idx;
      } else {
        voids.emplace_back(EMPTY, disk.size(), len);
        for (int i = 0; i < len; ++i) disk.emplace_back(EMPTY);
      }
      full = !full;
    }
  }

  for (const auto i : disk) {
    if (i >= 0) {
      cout << i;
    } else {
      cout << ".";
    }
  }
  cout << endl;

  auto it_free = find_if(disk.begin(), disk.end(), [](int a) { return a == EMPTY; });
  auto it_full = find_if(disk.rbegin(), disk.rend(), [](int a) { return a != EMPTY; });
  while (it_free != disk.end() && it_full != disk.rend() &&
         distance(disk.begin(), it_free) + distance(disk.rbegin(), it_full) < disk.size()) {
    swap(*it_free, *it_full);
    it_free = find_if(disk.begin(), disk.end(), [](int a) { return a == EMPTY; });
    it_full = find_if(disk.rbegin(), disk.rend(), [](int a) { return a != EMPTY; });
  }

  for (const auto i : disk) {
    if (i >= 0) {
      cout << i;
    } else {
      cout << ".";
    }
  }
  cout << endl;

  // for(const auto& s: voids) cout << s.start << ": " << s.len << endl;

  long long int checksum = 0;
  for (int i = 0; i < disk.size(); ++i) {
    if (disk[i] != EMPTY) checksum += i * disk[i];
  }

  return checksum;
}

long long int
solution2(const string& fname)
{
  vector<int> disk;

  vector<span> files;
  vector<span> voids;

  ifstream f(fname);
  while (f) {
    string line;
    std::getline(f, line);
    if (!f) break;
    if (line.empty()) continue;
    bool full = true;
    int file_idx = 0;
    for (char c : line) {
      int len = c - '0';
      if (full) {
        files.emplace_back(file_idx, disk.size(), len);
        for (int i = 0; i < len; ++i) disk.emplace_back(file_idx);
        ++file_idx;
      } else {
        voids.emplace_back(EMPTY, disk.size(), len);
        for (int i = 0; i < len; ++i) disk.emplace_back(EMPTY);
      }
      full = !full;
    }
  }

  for (const auto i : disk) {
    if (i >= 0) {
      cout << i;
    } else {
      cout << ".";
    }
  }
  cout << endl;

#if 0
  auto it_free = find_if(disk.begin(), disk.end(), [](int a) { return a == EMPTY;});
  auto it_full = find_if(disk.rbegin(), disk.rend(), [](int a) { return a != EMPTY;});
  while(it_free != disk.end() &&
      it_full != disk.rend() &&
      distance(disk.begin(), it_free) + distance(disk.rbegin(), it_full) < disk.size()) {
    swap(*it_free, *it_full);
    it_free = find_if(disk.begin(), disk.end(), [](int a) { return a == EMPTY;});
    it_full = find_if(disk.rbegin(), disk.rend(), [](int a) { return a != EMPTY;});
  }
#else
  for (auto it_file = files.rbegin(); it_file != files.rend(); ++it_file) {
    auto it_void = find_if(voids.begin(), voids.end(), [&](const span& s) { return s.len >= it_file->len; });
    if (it_void != voids.end() && it_void->start < it_file->start) {
      // move on disk level
      copy(disk.begin() + it_file->start, disk.begin() + it_file->start + it_file->len, disk.begin() + it_void->start);
      fill(disk.begin() + it_file->start, disk.begin() + it_file->start + it_file->len, EMPTY);

      // update voids
      // remove void at start
      it_void->start += it_file->len;
      it_void->len -= it_file->len;
      // add void
      voids.emplace_back(EMPTY, it_file->start, it_file->len);
      sort(voids.begin(), voids.end(), [](const span& a, const span& b) { return a.start < b.start; });

      normalize_spans(voids);
    }
  }
#endif

  for (const auto i : disk) {
    if (i >= 0) {
      cout << i;
    } else {
      cout << ".";
    }
  }
  cout << endl;

  // for(const auto& s: voids) cout << s.start << ": " << s.len << endl;

  long long int checksum = 0;
  for (int i = 0; i < disk.size(); ++i) {
    if (disk[i] != EMPTY) checksum += i * disk[i];
  }

  return checksum;
}
