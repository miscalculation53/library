#pragma once

#include "../graph/sssp.hpp"

namespace internal
{
template <class W>
struct MatroidPathCost
{
  W weight{};
  int hops = 0;

private:
  int infinity_ = 0;

public:
  MatroidPathCost() = default;
  MatroidPathCost([[maybe_unused]] int zero) { assert(zero == 0); }
  MatroidPathCost(W weight, int hops) : weight(std::move(weight)), hops(hops) {}

  static MatroidPathCost infty()
  {
    MatroidPathCost result;
    result.infinity_ = 1;
    return result;
  }
  bool finite() const { return infinity_ == 0; }
  bool is_infinite() const { return infinity_ != 0; }

  friend MatroidPathCost operator+(const MatroidPathCost &a, const MatroidPathCost &b)
  {
    if (a.is_infinite() || b.is_infinite())
    {
      assert(a.finite() || b.finite() || a.infinity_ == b.infinity_);
      return a.is_infinite() ? a : b;
    }
    return MatroidPathCost(a.weight + b.weight, a.hops + b.hops);
  }
  friend MatroidPathCost operator-(const MatroidPathCost &a)
  {
    if (a.is_infinite())
    {
      auto result = a;
      result.infinity_ = -result.infinity_;
      return result;
    }
    return MatroidPathCost(-a.weight, -a.hops);
  }
  friend MatroidPathCost operator-(const MatroidPathCost &a, const MatroidPathCost &b)
  { return a + -b; }
  MatroidPathCost &operator+=(const MatroidPathCost &b) { return *this = *this + b; }
  MatroidPathCost &operator-=(const MatroidPathCost &b) { return *this = *this - b; }

  friend bool operator<(const MatroidPathCost &a, const MatroidPathCost &b)
  {
    if (a.infinity_ != b.infinity_) return a.infinity_ < b.infinity_;
    if (a.is_infinite()) return false;
    if (a.weight < b.weight) return true;
    if (b.weight < a.weight) return false;
    return a.hops < b.hops;
  }
  friend bool operator>(const MatroidPathCost &a, const MatroidPathCost &b) { return b < a; }
  friend bool operator<=(const MatroidPathCost &a, const MatroidPathCost &b) { return !(b < a); }
  friend bool operator>=(const MatroidPathCost &a, const MatroidPathCost &b) { return !(a < b); }
  friend bool operator==(const MatroidPathCost &a, const MatroidPathCost &b)
  { return !(a < b) && !(b < a); }
  friend bool operator!=(const MatroidPathCost &a, const MatroidPathCost &b) { return !(a == b); }
};
} // namespace internal
