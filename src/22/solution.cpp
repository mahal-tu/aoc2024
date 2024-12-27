#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <numeric>
#include <array>

using namespace std;

long long int mix(long long int secret, long long int u) {
  return secret ^ u;
}

long long int prune(long long int secret) {
  return secret % 16777216UL;
}

long long int next_secret(long long int secret) {
  long long int u = prune(mix(secret, secret*64));
  u = prune(mix(u, u/32));
  u = prune(mix(u, u*2048));
  return u;
}

long long int
solution1(const string& fname)
{
  long long int result = 0;

  ifstream f(fname);
  while (f) {
    long long int u;
    f >> u;
    if (!f) break;
    cout << u << endl;
    for(int i=0;i<2000;++i) {
      u = next_secret(u);
    }
    cout << u << endl;
    result += u;
  }

  return result;
}

constexpr int delta_hash(int d1, int d2, int d3, int d4) {
  assert(d1 >= -9 && d1 <= 9);
  assert(d2 >= -9 && d2 <= 9);
  assert(d3 >= -9 && d3 <= 9);
  assert(d4 >= -9 && d4 <= 9);
  int result = d1 + 9;
  result *= 19;
  result += d2 + 9;
  result *= 19;
  result += d3 + 9;
  result *= 19;
  result += d4 + 9;
  return result;
}

static constexpr int MAX_HASH = delta_hash(9,9,9,9);

array<int, MAX_HASH> sum;
array<int, MAX_HASH> cur;

long long int
solution2(const string& fname)
{
  fill(sum.begin(), sum.end(), 0);

  array<int, 4> history;

  ifstream f(fname);
  while (f) {
    fill(cur.begin(), cur.end(), -1);
    long long int u;
    f >> u;
    if (!f) break;
    //cout << u << " ";
    history[3] = u % 10;
    for(int i=0;i<2000;++i) {
      u = next_secret(u);
      //cout << i << " " << (u%10) << " ";
      if(i>3) {
#if 0
        cout << ((u % 10) - history[(i-1) % history.size()]) << " ";
        cout << (history[(i-1) % history.size()] - history[(i-2) % history.size()]) << " ";
        cout << (history[(i-2) % history.size()] - history[(i-3) % history.size()]) << " ";
        cout << (history[(i-3) % history.size()] - history[(i-4) % history.size()]) << " ";
#endif
        int hash = delta_hash(
          (u % 10) - history[(i-1) % history.size()],
          history[(i-1) % history.size()] - history[(i-2) % history.size()],
          history[(i-2) % history.size()] - history[(i-3) % history.size()],
          history[(i-3) % history.size()] - history[(i-4) % history.size()]
          );
        if(cur[hash] < 0) {
          cur[hash] = u % 10;
        }
      }
      history[i % history.size()] = u % 10;
      //cout << endl;
    }
    //cout << u << endl;

    for(int h=0;h<MAX_HASH;++h) {
      if(cur[h] > 0) sum[h] += cur[h];
    }
  }

#if 0
  for(int h=0;h<MAX_HASH;++h) {
    if(sum[h] > 20) {
      int hash = h;
      int d1 = (hash % 19) - 9;
      hash/=19;
      int d2 = (hash % 19) - 9;
      hash/=19;
      int d3 = (hash % 19) - 9;
      hash/=19;
      int d4 = (hash % 19) - 9;
      cout << d1 << " "
        << d2 << " "
        << d3 << " "
        << d4 << " "
        << sum[h] << endl;
    }
  }
#endif

  return accumulate(sum.begin(), sum.end(), 0, [](int a, int b) {return max(a,b);});
}
