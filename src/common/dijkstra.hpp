#pragma once

#include "vec2.hpp"
#include <algorithm>
#include <cassert>
#include <limits>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

namespace ops {
static constexpr vec2i dirs[4] = { { 0, 1 }, { 0, -1 }, { 1, 0 }, { -1, 0 } };

struct LEFT
{
  static vec2i forward(const vec2i& s) { return s + dirs[0]; }
  static vec2i backward(const vec2i& s) { return s - dirs[0]; }
  static size_t cost() { return 1; }
};
struct RIGHT
{
  static vec2i forward(const vec2i& s) { return s + dirs[1]; }
  static vec2i backward(const vec2i& s) { return s - dirs[1]; }
  static size_t cost() { return 1; }
};
struct DOWN
{
  static vec2i forward(const vec2i& s) { return s + dirs[2]; }
  static vec2i backward(const vec2i& s) { return s - dirs[2]; }
  static size_t cost() { return 1; }
};
struct UP
{
  static vec2i forward(const vec2i& s) { return s + dirs[3]; }
  static vec2i backward(const vec2i& s) { return s - dirs[3]; }
  static size_t cost() { return 1; }
};
}

template<typename _state, typename... _ops>
struct dijkstra
{
  typedef int cost_t;
  // map to cost and index in unsolved heap
  std::unordered_map<_state, std::pair<cost_t, int>> cost;

  typedef decltype(cost)::value_type cost_type;
  std::vector<const cost_type*> unsolved_heap;
  // to construct a min heap based on the cost element it is pointing to
  static constexpr bool heap_comp(const cost_type* a, const cost_type* b) { return a->second > b->second; }

  static constexpr int INF_COST = std::numeric_limits<int>::max();

  _state beg;
  _state end;

  void add_node(const _state& s) { cost[s] = std::pair<cost_t, int>{ INF_COST, -1 }; }

  // call after node map is complete!
  void init(const _state& b, const _state& e)
  {
    beg = b;
    end = e;
    unsolved_heap.clear();
    for (auto& kv : cost) {
      kv.second.first = INF_COST;
      // index in unsolved heap is current size
      kv.second.second = unsolved_heap.size();
      unsolved_heap.emplace_back(&kv);
    }
    update_cost(beg, 0);
    assert(unsolved_heap.front()->second.first == 0);
    assert(is_heap(unsolved_heap.begin(), unsolved_heap.end(), heap_comp));
  }

  void swap_unsolved(int a, int b) {
    // swap pointers
    swap(unsolved_heap[a], unsolved_heap[b]);
    // update index within cost map
    const_cast<cost_type*>(unsolved_heap[a])->second.second = a;
    const_cast<cost_type*>(unsolved_heap[b])->second.second = b;
  }

  void pop_min_unsolved()
  {
    assert(!unsolved_heap.empty());
    const int new_size = unsolved_heap.size() - 1;
    swap_unsolved(0, new_size);

    // sort top element correctly
    for (int i = 0; i < new_size;) {
      int left_child = 2 * i + 1;
      int right_child = left_child + 1;

      int min_idx = i;
      cost_t min_val = unsolved_heap[min_idx]->second.first;
      if (left_child < new_size && unsolved_heap[left_child]->second.first < min_val) {
        min_idx = left_child;
        min_val = unsolved_heap[min_idx]->second.first;
      }

      if (right_child < new_size && unsolved_heap[right_child]->second.first < min_val) {
        min_idx = right_child;
        min_val = unsolved_heap[min_idx]->second.first;
      }

      if (min_idx != i) {
        swap_unsolved(i, min_idx);
        i = min_idx;
      } else {
        break;
      }
    }
  }

  const cost_type* lowest_unsolved()
  {
    // pop_heap(unsolved_heap.begin(), unsolved_heap.end(), heap_comp);
    pop_min_unsolved();
    auto result = unsolved_heap.back();
    unsolved_heap.pop_back();
    return result;
  }

  void update_cost(const _state& s, int new_cost)
  {
    auto it = cost.find(s);
    assert(it != cost.end());
    if (it->second.first > new_cost) {
      it->second.first = new_cost;
      // get index in unsolved map from cost entry
      int idx = it->second.second;
      while (idx != 0) {
        int parent_idx = (idx - 1) / 2;
        // ensure heap property
        if (unsolved_heap[idx]->second < unsolved_heap[parent_idx]->second) {
          swap_unsolved(idx, parent_idx);
          idx = parent_idx;
        } else {
          break;
        }
      }
      assert(is_heap(unsolved_heap.begin(), unsolved_heap.end(), heap_comp));
    }
  }

  template<typename A>
  void op_forward(const _state& cur, cost_t cur_cost)
  {
    _state s = A::forward(cur);
    if (cost.count(s) > 0) { update_cost(s, cur_cost + A::cost()); }
  }

  template<typename A, typename B, typename... C>
  void op_forward(const _state& cur, cost_t cur_cost)
  {
    _state s = A::forward(cur);
    if (cost.count(s) > 0) { update_cost(s, cur_cost + A::cost()); }
    op_forward<B, C...>(cur, cur_cost);
  }

  void run()
  {
    while (!unsolved_heap.empty()) {
      const cost_type* cur_p = lowest_unsolved();
      const _state& cur = cur_p->first;
      auto cur_cost = cur_p->second.first;
      if (cur_cost == INF_COST) break;

      // check next possible states
      op_forward<_ops...>(cur, cur_cost);
    }
  }

  int get_total_cost() const
  {
    auto it = cost.find(end);
    if (it != cost.end()) {
      return it->second.first;
    } else {
      return INF_COST;
    }
  }

  template<typename A>
  void op_backward(const _state& cur, std::vector<_state>& path, cost_t cur_cost) const
  {
    _state s = A::backward(cur);
    auto cit = cost.find(s);
    if (cit != cost.end() && cit->second.first == cur_cost - A::cost()) {
      follow(s, path, cit->second.first);
    }
  }

  template<typename A, typename B, typename... C>
  void op_backward(const _state& cur, std::vector<_state>& path, cost_t cur_cost) const
  {
    _state s = A::backward(cur);
    auto cit = cost.find(s);
    if (cit != cost.end() && cit->second.first == cur_cost - A::cost()) {
      follow(s, path, cit->second.first);
    } else {
      // only recurse if nothing found
      op_backward<B, C...>(cur, path, cur_cost);
    }
  }

  // from end to start
  void follow(const _state& cur, std::vector<_state>& path, cost_t cur_cost) const
  {
    path.push_back(cur);
    if (cur == beg) return;

    // check next possible states, but never go back
    op_backward<_ops...>(cur, path, cur_cost);
  }

  std::vector<_state> shortest_path() const
  {
    std::vector<_state> path;
    auto cit = cost.find(end);
    if (cit != cost.end() && cit->second.first != INF_COST) { follow(end, path, cit->second.first); }
    std::reverse(path.begin(), path.end());
    return path;
  }
};

typedef dijkstra<vec2i, ops::LEFT, ops::RIGHT, ops::UP, ops::DOWN> dijkstra2d;
