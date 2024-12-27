#pragma once

#include <algorithm>
#include <array>
#include <iostream>

template<typename _t>
using vec2 = std::array<_t, 2>;

template<typename _t>
inline vec2<_t>
operator+(const vec2<_t>& a, const vec2<_t>& b)
{
  vec2<_t> r;
  r[0] = a[0] + b[0];
  r[1] = a[1] + b[1];
  return r;
}

template<typename _t>
inline vec2<_t>
operator-(const vec2<_t>& a, const vec2<_t>& b)
{
  vec2<_t> r;
  r[0] = a[0] - b[0];
  r[1] = a[1] - b[1];
  return r;
}

template<typename _t>
inline vec2<_t>
operator*(const vec2<_t>& a, _t c)
{
  return vec2<_t>{ a[0] * c, a[1] * c };
}

template<typename _t>
inline vec2<_t>&
operator+=(vec2<_t>& a, const vec2<_t>& b)
{
  a[0] += b[0];
  a[1] += b[1];
  return a;
}

template<typename _t>
inline vec2<_t>&
operator-=(vec2<_t>& a, const vec2<_t>& b)
{
  a[0] -= b[0];
  a[1] -= b[1];
  return a;
}

template<typename _t>
inline std::ostream&
operator<<(std::ostream& o, const vec2<_t>& v)
{
  o << "(" << v[0] << "," << v[1] << ")";
  return o;
}

template<typename _t>
inline bool
operator<(const vec2<_t>& a, const vec2<_t>& b)
{
  return std::lexicographical_compare(a.begin(), a.end(), b.begin(), b.end());
}

template<typename _t>
inline vec2<_t>
turn_right(const vec2<_t>& v)
{
  vec2<_t> new_dir;
  new_dir[0] = v[1];
  new_dir[1] = -v[0];
  return new_dir;
}

template<typename _t>
inline vec2<_t>
turn_left(const vec2<_t>& v)
{
  vec2<_t> new_dir;
  new_dir[0] = -v[1];
  new_dir[1] = v[0];
  return new_dir;
}

typedef vec2<int> vec2i;
typedef vec2<long long int> vec2ll;